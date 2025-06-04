#include <fstream>
#include <filesystem>

#include "backend.h"
#include "coreml_rwkv_backend.h"
#include "commondef.h"

#ifdef ENABLE_COREML
#include "rwkv-coreml.h"
#endif

namespace rwkvmobile {

#ifdef ENABLE_COREML
int coreml_rwkv_backend::init(void * extra) {
    return RWKV_SUCCESS;
}

int coreml_rwkv_backend::load_model(std::string model_path) {
    ctx = rwkv_coreml_init(model_path.c_str());
    if (ctx == NULL) {
        return RWKV_ERROR_MODEL | RWKV_ERROR_IO;
    }

    vocab_size = rwkv_coreml_get_vocab_size(ctx);
    n_layers = rwkv_coreml_get_n_layers(ctx);
    num_heads = rwkv_coreml_get_num_heads(ctx);
    hidden_size = rwkv_coreml_get_hidden_dim(ctx);

    return RWKV_SUCCESS;
}

int coreml_rwkv_backend::eval(int id, float *& logits) {
    rwkv_coreml_decode(ctx, id);
    logits = rwkv_coreml_get_logits(ctx);
    return RWKV_SUCCESS;
}

int coreml_rwkv_backend::eval(std::vector<int> ids, float *& logits) {
    // TODO: sequential prefill
    for (int i = 0; i < ids.size(); i++) {
        int ret = eval(ids[i], logits);
        if (ret != RWKV_SUCCESS) {
            return ret;
        }
    }

    return RWKV_SUCCESS;
}

int coreml_rwkv_backend::get_state(std::any &state) {
    std::vector<std::vector<uint8_t>> state_vec = rwkv_coreml_get_state(ctx);
    state = state_vec;
    return RWKV_SUCCESS;
}

int coreml_rwkv_backend::set_state(std::any state) {
    std::vector<std::vector<uint8_t>> state_vec = std::any_cast<std::vector<std::vector<uint8_t>>>(state);
    rwkv_coreml_set_state(ctx, state_vec);
    return RWKV_SUCCESS;
}

int coreml_rwkv_backend::free_state(std::any state) {
    std::vector<std::vector<uint8_t>> state_vec = std::any_cast<std::vector<std::vector<uint8_t>>>(state);
    for (int i = 0; i < state_vec.size(); i++) {
        state_vec[i].clear();
    }
    state_vec.clear();
    return RWKV_SUCCESS;
}

int coreml_rwkv_backend::clear_state() {
    rwkv_coreml_clear_state(ctx);
    return RWKV_SUCCESS;
}

int coreml_rwkv_backend::release_model() {
    if (ctx) {
        rwkv_coreml_free(ctx);
        ctx = NULL;
    }
    return RWKV_SUCCESS;
}

int coreml_rwkv_backend::release() {
    if (ctx) {
        rwkv_coreml_free(ctx);
        ctx = NULL;
    }
    return RWKV_SUCCESS;
}

bool coreml_rwkv_backend::is_available() {
    return true;
}

#else

int coreml_rwkv_backend::init(void * extra) {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

int coreml_rwkv_backend::load_model(std::string model_path) {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

int coreml_rwkv_backend::eval(int id, float *& logits) {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

int coreml_rwkv_backend::eval(std::vector<int> ids, float *& logits) {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

int coreml_rwkv_backend::clear_state() {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

int coreml_rwkv_backend::get_state(std::any &state) {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

int coreml_rwkv_backend::set_state(std::any state) {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

int coreml_rwkv_backend::free_state(std::any state) {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

int coreml_rwkv_backend::release_model() {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

int coreml_rwkv_backend::release() {
    return RWKV_ERROR_BACKEND | RWKV_ERROR_UNSUPPORTED;
}

bool coreml_rwkv_backend::is_available() {
    return false;
}

#endif

} // namespace rwkvmobile