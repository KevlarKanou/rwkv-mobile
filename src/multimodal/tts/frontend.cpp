#include "frontend.h"
#include "audio.h"
#include "logger.h"
#include "librosa.h"
#include <chrono>
#include <random>
#include "onnxruntime_cxx_api.h"
#include "kaldi-native-fbank/csrc/feature-fbank.h"
#include "kaldi-native-fbank/csrc/online-feature.h"
#include "kaldi-native-fbank/csrc/istft.h"

#define PRINT_FEATURE_INFO 1

static void debug_print_mean_std(std::vector<float> feat, std::string name) {
#if PRINT_FEATURE_INFO
    LOGI("[TTS] %s.size(): %d", name.c_str(), feat.size());
    float mean = 0.0f;
    float std = 0.0f;
    for (int i = 0; i < feat.size(); i++) {
        mean += feat[i];
    }
    mean /= feat.size();
    for (int i = 0; i < feat.size(); i++) {
        std += (feat[i] - mean) * (feat[i] - mean);
    }
    std = std::sqrt(std / (feat.size()));
    LOGI("[TTS] %s Mean: %f, Std: %f", name.c_str(), mean, std);
#endif
}

static void debug_print_mean_std_2d(std::vector<std::vector<float>> feat, std::string name) {
#if PRINT_FEATURE_INFO
    LOGI("[TTS] %s.size(): %dx%d", name.c_str(), feat.size(), feat[0].size());
    float mean = 0.0f;
    float std = 0.0f;
    for (int i = 0; i < feat.size(); i++) {
        for (int j = 0; j < feat[i].size(); j++) {
            mean += feat[i][j];
        }
    }
    mean /= feat.size() * feat[0].size();
    for (int i = 0; i < feat.size(); i++) {
        for (int j = 0; j < feat[i].size(); j++) {
            std += (feat[i][j] - mean) * (feat[i][j] - mean);
        }
    }
    std = std::sqrt(std / (feat.size() * feat[0].size()));
    LOGI("[TTS] %s Mean: %f, Std: %f", name.c_str(), mean, std);
#endif
}

