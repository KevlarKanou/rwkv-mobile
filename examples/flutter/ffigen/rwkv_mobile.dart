// AUTO GENERATED FILE, DO NOT EDIT.
// 
// Generated by `package:ffigen`.
// ignore_for_file: type=lint
import 'dart:ffi' as ffi;
/// rwkv_mobile c_api.h in dart
class rwkv_mobile{
/// Holds the symbol lookup function.
final ffi.Pointer<T> Function<T extends ffi.NativeType>(String symbolName) _lookup;

/// The symbols are looked up in [dynamicLibrary].
rwkv_mobile(ffi.DynamicLibrary dynamicLibrary): _lookup = dynamicLibrary.lookup;

/// The symbols are looked up with [lookup].
rwkv_mobile.fromLookup(ffi.Pointer<T> Function<T extends ffi.NativeType>(String symbolName) lookup): _lookup = lookup;

int rwkvmobile_runtime_get_available_backend_names(ffi.Pointer<ffi.Char> backend_names_buffer,
int buffer_size,
) {
  return _rwkvmobile_runtime_get_available_backend_names(backend_names_buffer,
buffer_size,
);
}

late final _rwkvmobile_runtime_get_available_backend_namesPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(ffi.Pointer<ffi.Char> , ffi.Int )>>('rwkvmobile_runtime_get_available_backend_names');
late final _rwkvmobile_runtime_get_available_backend_names = _rwkvmobile_runtime_get_available_backend_namesPtr.asFunction<int Function(ffi.Pointer<ffi.Char> , int )>();

rwkvmobile_runtime_t rwkvmobile_runtime_init_with_name(ffi.Pointer<ffi.Char> backend_name,
) {
  return _rwkvmobile_runtime_init_with_name(backend_name,
);
}

late final _rwkvmobile_runtime_init_with_namePtr = _lookup<
    ffi.NativeFunction<rwkvmobile_runtime_t Function(ffi.Pointer<ffi.Char> )>>('rwkvmobile_runtime_init_with_name');
late final _rwkvmobile_runtime_init_with_name = _rwkvmobile_runtime_init_with_namePtr.asFunction<rwkvmobile_runtime_t Function(ffi.Pointer<ffi.Char> )>();

rwkvmobile_runtime_t rwkvmobile_runtime_init_with_name_extra(ffi.Pointer<ffi.Char> backend_name,
ffi.Pointer<ffi.Void> extra,
) {
  return _rwkvmobile_runtime_init_with_name_extra(backend_name,
extra,
);
}

late final _rwkvmobile_runtime_init_with_name_extraPtr = _lookup<
    ffi.NativeFunction<rwkvmobile_runtime_t Function(ffi.Pointer<ffi.Char> , ffi.Pointer<ffi.Void> )>>('rwkvmobile_runtime_init_with_name_extra');
late final _rwkvmobile_runtime_init_with_name_extra = _rwkvmobile_runtime_init_with_name_extraPtr.asFunction<rwkvmobile_runtime_t Function(ffi.Pointer<ffi.Char> , ffi.Pointer<ffi.Void> )>();

int rwkvmobile_runtime_release(rwkvmobile_runtime_t runtime,
) {
  return _rwkvmobile_runtime_release(runtime,
);
}

late final _rwkvmobile_runtime_releasePtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t )>>('rwkvmobile_runtime_release');
late final _rwkvmobile_runtime_release = _rwkvmobile_runtime_releasePtr.asFunction<int Function(rwkvmobile_runtime_t )>();

int rwkvmobile_runtime_load_model(rwkvmobile_runtime_t runtime,
ffi.Pointer<ffi.Char> model_path,
) {
  return _rwkvmobile_runtime_load_model(runtime,
model_path,
);
}

late final _rwkvmobile_runtime_load_modelPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>>('rwkvmobile_runtime_load_model');
late final _rwkvmobile_runtime_load_model = _rwkvmobile_runtime_load_modelPtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>();

int rwkvmobile_runtime_load_tokenizer(rwkvmobile_runtime_t runtime,
ffi.Pointer<ffi.Char> vocab_file,
) {
  return _rwkvmobile_runtime_load_tokenizer(runtime,
vocab_file,
);
}

late final _rwkvmobile_runtime_load_tokenizerPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>>('rwkvmobile_runtime_load_tokenizer');
late final _rwkvmobile_runtime_load_tokenizer = _rwkvmobile_runtime_load_tokenizerPtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>();

