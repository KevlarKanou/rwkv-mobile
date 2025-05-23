#include <fstream>
#include <filesystem>

#include "backend.h"
#include "mnn_rwkv_backend.h"
#include "commondef.h"

#ifdef ENABLE_MNN
#include <MNN/AutoTime.hpp>
#include <MNN/Interpreter.hpp>
#include <MNN/expr/Expr.hpp>
#include <MNN/expr/ExprCreator.hpp>
#include <MNN/expr/Executor.hpp>
#include <MNN/expr/Module.hpp>
#endif

namespace rwkvmobile {

#ifdef ENABLE_MNN
int mnn_rwkv_backend::init(void * extra) {
    return RWKV_SUCCESS;
}

int mnn_rwkv_backend::load_model(std::string model_path) {
    interpreter = MNN::Interpreter::createFromFile(model_path.c_str());
    MNN::ScheduleConfig config;
    config.type = MNN_FORWARD_CPU;
    MNN::BackendConfig backendConfig;
    backendConfig.memory = MNN::BackendConfig::Memory_Low;
    backendConfig.power = MNN::BackendConfig::Power_High;
    backendConfig.precision = MNN::BackendConfig::Precision_Low;
    config.backendConfig = &backendConfig;
    session = interpreter->createSession(config);
    auto inputs = interpreter->getSessionInputAll(session);
    auto outputs = interpreter->getSessionOutputAll(session);
    n_layers = inputs.size() / 3;
    hidden_size = inputs["state0_in"]->height();
    num_heads = inputs["state1_in"]->channel();
    vocab_size = outputs["out"]->height();
    state_tensors.resize(n_layers * 3);
    for (int i = 0; i < n_layers * 3; i++) {
        state_tensors[i] = new MNN::Tensor(inputs["state" + std::to_string(i) + "_in"], MNN::Tensor::CAFFE);
        memset(state_tensors[i]->host<float>(), 0, state_tensors[i]->size());
    }
    return RWKV_SUCCESS;
}

int mnn_rwkv_backend::eval(int id, float *& logits) {
    int token = id;
    auto inputs = interpreter->getSessionInputAll(session);
    for (int i = 0; i < n_layers * 3; i++) {
        inputs["state" + std::to_string(i) + "_in"]->copyFromHostTensor(state_tensors[i]);
    }
    void* token_input_ptr = inputs["in"]->map(MNN::Tensor::MAP_TENSOR_WRITE, inputs["in"]->getDimensionType());
    memcpy(token_input_ptr, &token, sizeof(int));
    inputs["in"]->unmap(MNN::Tensor::MAP_TENSOR_WRITE,  inputs["in"]->getDimensionType(), token_input_ptr);
    interpreter->runSession(session);
    auto outputs = interpreter->getSessionOutputAll(session);
    void* logits_output_ptr = outputs["out"]->map(MNN::Tensor::MAP_TENSOR_READ, outputs["out"]->getDimensionType());
    if (logits_buffer.size() < vocab_size) {
        logits_buffer.resize(vocab_size);
    }
    memcpy(logits_buffer.data(), logits_output_ptr, vocab_size * sizeof(float));
    outputs["out"]->unmap(MNN::Tensor::MAP_TENSOR_READ, outputs["out"]->getDimensionType(), logits_output_ptr);
    for (int i = 0; i < n_layers * 3; i++) {
        outputs["state" + std::to_string(i) + "_out"]->copyToHostTensor(state_tensors[i]);
    }
    logits = logits_buffer.data();
    return RWKV_SUCCESS;
}

int mnn_rwkv_backend::eval(std::vector<int> ids, float *& logits) {
    // TODO: sequential prefill
    int ret = RWKV_SUCCESS;
    for (int i = 0; i < ids.size(); i++) {
        ret = eval(ids[i], logits);
        if (ret != RWKV_SUCCESS) {
            return ret;
        }
    }

    return RWKV_SUCCESS;
}

int mnn_rwkv_backend::get_state(std::any &state) {
    // auto new_state = std::vector<ncnn::Mat>(states.size());
    // for (int i = 0; i < states.size(); i++) {
    //     new_state[i] = states[i].clone();
    // }
    // state = new_state;
    return RWKV_SUCCESS;
}

int mnn_rwkv_backend::set_state(std::any state) {
    // auto new_state = std::any_cast<std::vector<ncnn::Mat>>(state);
    // if (new_state.size() != states.size()) {
    //     return RWKV_ERROR_INVALID_PARAMETERS;
    // }
    // for (int i = 0; i < states.size(); i++) {
    //     states[i].clone_from(new_state[i]);
    // }
    return RWKV_SUCCESS;
}

int mnn_rwkv_backend::free_state(std::any state) {
    // auto new_state = std::any_cast<std::vector<ncnn::Mat>>(state);
    // for (auto &mat : new_state) {
    //     mat.release();
    // }
    return RWKV_SUCCESS;
}

int mnn_rwkv_backend::clear_state() {
    // for (auto &state : states) {
    //     state.fill(0.0f);
    // }
    return RWKV_SUCCESS;
}

int mnn_rwkv_backend::release_model() {
    // states.clear();
    // net.clear();
    return RWKV_SUCCESS;
}

int mnn_rwkv_backend::release() {
    return RWKV_SUCCESS;
}

bool mnn_rwkv_backend::is_available() {
    // always available
    return true;
}

#else

int mnn_rwkv_backend::init(void * extra) {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

int mnn_rwkv_backend::load_model(std::string model_path) {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

int mnn_rwkv_backend::eval(int id, float *& logits) {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

int mnn_rwkv_backend::eval(std::vector<int> ids, float *& logits) {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

int mnn_rwkv_backend::clear_state() {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

int mnn_rwkv_backend::get_state(std::any &state) {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

int mnn_rwkv_backend::set_state(std::any state) {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

int mnn_rwkv_backend::free_state(std::any state) {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

int mnn_rwkv_backend::release_model() {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

int mnn_rwkv_backend::release() {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

bool mnn_rwkv_backend::is_available() {
    return false;
}

#endif

} // namespace rwkvmobile