#ifndef QNN_BACKEND_H
#define QNN_BACKEND_H

#include "backend.h"
#include "rwkv-qualcomm/Interfaces.hpp"
#include "rwkv-qualcomm/Utils/IOTensor.hpp"

namespace rwkvmobile {

class qnn_backend : public execution_provider {
public:
    int init(void * extra) override;
    int load_model(std::string model_path) override;
    int eval(int id, float *& logits) override;
    int eval(std::vector<int> ids, float *& logits) override;
    int eval_with_embeddings(const float *embeddings, int n_tokens, float *& logits) override;
    void free_logits_if_allocated(float *& logits) override {
        // persistant buffer, no need to free after use
        return;
    };
    bool is_available() override;
    int clear_state() override;
    int get_state(std::any &state) override;
    int set_state(std::any state) override;
    int free_state(std::any state) override;
    int release_model() override;
    int release() override;

private:
    std::string qnnBackendPath;
    void *qnnBackendLibraryHandle = nullptr;
    void *qnnModelHandle = nullptr;

    uint32_t powerConfigId;
    uint32_t deviceId = 0;
    uint32_t coreId = 0;

    bool isContextCreated = false;
    bool isTensorInitialized = false;

    int prefillSequenceLength = 0;
    int embdPrefillSequenceLength = 0;

    qnn::tools::rwkv_app::QnnFunctionPointers qnnFunctionPointers;

    Qnn_LogHandle_t qnnLogHandle = nullptr;
    Qnn_BackendHandle_t qnnBackendHandle = nullptr;
    Qnn_DeviceHandle_t qnnDeviceHandle = nullptr;
    std::vector<Qnn_ContextHandle_t> qnnContextHandles;

    uint32_t qnnDecodeGraphsCount = 0;
    GraphInfo_t **qnnDecodeGraphsInfo = nullptr;

    uint32_t qnnPrefillGraphsCount = 0;
    GraphInfo_t **qnnPrefillGraphsInfo = nullptr;

    uint32_t qnnEmbdGraphsCount = 0;
    GraphInfo_t **qnnEmbdGraphsInfo = nullptr;

    uint32_t qnnEmbdPrefillGraphsCount = 0;
    GraphInfo_t **qnnEmbdPrefillGraphsInfo = nullptr;

    uint32_t graphConfigsInfoCount = 0;
    GraphConfigInfo_t **graphConfigsInfo = nullptr;

    Qnn_Tensor_t *inputTensors[8] = {nullptr};
    Qnn_Tensor_t *outputTensors[8] = {nullptr};

    Qnn_Tensor_t *inputTensorsPrefill[8] = {nullptr};
    Qnn_Tensor_t *outputTensorsPrefill[8] = {nullptr};

    Qnn_Tensor_t *inputTensorsEmbd[8] = {nullptr};
    Qnn_Tensor_t *outputTensorsEmbd[8] = {nullptr};

    Qnn_Tensor_t *inputTensorsEmbdPrefill[8] = {nullptr};
    Qnn_Tensor_t *outputTensorsEmbdPrefill[8] = {nullptr};

    Qnn_Tensor_t *logitsOutputTensor = nullptr;
    Qnn_Tensor_t *tokenInputTensor = nullptr;
    Qnn_Tensor_t *tokenInputTensorPrefill = nullptr;
    Qnn_Tensor_t *tokenInputTensorEmbd = nullptr;
    Qnn_Tensor_t *tokenInputTensorEmbdPrefill = nullptr;

    IOTensor* qnnIOTensorUtils = nullptr;

    std::vector<std::unordered_map<std::string, void*>> decodeGraphsTensorNameToTensorPointer;
    std::vector<std::unordered_map<std::string, size_t>> decodeGraphsTensorNameToSize;
    std::vector<std::unordered_map<std::string, void*>> prefillGraphsTensorNameToTensorPointer;
    std::vector<std::unordered_map<std::string, size_t>> prefillGraphsTensorNameToSize;
    std::vector<std::unordered_map<std::string, void*>> embdGraphsTensorNameToTensorPointer;
    std::vector<std::unordered_map<std::string, size_t>> embdGraphsTensorNameToSize;
    std::vector<std::unordered_map<std::string, void*>> embdPrefillGraphsTensorNameToTensorPointer;
    std::vector<std::unordered_map<std::string, size_t>> embdPrefillGraphsTensorNameToSize;

    int qnn_create_power_config_id();
    int qnn_destory_power_config_id();
    int qnn_set_power_config();
    int qnn_register_op_package(std::string package_path, std::string interface_provider);
    int qnn_set_rpc_latency_and_polling();
    int qnn_initialize_tensors();

    void fill_quantized_tensor(float value, Qnn_Tensor_t *tensor);

    std::vector<float> logits_buffer;
};

}

#endif