int rwkvmobile_runtime_eval_logits(rwkvmobile_runtime_t runtime,
ffi.Pointer<ffi.Int> ids,
int ids_len,
ffi.Pointer<ffi.Float> logits,
int logits_len,
) {
  return _rwkvmobile_runtime_eval_logits(runtime,
ids,
ids_len,
logits,
logits_len,
);
}

late final _rwkvmobile_runtime_eval_logitsPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Int> , ffi.Int , ffi.Pointer<ffi.Float> , ffi.Int )>>('rwkvmobile_runtime_eval_logits');
late final _rwkvmobile_runtime_eval_logits = _rwkvmobile_runtime_eval_logitsPtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Int> , int , ffi.Pointer<ffi.Float> , int )>();

int rwkvmobile_runtime_eval_chat(rwkvmobile_runtime_t runtime,
ffi.Pointer<ffi.Char> input,
int max_tokens,
ffi.Pointer<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<ffi.Char> , ffi.Int )>> callback,
int enable_reasoning,
) {
  return _rwkvmobile_runtime_eval_chat(runtime,
input,
max_tokens,
callback,
enable_reasoning,
);
}

late final _rwkvmobile_runtime_eval_chatPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> , ffi.Int , ffi.Pointer<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<ffi.Char> , ffi.Int )>> , ffi.Int )>>('rwkvmobile_runtime_eval_chat');
late final _rwkvmobile_runtime_eval_chat = _rwkvmobile_runtime_eval_chatPtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> , int , ffi.Pointer<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<ffi.Char> , ffi.Int )>> , int )>();

int rwkvmobile_runtime_eval_chat_with_history(rwkvmobile_runtime_t handle,
ffi.Pointer<ffi.Pointer<ffi.Char>> inputs,
int num_inputs,
int max_tokens,
ffi.Pointer<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<ffi.Char> , ffi.Int )>> callback,
int enable_reasoning,
) {
  return _rwkvmobile_runtime_eval_chat_with_history(handle,
inputs,
num_inputs,
max_tokens,
callback,
enable_reasoning,
);
}

late final _rwkvmobile_runtime_eval_chat_with_historyPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Pointer<ffi.Char>> , ffi.Int , ffi.Int , ffi.Pointer<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<ffi.Char> , ffi.Int )>> , ffi.Int )>>('rwkvmobile_runtime_eval_chat_with_history');
late final _rwkvmobile_runtime_eval_chat_with_history = _rwkvmobile_runtime_eval_chat_with_historyPtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Pointer<ffi.Char>> , int , int , ffi.Pointer<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<ffi.Char> , ffi.Int )>> , int )>();

int rwkvmobile_runtime_stop_generation(rwkvmobile_runtime_t runtime,
) {
  return _rwkvmobile_runtime_stop_generation(runtime,
);
}

late final _rwkvmobile_runtime_stop_generationPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t )>>('rwkvmobile_runtime_stop_generation');
late final _rwkvmobile_runtime_stop_generation = _rwkvmobile_runtime_stop_generationPtr.asFunction<int Function(rwkvmobile_runtime_t )>();

int rwkvmobile_runtime_is_generating(rwkvmobile_runtime_t runtime,
) {
  return _rwkvmobile_runtime_is_generating(runtime,
);
}

late final _rwkvmobile_runtime_is_generatingPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t )>>('rwkvmobile_runtime_is_generating');
late final _rwkvmobile_runtime_is_generating = _rwkvmobile_runtime_is_generatingPtr.asFunction<int Function(rwkvmobile_runtime_t )>();

int rwkvmobile_runtime_set_prompt(rwkvmobile_runtime_t runtime,
ffi.Pointer<ffi.Char> prompt,
) {
  return _rwkvmobile_runtime_set_prompt(runtime,
prompt,
);
}

late final _rwkvmobile_runtime_set_promptPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>>('rwkvmobile_runtime_set_prompt');
late final _rwkvmobile_runtime_set_prompt = _rwkvmobile_runtime_set_promptPtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>();

int rwkvmobile_runtime_get_prompt(rwkvmobile_runtime_t runtime,
ffi.Pointer<ffi.Char> prompt,
int buf_len,
) {
  return _rwkvmobile_runtime_get_prompt(runtime,
prompt,
buf_len,
);
}

