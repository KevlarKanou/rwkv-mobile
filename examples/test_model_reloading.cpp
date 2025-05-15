#include <iostream>
#include <chrono>
#if _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "commondef.h"
#include "runtime.h"
#include "c_api.h"

#define ENSURE_SUCCESS_OR_LOG_EXIT(x, msg) if (x != rwkvmobile::RWKV_SUCCESS) { std::cout << msg << std::endl; return 1; }

char msg0[] = "Hello!";
char msg1[] = "Hello! I'm your AI assistant. I'm here to help you with various tasks, such as answering questions, brainstorming ideas, drafting emails, writing code, providing advice, and much more.";
char msg2[] = "What's the weather like today?";

std::string response;
void callback(const char *msg, const int, const char *next) {
    // std::cout << "Callback: " << msg << std::endl;
    response = std::string(msg);
};

int main(int argc, char **argv) {
    // set stdout to be unbuffered
    setvbuf(stdout, NULL, _IONBF, 0);
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <vocab_file> <model_file1> <model_file2> <backend>" << std::endl;
        return 1;
    }

    auto vocab_file = argv[1];
    auto model1 = argv[2];
    auto model2 = argv[3];
    auto backend = argv[4];

    char *input_list[] = {
        msg0,
        msg1,
        msg2,
    };

    rwkvmobile_runtime_t runtime = rwkvmobile_runtime_init_with_name(backend);
    rwkvmobile_runtime_load_tokenizer(runtime, vocab_file);
    rwkvmobile_runtime_load_model(runtime, model1);
    rwkvmobile_runtime_set_penalty_params(runtime, {0, 0, 0});
    rwkvmobile_runtime_set_sampler_params(runtime, {1.0, 1, 1.0});

    rwkvmobile_runtime_eval_chat_with_history_async(runtime, (const char **)input_list, 3, 50, callback, 0);
    while (rwkvmobile_runtime_is_generating(runtime)) {
        std::cout << "Waiting for generation to finish..." << std::endl;
        sleep(1);
    }
    std::cout << "Response: " << response << std::endl;

    rwkvmobile_runtime_release(runtime);

    runtime = rwkvmobile_runtime_init_with_name(backend);
    rwkvmobile_runtime_load_tokenizer(runtime, vocab_file);
    rwkvmobile_runtime_load_model(runtime, model2);
    rwkvmobile_runtime_set_penalty_params(runtime, {0, 0, 0});
    rwkvmobile_runtime_set_sampler_params(runtime, {1.0, 1, 1.0});

    rwkvmobile_runtime_eval_chat_with_history_async(runtime, (const char **)input_list, 3, 50, callback, 0);
    while (rwkvmobile_runtime_is_generating(runtime)) {
        std::cout << "Waiting for generation to finish..." << std::endl;
        sleep(1);
    }
    std::cout << "Response: " << response << std::endl;

    rwkvmobile_runtime_release(runtime);

    runtime = rwkvmobile_runtime_init_with_name(backend);
    rwkvmobile_runtime_load_tokenizer(runtime, vocab_file);
    rwkvmobile_runtime_load_model(runtime, model1);
    rwkvmobile_runtime_set_penalty_params(runtime, {0, 0, 0});
    rwkvmobile_runtime_set_sampler_params(runtime, {1.0, 1, 1.0});

    rwkvmobile_runtime_eval_chat_with_history_async(runtime, (const char **)input_list, 3, 50, callback, 0);
    while (rwkvmobile_runtime_is_generating(runtime)) {
        std::cout << "Waiting for generation to finish..." << std::endl;
        sleep(1);
    }
    std::cout << "Response: " << response << std::endl;

    rwkvmobile_runtime_release(runtime);

    runtime = rwkvmobile_runtime_init_with_name(backend);
    rwkvmobile_runtime_load_tokenizer(runtime, vocab_file);
    rwkvmobile_runtime_load_model(runtime, model2);
    rwkvmobile_runtime_set_penalty_params(runtime, {0, 0, 0});
    rwkvmobile_runtime_set_sampler_params(runtime, {1.0, 1, 1.0});

    rwkvmobile_runtime_eval_chat_with_history_async(runtime, (const char **)input_list, 3, 50, callback, 0);
    while (rwkvmobile_runtime_is_generating(runtime)) {
        std::cout << "Waiting for generation to finish..." << std::endl;
        sleep(1);
    }
    std::cout << "Response: " << response << std::endl;

    rwkvmobile_runtime_release(runtime);
    return 0;
}
