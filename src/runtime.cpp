#include "runtime.h"
#include "backend.h"
#include "logger.h"
#ifdef ENABLE_WEBRWKV
#include "web_rwkv_backend.h"
#endif

#ifdef ENABLE_NCNN
#include "ncnn_rwkv_backend.h"
#endif

namespace rwkvmobile {

std::string backend_enum_to_str(int backend) {
    switch (backend) {
        case RWKV_BACKEND_WEBRWKV:
            return "web-rwkv";
        case RWKV_BACKEND_NCNN:
            return "ncnn";
        default:
            return "unknown";
    }
}

int backend_str_to_enum(std::string backend) {
    if (backend == "web-rwkv") {
        return RWKV_BACKEND_WEBRWKV;
    } else if (backend == "ncnn") {
        return RWKV_BACKEND_NCNN;
    }
    return -1;
}

int runtime::init(std::string backend_name) {
    int backend_id = backend_str_to_enum(backend_name);
    if (backend_id < 0) {
        return RWKV_ERROR_BACKEND;
    }
    int ret = init(backend_id);
    if (!ret) {
        LOGI("Initialized runtime with backend: %s\n", backend_name.c_str());
    } else {
        LOGE("Failed to initialize runtime with backend: %s, errno = %d\n", backend_name.c_str(), ret);
    }
    return ret;
}

int runtime::init(int backend_id) {
    _sampler = std::unique_ptr<sampler>(new sampler);
    if (_sampler == nullptr) {
        return RWKV_ERROR_SAMPLER;
    }

    if (backend_id == RWKV_BACKEND_WEBRWKV) {
#ifdef ENABLE_WEBRWKV
        _backend = std::unique_ptr<execution_provider>(new web_rwkv_backend);
#else
        return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
#endif
    } else if(backend_id == RWKV_BACKEND_NCNN) {
#ifdef ENABLE_NCNN
        _backend = std::unique_ptr<execution_provider>(new ncnn_rwkv_backend);
#else
        return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
#endif
    } else {
        return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
    }
    return _backend->init(nullptr);
}

int runtime::load_model(std::string model_path) {
    if (_backend == nullptr) {
        return RWKV_ERROR_RUNTIME | RWKV_ERROR_INVALID_PARAMETERS;
    }
    int ret =  _backend->load_model(model_path);
    if (!ret) {
        LOGI("Loaded model from: %s\n", model_path.c_str());
        LOGD("Model num_layers: %d, num_heads: %d, hidden_size: %d, vocab_size: %d\n",
             _backend->n_layers, _backend->num_heads, _backend->hidden_size, _backend->vocab_size);
    } else {
        LOGE("Failed to load model from: %s, errno = %d\n", model_path.c_str(), ret);
    }

    // Initialize state
    _state_head = new state_node;
    if (_state_head == nullptr) {
        return RWKV_ERROR_RUNTIME | RWKV_ERROR_ALLOC;
    }
    _backend->clear_state();
    _backend->get_state(_state_head->state);
    return ret;
}

int runtime::load_tokenizer(std::string vocab_file) {
    if (_tokenizer != nullptr) {
        return RWKV_ERROR_RUNTIME | RWKV_ERROR_INVALID_PARAMETERS;
    }
    _tokenizer = std::unique_ptr<tokenizer_base>(new trie_tokenizer);
    if (_tokenizer == nullptr) {
        return RWKV_ERROR_TOKENIZER;
    }
    return _tokenizer->load(vocab_file);
}

int runtime::get_available_backend_ids(std::vector<int> &backend_ids) {
    backend_ids = std::vector<int>();

#ifdef ENABLE_WEBRWKV
    // TODO: Detect if the platform has Qualcomm Adreno proprietary vulkan driver
    // (Doesn't work with WEBRWKV)
    backend_ids.push_back(RWKV_BACKEND_WEBRWKV);
#endif

#ifdef ENABLE_NCNN
    backend_ids.push_back(RWKV_BACKEND_NCNN);
#endif
    return RWKV_SUCCESS;
}

std::string runtime::get_available_backends_str() {
    std::vector<int> backend_ids;
    get_available_backend_ids(backend_ids);
    std::string ret = "";
    for (auto id : backend_ids) {
        ret += backend_enum_to_str(id) + ",";
    }
    return ret;
}

int runtime::eval_logits(int id, std::vector<float> &logits) {
    if (_backend == nullptr) {
        return RWKV_ERROR_RUNTIME | RWKV_ERROR_INVALID_PARAMETERS;
    }
    return _backend->eval(id, logits);
}

int runtime::eval_logits(std::vector<int> ids, std::vector<float> &logits) {
    if (_backend == nullptr) {
        return RWKV_ERROR_RUNTIME | RWKV_ERROR_INVALID_PARAMETERS;
    }
    return _backend->eval(ids, logits);
}

int runtime::chat(std::string input, std::string &response, const int max_length, void (*callback)(const char *)) {
    if (_backend == nullptr || _tokenizer == nullptr) {
        return RWKV_ERROR_RUNTIME | RWKV_ERROR_INVALID_PARAMETERS;
    }
    std::string prompt = user_role + ": " + input + "\n\n" + response_role + ":";
    std::vector<int> ids = _tokenizer->encode(prompt);
    std::vector<float> logits(_vocab_size);
    response = "";
    int ret = eval_logits(ids, logits);
    if (ret) {
        return ret;
    }

    for (int i = 0; i < max_length; i++) {
        for (auto &[id, occurence] : _occurences) {
            logits[id] -=
                _frequency_penalty * occurence + _presence_penalty;
            occurence *= _penalty_decay;
        }

        int idx = _sampler->sample(logits.data(), logits.size(), _temperature, _top_k, _top_p);
        if (idx == 0) {
            break;
        }
        _occurences[idx]++;

        response += _tokenizer->decode(idx);
        if (callback) {
            callback(response.c_str());
        }

        bool stopping = false;
        for (auto &stop_code : _stop_codes) {
            if (response.size() >= stop_code.size() &&
                response.compare(response.size() - stop_code.size(), stop_code.size(), stop_code) == 0) {
                stopping = true;
                break;
            }
        }

        ret = eval_logits(idx, logits);
        if (ret) return ret;
        if (stopping) break;
    }

    return RWKV_SUCCESS;
}

int runtime::chat(std::vector<std::string> inputs, std::string &response, const int max_length, void (*callback)(const char *)) {
    if (_backend == nullptr || _tokenizer == nullptr) {
        return RWKV_ERROR_RUNTIME | RWKV_ERROR_INVALID_PARAMETERS;
    }

    // LOGD("Input list:\n");
    // for (auto input : inputs) {
    //     LOGD("  \"%s\"\n", input.c_str());
    // }
    // LOGD("\n");

    struct state_node *node = _state_head;
    int start_idx = 0;
    bool edited = false;
    while (node && node->next && start_idx < inputs.size()) {
        LOGD("Comparing state node %i hash %llu with %llu\n", start_idx, node->next->hash, hash_string(inputs[start_idx]));
        unsigned long long input_hash = hash_string(inputs[start_idx]);
        if (node->next->hash != input_hash) {
            edited = true;
            struct state_node *ptr = node;
            while(ptr->next) {
                struct state_node *tmp = ptr->next;
                ptr->next = ptr->next->next;
                _backend->free_state(tmp->state);
                delete tmp;
            }
            break;
        }
        start_idx++;
        node = node->next;
    }

    LOGD("Loading state node %i hash %llu\n", start_idx-1, node->hash);
    _backend->set_state(node->state);

    std::vector<float> logits(_vocab_size);
    response = "";
    int ret;
    for (int i = start_idx; i < inputs.size(); i++) {
        std::string prompt;
        if (i % 2 == 0) {
            prompt = user_role + ": " + inputs[i] + "\n\n";
        } else {
            prompt = response_role + ": " + inputs[i] + "\n\n";
        }
        LOGD("Processing history %i: \"%s\"\n", i, prompt.c_str());
        if (i == inputs.size() - 1) {
            prompt += response_role + ":";
        }
        std::vector<int> ids = _tokenizer->encode(prompt);
        ret = eval_logits(ids, logits);
        if (ret) return ret;
        node->next = new state_node;
        if (node->next == nullptr) {
            return RWKV_ERROR_RUNTIME | RWKV_ERROR_ALLOC;
        }
        node = node->next;
        node->hash = hash_string(inputs[i]);
        _backend->get_state(node->state);
    }

    if (edited || start_idx == 0) {
        _occurences.clear();
        for (int i = 1; i < inputs.size(); i += 2) {
            std::vector<int> ids = _tokenizer->encode(" " + inputs[i]);
            for (auto id: ids) {
                for (auto &[id, occurence] : _occurences) {
                    occurence *= _penalty_decay;
                }
                _occurences[id]++;
            }
        }
    }

    for (int i = 0; i < max_length; i++) {
        for (auto &[id, occurence] : _occurences) {
            logits[id] -=
                _frequency_penalty * occurence + _presence_penalty;
            occurence *= _penalty_decay;
        }

        int idx = _sampler->sample(logits.data(), logits.size(), _temperature, _top_k, _top_p);
        if (idx == 0) {
            break;
        }

        std::string tmp = response + _tokenizer->decode(idx);
        bool stopping = false;
        for (auto &stop_code : _stop_codes) {
            if (tmp.size() >= stop_code.size() &&
                tmp.compare(tmp.size() - stop_code.size(), stop_code.size(), stop_code) == 0) {
                stopping = true;
                break;
            }
        }

        if (stopping) {
            break;
        }

        response += _tokenizer->decode(idx);

        _occurences[idx]++;

        if (callback && !(i == 0 && response == " ")) {
            callback(response.c_str());
        }

        ret = eval_logits(idx, logits);
        if (ret) return ret;
    }

    ret = eval_logits(_tokenizer->encode(_stop_codes[0]), logits);
    if (ret) return ret;

    // remove the spaces prefix in response
    while (response.size() > 0 && response[0] == ' ') {
        response = response.substr(1);
    }

    LOGD("Response: \"%s\"\n", response.c_str());

    node->next = new state_node;
    if (node->next == nullptr) {
        return RWKV_ERROR_RUNTIME | RWKV_ERROR_ALLOC;
    }
    node = node->next;
    node->hash = hash_string(response);
    _backend->get_state(node->state);

    return RWKV_SUCCESS;
}

int runtime::gen_completion(std::string prompt, std::string &completion, int length) {
    if (_backend == nullptr || _tokenizer == nullptr) {
        return RWKV_ERROR_RUNTIME | RWKV_ERROR_INVALID_PARAMETERS;
    }
    std::vector<int> ids = _tokenizer->encode(prompt);
    std::vector<float> logits(_vocab_size);
    int ret = eval_logits(ids, logits);
    if (ret) {
        return ret;
    }

    completion = "";
    for (int i = 0; i < length; i++) {
        for (auto &[id, occurence] : _occurences) {
            logits[id] -=
                _frequency_penalty * occurence + _presence_penalty;
            occurence *= _penalty_decay;
        }

        int idx = _sampler->sample(logits.data(), logits.size(), _temperature, _top_k, _top_p);
        if (idx == 0) {
            break;
        }
        _occurences[idx]++;

        completion += _tokenizer->decode(idx);
        if (i != length - 1) {
            ret = eval_logits(idx, logits);
            if (ret) {
                return ret;
            }
        }
    }

    return RWKV_SUCCESS;
}

} // namespace rwkvmobile