late final _rwkvmobile_runtime_get_promptPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> , ffi.Int )>>('rwkvmobile_runtime_get_prompt');
late final _rwkvmobile_runtime_get_prompt = _rwkvmobile_runtime_get_promptPtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> , int )>();

int rwkvmobile_runtime_gen_completion(rwkvmobile_runtime_t runtime,
ffi.Pointer<ffi.Char> prompt,
int max_tokens,
int stop_code,
ffi.Pointer<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<ffi.Char> , ffi.Int )>> callback,
) {
  return _rwkvmobile_runtime_gen_completion(runtime,
prompt,
max_tokens,
stop_code,
callback,
);
}

late final _rwkvmobile_runtime_gen_completionPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> , ffi.Int , ffi.Int , ffi.Pointer<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<ffi.Char> , ffi.Int )>> )>>('rwkvmobile_runtime_gen_completion');
late final _rwkvmobile_runtime_gen_completion = _rwkvmobile_runtime_gen_completionPtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> , int , int , ffi.Pointer<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<ffi.Char> , ffi.Int )>> )>();

int rwkvmobile_runtime_gen_completion_blocking(rwkvmobile_runtime_t runtime,
ffi.Pointer<ffi.Char> prompt,
int max_tokens,
int stop_code,
ffi.Pointer<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<ffi.Char> , ffi.Int )>> callback,
) {
  return _rwkvmobile_runtime_gen_completion_blocking(runtime,
prompt,
max_tokens,
stop_code,
callback,
);
}

late final _rwkvmobile_runtime_gen_completion_blockingPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> , ffi.Int , ffi.Int , ffi.Pointer<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<ffi.Char> , ffi.Int )>> )>>('rwkvmobile_runtime_gen_completion_blocking');
late final _rwkvmobile_runtime_gen_completion_blocking = _rwkvmobile_runtime_gen_completion_blockingPtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> , int , int , ffi.Pointer<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<ffi.Char> , ffi.Int )>> )>();

int rwkvmobile_runtime_clear_state(rwkvmobile_runtime_t runtime,
) {
  return _rwkvmobile_runtime_clear_state(runtime,
);
}

late final _rwkvmobile_runtime_clear_statePtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t )>>('rwkvmobile_runtime_clear_state');
late final _rwkvmobile_runtime_clear_state = _rwkvmobile_runtime_clear_statePtr.asFunction<int Function(rwkvmobile_runtime_t )>();

sampler_params rwkvmobile_runtime_get_sampler_params(rwkvmobile_runtime_t runtime,
) {
  return _rwkvmobile_runtime_get_sampler_params(runtime,
);
}

late final _rwkvmobile_runtime_get_sampler_paramsPtr = _lookup<
    ffi.NativeFunction<sampler_params Function(rwkvmobile_runtime_t )>>('rwkvmobile_runtime_get_sampler_params');
late final _rwkvmobile_runtime_get_sampler_params = _rwkvmobile_runtime_get_sampler_paramsPtr.asFunction<sampler_params Function(rwkvmobile_runtime_t )>();

void rwkvmobile_runtime_set_sampler_params(rwkvmobile_runtime_t runtime,
sampler_params params,
) {
  return _rwkvmobile_runtime_set_sampler_params(runtime,
params,
);
}

late final _rwkvmobile_runtime_set_sampler_paramsPtr = _lookup<
    ffi.NativeFunction<ffi.Void Function(rwkvmobile_runtime_t , sampler_params )>>('rwkvmobile_runtime_set_sampler_params');
late final _rwkvmobile_runtime_set_sampler_params = _rwkvmobile_runtime_set_sampler_paramsPtr.asFunction<void Function(rwkvmobile_runtime_t , sampler_params )>();

penalty_params rwkvmobile_runtime_get_penalty_params(rwkvmobile_runtime_t runtime,
) {
  return _rwkvmobile_runtime_get_penalty_params(runtime,
);
}

late final _rwkvmobile_runtime_get_penalty_paramsPtr = _lookup<
    ffi.NativeFunction<penalty_params Function(rwkvmobile_runtime_t )>>('rwkvmobile_runtime_get_penalty_params');
late final _rwkvmobile_runtime_get_penalty_params = _rwkvmobile_runtime_get_penalty_paramsPtr.asFunction<penalty_params Function(rwkvmobile_runtime_t )>();