namespace rwkvmobile {

bool frontend::load_speech_tokenizer(const std::string model_path) {
    if (env == nullptr) {
        env = new Ort::Env(ORT_LOGGING_LEVEL_WARNING, "rwkv_mobile");
    }
    Ort::SessionOptions session_options;
    session_options.SetGraphOptimizationLevel(ORT_ENABLE_ALL);
    speech_tokenizer_session = new Ort::Session(*env, model_path.c_str(), session_options);
    return true;
}

bool frontend::load_campplus(const std::string model_path) {
    if (env == nullptr) {
        env = new Ort::Env(ORT_LOGGING_LEVEL_WARNING, "rwkv_mobile");
    }
    Ort::SessionOptions session_options;
    session_options.SetGraphOptimizationLevel(ORT_ENABLE_ALL);
    campplus_session = new Ort::Session(*env, model_path.c_str(), session_options);
    return true;
}

bool frontend::load_flow_encoder(const std::string model_path) {
    if (env == nullptr) {
        env = new Ort::Env(ORT_LOGGING_LEVEL_WARNING, "rwkv_mobile");
    }
    Ort::SessionOptions session_options;
    session_options.SetGraphOptimizationLevel(ORT_ENABLE_ALL);
    flow_encoder_session = new Ort::Session(*env, model_path.c_str(), session_options);
    return true;
}

bool frontend::load_flow_decoder_estimator(const std::string model_path) {
    if (env == nullptr) {
        env = new Ort::Env(ORT_LOGGING_LEVEL_WARNING, "rwkv_mobile");
    }
    Ort::SessionOptions session_options;
    session_options.SetGraphOptimizationLevel(ORT_ENABLE_ALL);
    flow_decoder_estimator_session = new Ort::Session(*env, model_path.c_str(), session_options);
    return true;
}

bool frontend::load_hift_generator(const std::string model_path) {
    if (env == nullptr) {
        env = new Ort::Env(ORT_LOGGING_LEVEL_WARNING, "rwkv_mobile");
    }
    Ort::SessionOptions session_options;
    session_options.SetGraphOptimizationLevel(ORT_ENABLE_ALL);
    hift_generator_session = new Ort::Session(*env, model_path.c_str(), session_options);
    return true;
}

std::vector<int> frontend::extract_speech_tokens(std::vector<float> audio_samples, int sample_rate) {
    if (speech_tokenizer_session == nullptr) {
        LOGE("[TTS] speech_tokenizer model not loaded.")
        return std::vector<int>();
    }
    auto start = std::chrono::high_resolution_clock::now();
    int fmin = 0;
    int fmax = sample_rate / 2;
    int n_fft = 400;
    int n_hop = 160;
    int n_mel = 128;
    std::vector<std::vector<float>> mels = logMelSpectrogram(audio_samples, sample_rate, n_fft, n_hop, n_mel, fmin, fmax, 2.0, true, false);
    auto end = std::chrono::high_resolution_clock::now();
    LOGI("[TTS] extract_speech_tokens Log-Melspectrogram duration: %lld ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    LOGD("[TTS] mels.size(): %dx%d", mels.size(), mels[0].size());

    debug_print_mean_std_2d(mels, "24000Hz mel");

    Ort::RunOptions run_options;
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::AllocatorWithDefaultOptions allocator;

    std::vector<int64_t> input_shape = {1, static_cast<int64_t>(mels.size()), static_cast<int64_t>(mels[0].size())};
    std::vector<int64_t> feat_len_shape = {1};

    int32_t feat_len = mels[0].size();
    Ort::Value feat_input = Ort::Value::CreateTensor<float>(allocator, input_shape.data(), input_shape.size());
    for (int i = 0; i < mels.size(); i++) {
        memcpy(feat_input.GetTensorMutableData<float>() + i * mels[i].size(), mels[i].data(), mels[i].size() * sizeof(float));
    }
    Ort::Value feat_len_input = Ort::Value::CreateTensor<int32_t>(memory_info, &feat_len, 1, feat_len_shape.data(), feat_len_shape.size());

    std::vector<const char*> input_names = {"feats", "feats_length"};
    std::vector<const char*> output_names = {"indices"};
    std::vector<Ort::Value> inputs;
    inputs.push_back(std::move(feat_input));
    inputs.push_back(std::move(feat_len_input));

    auto encoder_output = speech_tokenizer_session->Run(run_options, input_names.data(), inputs.data(), 2, output_names.data(), 1);
    auto output = encoder_output[0].GetTensorMutableData<int32_t>();
    int64_t output_size = encoder_output[0].GetTensorTypeAndShapeInfo().GetElementCount();
    std::vector<int> output_vector(output_size);
    std::memcpy(output_vector.data(), output, output_size * sizeof(int32_t));

    return output_vector;
}

std::vector<float> frontend::extract_speech_embedding(std::vector<float> audio_samples, int sample_rate) {
    if (campplus_session == nullptr) {
        LOGE("[TTS] speech_tokenizer model not loaded.")
        return std::vector<float>();
    }

    knf::FbankOptions opts;
    opts.frame_opts.dither = 0;
    opts.frame_opts.samp_freq = sample_rate;
    opts.mel_opts.num_bins = 80;
    knf::OnlineFbank fbank(opts);
    fbank.AcceptWaveform(sample_rate, audio_samples.data(), audio_samples.size());
    int32_t n = fbank.NumFramesReady();
    std::vector<std::vector<float>> feat_kaldi;
    for (int i = 0; i < n; i++) {
        feat_kaldi.emplace_back(std::move(std::vector<float>(fbank.GetFrame(i), fbank.GetFrame(i) + 80)));
    }

    debug_print_mean_std_2d(feat_kaldi, "feat_kaldi");

    std::vector<float> mean(80, 0.f);
    for (int i = 0; i < feat_kaldi.size(); i++) {
        for (int j = 0; j < feat_kaldi[i].size(); j++) {
            mean[j] += feat_kaldi[i][j];
        }
    }
    for (int j = 0; j < 80; j++) {
        mean[j] /= feat_kaldi.size();
    }
    for (int i = 0; i < feat_kaldi.size(); i++) {
        for (int j = 0; j < feat_kaldi[i].size(); j++) {
            feat_kaldi[i][j] -= mean[j];
        }
    }

    Ort::RunOptions run_options;
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::AllocatorWithDefaultOptions allocator;

    std::vector<int64_t> input_shape = {1, static_cast<int64_t>(feat_kaldi.size()), static_cast<int64_t>(feat_kaldi[0].size())};
    Ort::Value feat_input = Ort::Value::CreateTensor<float>(allocator, input_shape.data(), input_shape.size());
    for (int i = 0; i < feat_kaldi.size(); i++) {
        memcpy(feat_input.GetTensorMutableData<float>() + i * feat_kaldi[i].size(), feat_kaldi[i].data(), feat_kaldi[i].size() * sizeof(float));
    }

    std::vector<const char*> input_names = {"input"};
    std::vector<const char*> output_names = {"output"};
    std::vector<Ort::Value> inputs;
    inputs.push_back(std::move(feat_input));

    auto encoder_output = campplus_session->Run(run_options, input_names.data(), inputs.data(), 1, output_names.data(), 1);
    auto output = encoder_output[0].GetTensorMutableData<float>();
    int64_t output_size = encoder_output[0].GetTensorTypeAndShapeInfo().GetElementCount();
    std::vector<float> output_vector(output_size);
    std::memcpy(output_vector.data(), output, output_size * sizeof(float));
    LOGD("[TTS] speech embedding size: %d", output_vector.size());
    debug_print_mean_std(output_vector, "speech_embedding");

    return output_vector;
}

bool frontend::process_zeroshot(const std::string prompt_audio_path, std::vector<int> &speech_tokens, std::vector<std::vector<float>> &speech_features, std::vector<float> &speech_embedding, const int resample_rate) {
    if (speech_tokenizer_session == nullptr) {
        LOGE("[TTS] Speech tokenizer is not loaded");
        return false;
    }

    wav_file prompt_audio;
    prompt_audio.load(prompt_audio_path);
    if (prompt_audio.samples.size() / prompt_audio.sample_rate > 30) {
        LOGE("[TTS] Prompt audio is too long: should be less than 30 seconds");
        return false;
    }
    auto original_sample_rate = prompt_audio.sample_rate;
    auto original_samples = prompt_audio.samples;
    auto original_num_samples = prompt_audio.num_samples;

    if (prompt_audio.sample_rate != 16000) {
        LOGI("[TTS] Resampling prompt audio to 16000 Hz");
        prompt_audio.resample(16000);
        if (prompt_audio.sample_rate != 16000) {
            LOGE("[TTS] Resample to %d Hz failed", 16000);
            return false;
        }
    }
    prompt_audio.bit_depth = 16;
    prompt_audio.num_channels = 1;
    prompt_audio.byte_rate = 16000 * 16 / 8;
    prompt_audio.block_align = 2;
    prompt_audio.audio_format = 1;
    prompt_audio.num_samples = prompt_audio.samples.size();
    prompt_audio.save("prompt_audio_16k.wav");

    auto samples_16k = prompt_audio.samples;
    auto start = std::chrono::high_resolution_clock::now();
    speech_tokens = extract_speech_tokens(samples_16k, 16000);
    auto end = std::chrono::high_resolution_clock::now();
    LOGI("[TTS] extract_speech_tokens duration: %lld ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

    prompt_audio.samples = original_samples;
    prompt_audio.sample_rate = original_sample_rate;
    prompt_audio.num_samples = original_num_samples;
    prompt_audio.resample(resample_rate);
    if (prompt_audio.sample_rate != resample_rate) {
        LOGE("[TTS] Resample to %d Hz failed", resample_rate);
        return false;
    }

    int fmin = 0;
    int fmax = 8000;
    int n_fft = 1920;
    int n_hop = 480;
    int n_mel = 80;
    start = std::chrono::high_resolution_clock::now();
    speech_features = melSpectrogram(prompt_audio.samples, resample_rate, n_fft, n_hop, n_mel, fmin, fmax, 1.0, true, true);
    dynamic_range_compression(speech_features);
    end = std::chrono::high_resolution_clock::now();
    LOGI("[TTS] feat_extractor duration: %lld ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

    debug_print_mean_std_2d(speech_features, "speech_features");

    if (resample_rate == 24000) {
        int token_length = std::min(speech_features[0].size() / 2, speech_tokens.size());
        for (int i = 0; i < speech_features.size(); i++) {
            speech_features[i].resize(token_length * 2);
        }
        speech_tokens.resize(token_length);
    }
    LOGD("[TTS] speech_features.size(): %dx%d", speech_features.size(), speech_features[0].size());
    LOGD("[TTS] speech_tokens.size(): %d", speech_tokens.size());

    start = std::chrono::high_resolution_clock::now();
    speech_embedding = extract_speech_embedding(samples_16k, 16000);
    end = std::chrono::high_resolution_clock::now();
    LOGI("[TTS] extract_speech_embedding duration: %lld ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

    return true;
}

std::vector<int> frontend::get_llm_tokens(const std::vector<int> tts_tokens, const std::vector<int> prompt_tokens, int &min_len, int &max_len) {
    std::vector<int> tokens(tts_tokens.size() + prompt_tokens.size());
    for (int i = 0; i < prompt_tokens.size(); i++) {
        tokens[i] = prompt_tokens[i];
    }
    for (int i = 0; i < tts_tokens.size(); i++) {
        tokens[prompt_tokens.size() + i] = tts_tokens[i];
    }

    int content_length = tokens.size();
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i] == 65531) {
            content_length = content_length - (i + 1);
            break;
        }
    }

    float max_token_text_ratio = 20;
    float min_token_text_ratio = 2;
    min_len = content_length * min_token_text_ratio;
    max_len = content_length * max_token_text_ratio;
    LOGI("[TTS] min_len: %d, max_len: %d", min_len, max_len);

    int sos_eos_token = 72110;
    int task_token = 72111;
    tokens.insert(tokens.begin(), sos_eos_token);
    tokens.push_back(task_token);

    std::string debug_msg = "tokens: [";
    for (int i = 0; i < tokens.size(); i++) {
        debug_msg += std::to_string(tokens[i]) + ", ";
    }
    LOGI("[TTS] %s]", debug_msg.c_str());

    return tokens;
}

bool frontend::speech_token_to_wav(const std::vector<int> tokens, const std::vector<std::vector<float>> speech_features, const std::vector<float> speech_embedding, const std::string output_path) {
    if (flow_encoder_session == nullptr) {
        LOGE("[TTS] Flow encoder is not loaded");
        return false;
    }

    if (flow_decoder_estimator_session == nullptr) {
        LOGE("[TTS] Flow decoder estimator is not loaded");
        return false;
    }

    if (hift_generator_session == nullptr) {
        LOGE("[TTS] Hift generator is not loaded");
        return false;
    }

    LOGI("[TTS] tokens.size(): %d", tokens.size());
    LOGI("[TTS] speech_features.size(): %dx%d", speech_features.size(), speech_features[0].size());
    LOGI("[TTS] speech_embedding.size(): %d", speech_embedding.size());

    // Flow encoder

    Ort::RunOptions run_options;
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::AllocatorWithDefaultOptions allocator;

    std::vector<int64_t> token_input_shape = {1, static_cast<int64_t>(tokens.size())};
    std::vector<int64_t> speech_feature_input_shape = {1, static_cast<int64_t>(speech_features.size()), static_cast<int64_t>(speech_features[0].size())};
    std::vector<int64_t> speech_embedding_input_shape = {1, static_cast<int64_t>(speech_embedding.size())};
    Ort::Value token_input = Ort::Value::CreateTensor<int32_t>(allocator, token_input_shape.data(), token_input_shape.size());
    Ort::Value speech_feature_input = Ort::Value::CreateTensor<float>(allocator, speech_feature_input_shape.data(), speech_feature_input_shape.size());
    Ort::Value speech_embedding_input = Ort::Value::CreateTensor<float>(allocator, speech_embedding_input_shape.data(), speech_embedding_input_shape.size());

    memcpy(token_input.GetTensorMutableData<int32_t>(), tokens.data(), tokens.size() * sizeof(int32_t));
    memcpy(speech_embedding_input.GetTensorMutableData<float>(), speech_embedding.data(), speech_embedding.size() * sizeof(float));
    for (int i = 0; i < speech_features.size(); i++) {
        memcpy(speech_feature_input.GetTensorMutableData<float>() + i * speech_features[i].size(), speech_features[i].data(), speech_features[i].size() * sizeof(float));
    }

    std::vector<const char*> input_names = {"token", "prompt_feat", "embedding"};
    std::vector<const char*> output_names = {"mu", "embedding_out", "conds"};
    std::vector<Ort::Value> inputs;
    inputs.push_back(std::move(token_input));
    inputs.push_back(std::move(speech_feature_input));
    inputs.push_back(std::move(speech_embedding_input));

    auto encoder_output = flow_encoder_session->Run(run_options, input_names.data(), inputs.data(), 3, output_names.data(), 3);
    auto mu = encoder_output[0].GetTensorMutableData<float>();
    auto embedding_out = encoder_output[1].GetTensorMutableData<float>();
    auto conds = encoder_output[2].GetTensorMutableData<float>();
    LOGI("[TTS] mu size: %dx%dx%d", encoder_output[0].GetTensorTypeAndShapeInfo().GetShape()[0], encoder_output[0].GetTensorTypeAndShapeInfo().GetShape()[1], encoder_output[0].GetTensorTypeAndShapeInfo().GetShape()[2]);
    LOGI("[TTS] embedding_out size: %dx%d", encoder_output[1].GetTensorTypeAndShapeInfo().GetShape()[0], encoder_output[1].GetTensorTypeAndShapeInfo().GetShape()[1]);
    LOGI("[TTS] conds size: %dx%dx%d", encoder_output[2].GetTensorTypeAndShapeInfo().GetShape()[0], encoder_output[2].GetTensorTypeAndShapeInfo().GetShape()[1], encoder_output[2].GetTensorTypeAndShapeInfo().GetShape()[2]);

    int mel_len1 = speech_features[0].size();
    int mel_len2 = encoder_output[0].GetTensorTypeAndShapeInfo().GetShape()[2] - mel_len1;
    LOGI("[TTS] mel_len1: %d, mel_len2: %d", mel_len1, mel_len2);

    // Flow decoder
    const int n_timesteps = 10;
    int len_mu = encoder_output[0].GetTensorTypeAndShapeInfo().GetElementCount();
    if (random_noise.size() < len_mu) {
        int original_size = random_noise.size();
        random_noise.resize(len_mu);
        std::mt19937 generator(time(nullptr));
        std::normal_distribution<float> distribution(0.0f, 1.0f);
        std::generate(random_noise.begin() + original_size, random_noise.end(), [&]() { return distribution(generator); });
    }
    if (t_span.empty()) {
        t_span.resize(n_timesteps + 1);
        for (int i = 0; i < n_timesteps + 1; i++) {
            t_span[i] = i * 1.0f / n_timesteps;
            // cosine schedule
            t_span[i] = 1 - cos(t_span[i] * 0.5 * M_PI);
        }
    }

    float dt = t_span[1] - t_span[0];
    float t = t_span[0];

    std::vector<int64_t> x_input_shape = {2, 80, encoder_output[0].GetTensorTypeAndShapeInfo().GetShape()[2]};
    std::vector<int64_t> mask_input_shape = {2, 1, encoder_output[0].GetTensorTypeAndShapeInfo().GetShape()[2]};
    std::vector<int64_t> mu_input_shape = {2, 80, encoder_output[0].GetTensorTypeAndShapeInfo().GetShape()[2]};
    std::vector<int64_t> t_input_shape = {2};
    std::vector<int64_t> spks_input_shape = {2, 80};
    std::vector<int64_t> cond_input_shape = {2, 80, encoder_output[0].GetTensorTypeAndShapeInfo().GetShape()[2]};
    Ort::Value x_input = Ort::Value::CreateTensor<float>(allocator, x_input_shape.data(), x_input_shape.size());
    Ort::Value mask_input = Ort::Value::CreateTensor<float>(allocator, mask_input_shape.data(), mask_input_shape.size());
    Ort::Value mu_input = Ort::Value::CreateTensor<float>(allocator, mu_input_shape.data(), mu_input_shape.size());
    Ort::Value t_input = Ort::Value::CreateTensor<float>(allocator, t_input_shape.data(), t_input_shape.size());
    Ort::Value spks_input = Ort::Value::CreateTensor<float>(allocator, spks_input_shape.data(), spks_input_shape.size());
    Ort::Value cond_input = Ort::Value::CreateTensor<float>(allocator, cond_input_shape.data(), cond_input_shape.size());

    for (int i = 0; i < mask_input.GetTensorTypeAndShapeInfo().GetElementCount(); i++) {
        mask_input.GetTensorMutableData<float>()[i] = 1.0f;
    }

    memcpy(mu_input.GetTensorMutableData<float>(), mu, len_mu * sizeof(float));
    memcpy(mu_input.GetTensorMutableData<float>() + len_mu, mu, len_mu * sizeof(float));

    memset(spks_input.GetTensorMutableData<float>(), 0, spks_input.GetTensorTypeAndShapeInfo().GetElementCount() * sizeof(float));

    memcpy(cond_input.GetTensorMutableData<float>(), conds, len_mu * sizeof(float));
    memcpy(cond_input.GetTensorMutableData<float>() + len_mu, conds, len_mu * sizeof(float));

    std::vector<float> x(len_mu);
    memcpy(x.data(), random_noise.data(), len_mu * sizeof(float));

    std::vector<const char*> input_names_estimator = {"x", "mask", "mu", "t", "spks", "cond"};
    std::vector<const char*> output_names_estimator = {"dphi_dt"};

    std::vector<Ort::Value> inputs_estimator;
    inputs_estimator.push_back(std::move(x_input));
    inputs_estimator.push_back(std::move(mask_input));
    inputs_estimator.push_back(std::move(mu_input));
    inputs_estimator.push_back(std::move(t_input));
    inputs_estimator.push_back(std::move(spks_input));
    inputs_estimator.push_back(std::move(cond_input));

    const float inference_cfg_rate = 0.7;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 1; i <= n_timesteps; i++) {
        memcpy(inputs_estimator[0].GetTensorMutableData<float>(), x.data(), len_mu * sizeof(float));
        memcpy(inputs_estimator[0].GetTensorMutableData<float>() + len_mu, x.data(), len_mu * sizeof(float));

        inputs_estimator[3].GetTensorMutableData<float>()[0] = t;
        inputs_estimator[3].GetTensorMutableData<float>()[1] = t;

        auto decoder_output = flow_decoder_estimator_session->Run(run_options, input_names_estimator.data(), inputs_estimator.data(), 6, output_names_estimator.data(), 1);
        auto dphi_dt = decoder_output[0].GetTensorMutableData<float>();
        LOGI("[TTS] dphi_dt size: %dx%dx%d", decoder_output[0].GetTensorTypeAndShapeInfo().GetShape()[0], decoder_output[0].GetTensorTypeAndShapeInfo().GetShape()[1], decoder_output[0].GetTensorTypeAndShapeInfo().GetShape()[2]);

        for (int j = 0; j < len_mu; j++) {
            float dphi_dt_val = (1.0 + inference_cfg_rate) * dphi_dt[j] - inference_cfg_rate * dphi_dt[j + len_mu];
            x[j] += dphi_dt_val * dt;
        }

        t += dt;
        dt = t_span[i + 1] - t;
    }
    auto end = std::chrono::high_resolution_clock::now();
    LOGI("[TTS] flow_decoder_estimator diffusion duration: %lld ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

    // Hift generator
    std::vector<int64_t> speech_feat_shape = {1, 80, mel_len2};
    Ort::Value speech_feat_input = Ort::Value::CreateTensor<float>(allocator, speech_feat_shape.data(), speech_feat_shape.size());
    for (int i = 0; i < 80; i++) {
        memcpy(speech_feat_input.GetTensorMutableData<float>() + i * mel_len2, x.data() + i * (mel_len1 + mel_len2) + mel_len1, mel_len2 * sizeof(float));
    }

    std::vector<const char*> input_names_hift = {"speech_feat"};
    std::vector<const char*> output_names_hift = {"real", "img"};
    std::vector<Ort::Value> inputs_hift;
    inputs_hift.push_back(std::move(speech_feat_input));

    auto hift_output = hift_generator_session->Run(run_options, input_names_hift.data(), inputs_hift.data(), 1, output_names_hift.data(), 2);
    auto real = hift_output[0].GetTensorMutableData<float>();
    auto imag = hift_output[1].GetTensorMutableData<float>();
    LOGI("[TTS] real size: %dx%dx%d", hift_output[0].GetTensorTypeAndShapeInfo().GetShape()[0], hift_output[0].GetTensorTypeAndShapeInfo().GetShape()[1], hift_output[0].GetTensorTypeAndShapeInfo().GetShape()[2]);
    LOGI("[TTS] img size: %dx%dx%d", hift_output[1].GetTensorTypeAndShapeInfo().GetShape()[0], hift_output[1].GetTensorTypeAndShapeInfo().GetShape()[1], hift_output[1].GetTensorTypeAndShapeInfo().GetShape()[2]);
    std::vector<float> real_vector(real, real + hift_output[0].GetTensorTypeAndShapeInfo().GetElementCount());
    std::vector<float> imag_vector(imag, imag + hift_output[1].GetTensorTypeAndShapeInfo().GetElementCount());
    debug_print_mean_std(real_vector, "real_vector");
    debug_print_mean_std(imag_vector, "imag_vector");
    knf::StftResult stft_result = {
        .real = real_vector,
        .imag = imag_vector,
        .num_frames = hift_output[0].GetTensorTypeAndShapeInfo().GetShape()[1]
    };

    int istft_n_fft = 16;
    int istft_hop_length = 4;

    // istft
    knf::StftConfig stft_config;
    stft_config.n_fft = istft_n_fft;
    stft_config.hop_length = istft_hop_length;
    stft_config.window_type = "hann";
    stft_config.win_length = istft_n_fft;
    knf::IStft istft(stft_config);
    std::vector<float> speech_output_istft = istft.Compute(stft_result);
    float max_val = 0.0f;
    for (int i = 0; i < speech_output_istft.size(); i++) {
        max_val = std::max(max_val, std::abs(speech_output_istft[i]));
    }
    for (int i = 0; i < speech_output_istft.size(); i++) {
        speech_output_istft[i] = speech_output_istft[i] / max_val;
    }
    wav_file wav_file;
    wav_file.sample_rate = 24000;
    wav_file.num_channels = 1;
    wav_file.num_samples = speech_output_istft.size();
    wav_file.bit_depth = 16;
    wav_file.audio_format = 1;
    wav_file.byte_rate = 24000 * 16 / 8;
    wav_file.block_align = 2;
    wav_file.samples = speech_output_istft;
    wav_file.save(output_path);

    return true;
}

int frontend::speech_token_sampler(float *logits, size_t size, std::vector<int> decoded_tokens, bool ignore_eos) {
    if (logits == nullptr) {
        return 0;
    }

    int num_trials = 0, max_trials = 100;
    const int eos_token = 6562;
    int token_id = eos_token;
    int top_k = 25;
    float top_p = 0.8;
    float tau_r = 0.1;
    int win_size = 10;
    while (num_trials < max_trials) {
        token_id = _sampler.sample(logits, size, 1.0, top_k, top_p);
        int rep_num = 0;
        int win_size_actual = std::min(win_size, (int)decoded_tokens.size());
        for (int i = 0; i < win_size_actual; i++) {
            if (decoded_tokens[decoded_tokens.size() - win_size_actual + i] == token_id) {
                rep_num++;
            }
        }
        if (rep_num >= win_size * tau_r) {
            token_id = _sampler.sample(logits, size, 1.0, 1, top_p);
        }

        if (!ignore_eos || token_id != eos_token) {
            break;
        }
        num_trials++;
    }
    return token_id;
}

std::string frontend::normalize_text(std::string text) {
    auto replace = [](std::string &text, const std::string &from, const std::string &to) {
        while (text.find(from) != std::string::npos) {
            text.replace(text.find(from), from.length(), to);
        }
    };

    replace(text, "\n", "");

    // remove blank between chinese characters
    // TODO

    replace(text, "²", "平方");
    replace(text, "³", "立方");
    replace(text, "（", "");
    replace(text, "）", "");
    replace(text, "【", "");
    replace(text, "】", "");
    replace(text, "`", "");
    replace(text, "”", "");
    replace(text, "——", " ");

    return text;
}

}
