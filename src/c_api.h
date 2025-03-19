#ifndef C_API_H
#define C_API_H

typedef void * rwkvmobile_runtime_t;
typedef void * rwkvmobile_vision_encoder_t;

struct sampler_params {
    float temperature;
    int top_k;
    float top_p;
};

struct penalty_params {
    float presence_penalty;
    float frequency_penalty;
    float penalty_decay;
};

struct token_ids {
    int * ids;
    int len;
};

#ifdef __cplusplus
extern "C" {
#endif

int rwkvmobile_runtime_get_available_backend_names(char * backend_names_buffer, int buffer_size);

rwkvmobile_runtime_t rwkvmobile_runtime_init_with_name(const char * backend_name);

rwkvmobile_runtime_t rwkvmobile_runtime_init_with_name_extra(const char * backend_name, void * extra);

int rwkvmobile_runtime_release(rwkvmobile_runtime_t runtime);

int rwkvmobile_runtime_load_model(rwkvmobile_runtime_t runtime, const char * model_path);

int rwkvmobile_runtime_load_tokenizer(rwkvmobile_runtime_t runtime, const char * vocab_file);

int rwkvmobile_runtime_eval_logits(rwkvmobile_runtime_t runtime, const int *ids, int ids_len, float * logits, int logits_len);

int rwkvmobile_runtime_eval_chat(rwkvmobile_runtime_t runtime, const char * input, const int max_tokens, void (*callback)(const char *, const int), int enable_reasoning);

int rwkvmobile_runtime_eval_chat_with_history(rwkvmobile_runtime_t handle, const char ** inputs, const int num_inputs, const int max_tokens, void (*callback)(const char *, const int), int enable_reasoning);

int rwkvmobile_runtime_stop_generation(rwkvmobile_runtime_t runtime);

int rwkvmobile_runtime_is_generating(rwkvmobile_runtime_t runtime);

int rwkvmobile_runtime_set_prompt(rwkvmobile_runtime_t runtime, const char * prompt);

int rwkvmobile_runtime_get_prompt(rwkvmobile_runtime_t runtime, char * prompt, const int buf_len);

int rwkvmobile_runtime_gen_completion(rwkvmobile_runtime_t runtime, const char * prompt, const int max_tokens, const int stop_code, void (*callback)(const char *, const int));

int rwkvmobile_runtime_gen_completion_blocking(rwkvmobile_runtime_t runtime, const char * prompt, const int max_tokens, const int stop_code, void (*callback)(const char *, const int));

int rwkvmobile_runtime_clear_state(rwkvmobile_runtime_t runtime);

struct sampler_params rwkvmobile_runtime_get_sampler_params(rwkvmobile_runtime_t runtime);

void rwkvmobile_runtime_set_sampler_params(rwkvmobile_runtime_t runtime, struct sampler_params params);

struct penalty_params rwkvmobile_runtime_get_penalty_params(rwkvmobile_runtime_t runtime);

void rwkvmobile_runtime_set_penalty_params(rwkvmobile_runtime_t runtime, struct penalty_params params);

void rwkvmobile_runtime_add_adsp_library_path(const char * path);

double rwkvmobile_runtime_get_avg_decode_speed(rwkvmobile_runtime_t runtime);

double rwkvmobile_runtime_get_avg_prefill_speed(rwkvmobile_runtime_t runtime);

// Vision
int rwkvmobile_runtime_load_vision_encoder(rwkvmobile_runtime_t runtime, const char * encoder_path);

int rwkvmobile_runtime_release_vision_encoder(rwkvmobile_runtime_t runtime);

int rwkvmobile_runtime_set_image_prompt(rwkvmobile_runtime_t runtime, const char * image_path);

// Whisper
int rwkvmobile_runtime_load_whisper_encoder(rwkvmobile_runtime_t runtime, const char * encoder_path);

int rwkvmobile_runtime_release_whisper_encoder(rwkvmobile_runtime_t runtime);

int rwkvmobile_runtime_set_audio_prompt(rwkvmobile_runtime_t runtime, const char * audio_path);

int rwkvmobile_runtime_set_token_banned(rwkvmobile_runtime_t runtime, const int * token_banned, int token_banned_len);

int rwkvmobile_runtime_set_eos_token(rwkvmobile_runtime_t runtime, const char * eos_token);

int rwkvmobile_runtime_set_bos_token(rwkvmobile_runtime_t runtime, const char * bos_token);

int rwkvmobile_runtime_set_user_role(rwkvmobile_runtime_t runtime, const char * user_role);

int rwkvmobile_runtime_set_response_role(rwkvmobile_runtime_t runtime, const char * response_role);

int rwkvmobile_runtime_set_thinking_token(rwkvmobile_runtime_t runtime, const char * thinking_token);

const char * rwkvmobile_runtime_get_response_buffer_content(rwkvmobile_runtime_t runtime);

struct token_ids rwkvmobile_runtime_get_response_buffer_ids(rwkvmobile_runtime_t runtime);

void rwkvmobile_runtime_free_token_ids(struct token_ids ids);

#ifdef __cplusplus
}
#endif

#endif // C_API_H