void rwkvmobile_runtime_set_penalty_params(rwkvmobile_runtime_t runtime,
penalty_params params,
) {
  return _rwkvmobile_runtime_set_penalty_params(runtime,
params,
);
}

late final _rwkvmobile_runtime_set_penalty_paramsPtr = _lookup<
    ffi.NativeFunction<ffi.Void Function(rwkvmobile_runtime_t , penalty_params )>>('rwkvmobile_runtime_set_penalty_params');
late final _rwkvmobile_runtime_set_penalty_params = _rwkvmobile_runtime_set_penalty_paramsPtr.asFunction<void Function(rwkvmobile_runtime_t , penalty_params )>();

void rwkvmobile_runtime_add_adsp_library_path(ffi.Pointer<ffi.Char> path,
) {
  return _rwkvmobile_runtime_add_adsp_library_path(path,
);
}

late final _rwkvmobile_runtime_add_adsp_library_pathPtr = _lookup<
    ffi.NativeFunction<ffi.Void Function(ffi.Pointer<ffi.Char> )>>('rwkvmobile_runtime_add_adsp_library_path');
late final _rwkvmobile_runtime_add_adsp_library_path = _rwkvmobile_runtime_add_adsp_library_pathPtr.asFunction<void Function(ffi.Pointer<ffi.Char> )>();

double rwkvmobile_runtime_get_avg_decode_speed(rwkvmobile_runtime_t runtime,
) {
  return _rwkvmobile_runtime_get_avg_decode_speed(runtime,
);
}

late final _rwkvmobile_runtime_get_avg_decode_speedPtr = _lookup<
    ffi.NativeFunction<ffi.Double Function(rwkvmobile_runtime_t )>>('rwkvmobile_runtime_get_avg_decode_speed');
late final _rwkvmobile_runtime_get_avg_decode_speed = _rwkvmobile_runtime_get_avg_decode_speedPtr.asFunction<double Function(rwkvmobile_runtime_t )>();

double rwkvmobile_runtime_get_avg_prefill_speed(rwkvmobile_runtime_t runtime,
) {
  return _rwkvmobile_runtime_get_avg_prefill_speed(runtime,
);
}

late final _rwkvmobile_runtime_get_avg_prefill_speedPtr = _lookup<
    ffi.NativeFunction<ffi.Double Function(rwkvmobile_runtime_t )>>('rwkvmobile_runtime_get_avg_prefill_speed');
late final _rwkvmobile_runtime_get_avg_prefill_speed = _rwkvmobile_runtime_get_avg_prefill_speedPtr.asFunction<double Function(rwkvmobile_runtime_t )>();

int rwkvmobile_runtime_load_vision_encoder(rwkvmobile_runtime_t runtime,
ffi.Pointer<ffi.Char> encoder_path,
) {
  return _rwkvmobile_runtime_load_vision_encoder(runtime,
encoder_path,
);
}

late final _rwkvmobile_runtime_load_vision_encoderPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>>('rwkvmobile_runtime_load_vision_encoder');
late final _rwkvmobile_runtime_load_vision_encoder = _rwkvmobile_runtime_load_vision_encoderPtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>();

int rwkvmobile_runtime_release_vision_encoder(rwkvmobile_runtime_t runtime,
) {
  return _rwkvmobile_runtime_release_vision_encoder(runtime,
);
}

late final _rwkvmobile_runtime_release_vision_encoderPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t )>>('rwkvmobile_runtime_release_vision_encoder');
late final _rwkvmobile_runtime_release_vision_encoder = _rwkvmobile_runtime_release_vision_encoderPtr.asFunction<int Function(rwkvmobile_runtime_t )>();

int rwkvmobile_runtime_set_image_prompt(rwkvmobile_runtime_t runtime,
ffi.Pointer<ffi.Char> image_path,
) {
  return _rwkvmobile_runtime_set_image_prompt(runtime,
image_path,
);
}

late final _rwkvmobile_runtime_set_image_promptPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>>('rwkvmobile_runtime_set_image_prompt');
late final _rwkvmobile_runtime_set_image_prompt = _rwkvmobile_runtime_set_image_promptPtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>();

int rwkvmobile_runtime_load_whisper_encoder(rwkvmobile_runtime_t runtime,
ffi.Pointer<ffi.Char> encoder_path,
) {
  return _rwkvmobile_runtime_load_whisper_encoder(runtime,
encoder_path,
);
}

late final _rwkvmobile_runtime_load_whisper_encoderPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>>('rwkvmobile_runtime_load_whisper_encoder');
late final _rwkvmobile_runtime_load_whisper_encoder = _rwkvmobile_runtime_load_whisper_encoderPtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>();

int rwkvmobile_runtime_release_whisper_encoder(rwkvmobile_runtime_t runtime,
) {
  return _rwkvmobile_runtime_release_whisper_encoder(runtime,
);
}

late final _rwkvmobile_runtime_release_whisper_encoderPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t )>>('rwkvmobile_runtime_release_whisper_encoder');
late final _rwkvmobile_runtime_release_whisper_encoder = _rwkvmobile_runtime_release_whisper_encoderPtr.asFunction<int Function(rwkvmobile_runtime_t )>();

int rwkvmobile_runtime_set_audio_prompt(rwkvmobile_runtime_t runtime,
ffi.Pointer<ffi.Char> audio_path,
) {
  return _rwkvmobile_runtime_set_audio_prompt(runtime,
audio_path,
);
}

late final _rwkvmobile_runtime_set_audio_promptPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>>('rwkvmobile_runtime_set_audio_prompt');
late final _rwkvmobile_runtime_set_audio_prompt = _rwkvmobile_runtime_set_audio_promptPtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>();

int rwkvmobile_runtime_set_token_banned(rwkvmobile_runtime_t runtime,
ffi.Pointer<ffi.Int> token_banned,
int token_banned_len,
) {
  return _rwkvmobile_runtime_set_token_banned(runtime,
token_banned,
token_banned_len,
);
}

late final _rwkvmobile_runtime_set_token_bannedPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Int> , ffi.Int )>>('rwkvmobile_runtime_set_token_banned');
late final _rwkvmobile_runtime_set_token_banned = _rwkvmobile_runtime_set_token_bannedPtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Int> , int )>();

int rwkvmobile_runtime_set_eos_token(rwkvmobile_runtime_t runtime,
ffi.Pointer<ffi.Char> eos_token,
) {
  return _rwkvmobile_runtime_set_eos_token(runtime,
eos_token,
);
}

late final _rwkvmobile_runtime_set_eos_tokenPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>>('rwkvmobile_runtime_set_eos_token');
late final _rwkvmobile_runtime_set_eos_token = _rwkvmobile_runtime_set_eos_tokenPtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>();

int rwkvmobile_runtime_set_bos_token(rwkvmobile_runtime_t runtime,
ffi.Pointer<ffi.Char> bos_token,
) {
  return _rwkvmobile_runtime_set_bos_token(runtime,
bos_token,
);
}

late final _rwkvmobile_runtime_set_bos_tokenPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>>('rwkvmobile_runtime_set_bos_token');
late final _rwkvmobile_runtime_set_bos_token = _rwkvmobile_runtime_set_bos_tokenPtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>();

int rwkvmobile_runtime_set_user_role(rwkvmobile_runtime_t runtime,
ffi.Pointer<ffi.Char> user_role,
) {
  return _rwkvmobile_runtime_set_user_role(runtime,
user_role,
);
}

late final _rwkvmobile_runtime_set_user_rolePtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>>('rwkvmobile_runtime_set_user_role');
late final _rwkvmobile_runtime_set_user_role = _rwkvmobile_runtime_set_user_rolePtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>();

int rwkvmobile_runtime_set_response_role(rwkvmobile_runtime_t runtime,
ffi.Pointer<ffi.Char> response_role,
) {
  return _rwkvmobile_runtime_set_response_role(runtime,
response_role,
);
}

late final _rwkvmobile_runtime_set_response_rolePtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>>('rwkvmobile_runtime_set_response_role');
late final _rwkvmobile_runtime_set_response_role = _rwkvmobile_runtime_set_response_rolePtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>();

int rwkvmobile_runtime_set_thinking_token(rwkvmobile_runtime_t runtime,
ffi.Pointer<ffi.Char> thinking_token,
) {
  return _rwkvmobile_runtime_set_thinking_token(runtime,
thinking_token,
);
}

late final _rwkvmobile_runtime_set_thinking_tokenPtr = _lookup<
    ffi.NativeFunction<ffi.Int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>>('rwkvmobile_runtime_set_thinking_token');
late final _rwkvmobile_runtime_set_thinking_token = _rwkvmobile_runtime_set_thinking_tokenPtr.asFunction<int Function(rwkvmobile_runtime_t , ffi.Pointer<ffi.Char> )>();

ffi.Pointer<ffi.Char> rwkvmobile_runtime_get_response_buffer_content(rwkvmobile_runtime_t runtime,
) {
  return _rwkvmobile_runtime_get_response_buffer_content(runtime,
);
}

late final _rwkvmobile_runtime_get_response_buffer_contentPtr = _lookup<
    ffi.NativeFunction<ffi.Pointer<ffi.Char> Function(rwkvmobile_runtime_t )>>('rwkvmobile_runtime_get_response_buffer_content');
late final _rwkvmobile_runtime_get_response_buffer_content = _rwkvmobile_runtime_get_response_buffer_contentPtr.asFunction<ffi.Pointer<ffi.Char> Function(rwkvmobile_runtime_t )>();

token_ids rwkvmobile_runtime_get_response_buffer_ids(rwkvmobile_runtime_t runtime,
) {
  return _rwkvmobile_runtime_get_response_buffer_ids(runtime,
);
}

late final _rwkvmobile_runtime_get_response_buffer_idsPtr = _lookup<
    ffi.NativeFunction<token_ids Function(rwkvmobile_runtime_t )>>('rwkvmobile_runtime_get_response_buffer_ids');
late final _rwkvmobile_runtime_get_response_buffer_ids = _rwkvmobile_runtime_get_response_buffer_idsPtr.asFunction<token_ids Function(rwkvmobile_runtime_t )>();

void rwkvmobile_runtime_free_token_ids(token_ids ids,
) {
  return _rwkvmobile_runtime_free_token_ids(ids,
);
}

late final _rwkvmobile_runtime_free_token_idsPtr = _lookup<
    ffi.NativeFunction<ffi.Void Function(token_ids )>>('rwkvmobile_runtime_free_token_ids');
late final _rwkvmobile_runtime_free_token_ids = _rwkvmobile_runtime_free_token_idsPtr.asFunction<void Function(token_ids )>();

ffi.Pointer<ffi.Char> rwkvmobile_get_platform_name() {
  return _rwkvmobile_get_platform_name();
}

late final _rwkvmobile_get_platform_namePtr = _lookup<
    ffi.NativeFunction<ffi.Pointer<ffi.Char> Function()>>('rwkvmobile_get_platform_name');
late final _rwkvmobile_get_platform_name = _rwkvmobile_get_platform_namePtr.asFunction<ffi.Pointer<ffi.Char> Function()>();

ffi.Pointer<ffi.Char> rwkvmobile_get_soc_name() {
  return _rwkvmobile_get_soc_name();
}

late final _rwkvmobile_get_soc_namePtr = _lookup<
    ffi.NativeFunction<ffi.Pointer<ffi.Char> Function()>>('rwkvmobile_get_soc_name');
late final _rwkvmobile_get_soc_name = _rwkvmobile_get_soc_namePtr.asFunction<ffi.Pointer<ffi.Char> Function()>();

ffi.Pointer<ffi.Char> rwkvmobile_get_soc_partname() {
  return _rwkvmobile_get_soc_partname();
}

late final _rwkvmobile_get_soc_partnamePtr = _lookup<
    ffi.NativeFunction<ffi.Pointer<ffi.Char> Function()>>('rwkvmobile_get_soc_partname');
late final _rwkvmobile_get_soc_partname = _rwkvmobile_get_soc_partnamePtr.asFunction<ffi.Pointer<ffi.Char> Function()>();

}

final class sampler_params extends ffi.Struct{
  @ffi.Float()
  external double temperature;

  @ffi.Int()
  external int top_k;

  @ffi.Float()
  external double top_p;

}

final class penalty_params extends ffi.Struct{
  @ffi.Float()
  external double presence_penalty;

  @ffi.Float()
  external double frequency_penalty;

  @ffi.Float()
  external double penalty_decay;

}

final class token_ids extends ffi.Struct{
  external ffi.Pointer<ffi.Int> ids;

  @ffi.Int()
  external int len;

}

typedef rwkvmobile_runtime_t = ffi.Pointer<ffi.Void>;
