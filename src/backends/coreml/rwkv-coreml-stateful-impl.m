//
// rwkv_coreml_stateful_impl.m
//
// This file was automatically generated and should not be edited.
//

#if !__has_feature(objc_arc)
#error This file must be compiled with automatic reference counting enabled (-fobjc-arc)
#endif

#import "rwkv-coreml-stateful-impl.h"

@implementation rwkv_coreml_stateful_implInput

- (instancetype)initWithIn0:(MLMultiArray *)in0 {
    self = [super init];
    if (self) {
        _in0 = in0;
    }
    return self;
}

- (NSSet<NSString *> *)featureNames {
    return [NSSet setWithArray:@[@"in0"]];
}

- (nullable MLFeatureValue *)featureValueForName:(NSString *)featureName {
    if ([featureName isEqualToString:@"in0"]) {
        return [MLFeatureValue featureValueWithMultiArray:self.in0];
    }
    return nil;
}

@end

@implementation rwkv_coreml_stateful_implOutput

- (instancetype)initWithLogits:(MLMultiArray *)logits {
    self = [super init];
    if (self) {
        _logits = logits;
    }
    return self;
}

- (NSSet<NSString *> *)featureNames {
    return [NSSet setWithArray:@[@"logits"]];
}

- (nullable MLFeatureValue *)featureValueForName:(NSString *)featureName {
    if ([featureName isEqualToString:@"logits"]) {
        return [MLFeatureValue featureValueWithMultiArray:self.logits];
    }
    return nil;
}

@end

@implementation rwkv_coreml_stateful_implState

- (instancetype)initWithState:(MLState *)state {
    self = [super init];
    if (self) {
        _state = state;
    }
    return self;
}

- (void)getMultiArrayForState:(rwkv_coreml_stateful_implStateName)stateName
                      handler:(void (NS_NOESCAPE ^)(MLMultiArray *buffer))handler {
    NSString *stateNameString = nil;
    switch (stateName) {
        case rwkv_coreml_stateful_implStateNameState_tokenshift:
            stateNameString = @"state_tokenshift";
            break;
        case rwkv_coreml_stateful_implStateNameState_wkv:
            stateNameString = @"state_wkv";
            break;
    }

    [self.state getMultiArrayForStateNamed:stateNameString handler:handler];
}

@end

@implementation rwkv_coreml_stateful_impl


/**
    URL of the underlying .mlmodelc directory.
*/
+ (nullable NSURL *)URLOfModelInThisBundle {
    NSString *assetPath = [[NSBundle bundleForClass:[self class]] pathForResource:@"rwkv_coreml_stateful_impl" ofType:@"mlmodelc"];
    if (nil == assetPath) { os_log_error(OS_LOG_DEFAULT, "Could not load rwkv_coreml_stateful_impl.mlmodelc in the bundle resource"); return nil; }
    return [NSURL fileURLWithPath:assetPath];
}


/**
    Initialize rwkv_coreml_stateful_impl instance from an existing MLModel object.

    Usually the application does not use this initializer unless it makes a subclass of rwkv_coreml_stateful_impl.
    Such application may want to use `-[MLModel initWithContentsOfURL:configuration:error:]` and `+URLOfModelInThisBundle` to create a MLModel object to pass-in.
*/
- (instancetype)initWithMLModel:(MLModel *)model {
    if (model == nil) {
        return nil;
    }
    self = [super init];
    if (self != nil) {
        _model = model;
    }
    return self;
}


/**
    Initialize rwkv_coreml_stateful_impl instance with the model in this bundle.
*/
- (nullable instancetype)init {
    return [self initWithContentsOfURL:(NSURL * _Nonnull)self.class.URLOfModelInThisBundle error:nil];
}


/**
    Initialize rwkv_coreml_stateful_impl instance with the model in this bundle.

    @param configuration The model configuration object
    @param error If an error occurs, upon return contains an NSError object that describes the problem. If you are not interested in possible errors, pass in NULL.
*/
- (nullable instancetype)initWithConfiguration:(MLModelConfiguration *)configuration error:(NSError * _Nullable __autoreleasing * _Nullable)error {
    return [self initWithContentsOfURL:(NSURL * _Nonnull)self.class.URLOfModelInThisBundle configuration:configuration error:error];
}


/**
    Initialize rwkv_coreml_stateful_impl instance from the model URL.

    @param modelURL URL to the .mlmodelc directory for rwkv_coreml_stateful_impl.
    @param error If an error occurs, upon return contains an NSError object that describes the problem. If you are not interested in possible errors, pass in NULL.
*/
- (nullable instancetype)initWithContentsOfURL:(NSURL *)modelURL error:(NSError * _Nullable __autoreleasing * _Nullable)error {
    MLModel *model = [MLModel modelWithContentsOfURL:modelURL error:error];
    if (model == nil) { return nil; }
    return [self initWithMLModel:model];
}


/**
    Initialize rwkv_coreml_stateful_impl instance from the model URL.

    @param modelURL URL to the .mlmodelc directory for rwkv_coreml_stateful_impl.
    @param configuration The model configuration object
    @param error If an error occurs, upon return contains an NSError object that describes the problem. If you are not interested in possible errors, pass in NULL.
*/
- (nullable instancetype)initWithContentsOfURL:(NSURL *)modelURL configuration:(MLModelConfiguration *)configuration error:(NSError * _Nullable __autoreleasing * _Nullable)error {
    MLModel *model = [MLModel modelWithContentsOfURL:modelURL configuration:configuration error:error];
    if (model == nil) { return nil; }
    return [self initWithMLModel:model];
}


/**
    Construct rwkv_coreml_stateful_impl instance asynchronously with configuration.
    Model loading may take time when the model content is not immediately available (e.g. encrypted model). Use this factory method especially when the caller is on the main thread.

    @param configuration The model configuration
    @param handler When the model load completes successfully or unsuccessfully, the completion handler is invoked with a valid rwkv_coreml_stateful_impl instance or NSError object.
*/
+ (void)loadWithConfiguration:(MLModelConfiguration *)configuration completionHandler:(void (^)(rwkv_coreml_stateful_impl * _Nullable model, NSError * _Nullable error))handler {
    [self loadContentsOfURL:(NSURL * _Nonnull)[self URLOfModelInThisBundle]
              configuration:configuration
          completionHandler:handler];
}


/**
    Construct rwkv_coreml_stateful_impl instance asynchronously with URL of .mlmodelc directory and optional configuration.

    Model loading may take time when the model content is not immediately available (e.g. encrypted model). Use this factory method especially when the caller is on the main thread.

    @param modelURL The model URL.
    @param configuration The model configuration
    @param handler When the model load completes successfully or unsuccessfully, the completion handler is invoked with a valid rwkv_coreml_stateful_impl instance or NSError object.
*/
+ (void)loadContentsOfURL:(NSURL *)modelURL configuration:(MLModelConfiguration *)configuration completionHandler:(void (^)(rwkv_coreml_stateful_impl * _Nullable model, NSError * _Nullable error))handler {
    [MLModel loadContentsOfURL:modelURL
                 configuration:configuration
             completionHandler:^(MLModel *model, NSError *error) {
        if (model != nil) {
            rwkv_coreml_stateful_impl *typedModel = [[rwkv_coreml_stateful_impl alloc] initWithMLModel:model];
            handler(typedModel, nil);
        } else {
            handler(nil, error);
        }
    }];
}


/**
    Make a new state.
*/
- (rwkv_coreml_stateful_implState *)newState {
    MLState *state = [self.model newState];
    return [[rwkv_coreml_stateful_implState alloc] initWithState:state];
}

- (nullable rwkv_coreml_stateful_implOutput *)predictionFromFeatures:(rwkv_coreml_stateful_implInput *)input usingState:(rwkv_coreml_stateful_implState *)state error:(NSError * _Nullable __autoreleasing * _Nullable)error {
    return [self predictionFromFeatures:input usingState:state options:[[MLPredictionOptions alloc] init] error:error];
}

- (nullable rwkv_coreml_stateful_implOutput *)predictionFromFeatures:(rwkv_coreml_stateful_implInput *)input usingState:(rwkv_coreml_stateful_implState *)state options:(MLPredictionOptions *)options error:(NSError * _Nullable __autoreleasing * _Nullable)error {
    id<MLFeatureProvider> outFeatures = [self.model predictionFromFeatures:input usingState:state.state options:options error:error];
    if (!outFeatures) { return nil; }
    return [[rwkv_coreml_stateful_implOutput alloc] initWithLogits:(MLMultiArray *)[outFeatures featureValueForName:@"logits"].multiArrayValue];
}

- (void)predictionFromFeatures:(rwkv_coreml_stateful_implInput *)input usingState:(rwkv_coreml_stateful_implState *)state completionHandler:(void (^)(rwkv_coreml_stateful_implOutput * _Nullable output, NSError * _Nullable error))completionHandler {
    [self.model predictionFromFeatures:input usingState:state.state options:[[MLPredictionOptions alloc] init] completionHandler:^(id<MLFeatureProvider> prediction, NSError *predictionError) {
        if (prediction != nil) {
            rwkv_coreml_stateful_implOutput *output = [[rwkv_coreml_stateful_implOutput alloc] initWithLogits:(MLMultiArray *)[prediction featureValueForName:@"logits"].multiArrayValue];
            completionHandler(output, predictionError);
        } else {
            completionHandler(nil, predictionError);
        }
    }];
}

- (void)predictionFromFeatures:(rwkv_coreml_stateful_implInput *)input usingState:(rwkv_coreml_stateful_implState *)state options:(MLPredictionOptions *)options completionHandler:(void (^)(rwkv_coreml_stateful_implOutput * _Nullable output, NSError * _Nullable error))completionHandler {
    [self.model predictionFromFeatures:input usingState:state.state options:options completionHandler:^(id<MLFeatureProvider> prediction, NSError *predictionError) {
        if (prediction != nil) {
            rwkv_coreml_stateful_implOutput *output = [[rwkv_coreml_stateful_implOutput alloc] initWithLogits:(MLMultiArray *)[prediction featureValueForName:@"logits"].multiArrayValue];
            completionHandler(output, predictionError);
        } else {
            completionHandler(nil, predictionError);
        }
    }];
}

- (nullable rwkv_coreml_stateful_implOutput *)predictionFromIn0:(MLMultiArray *)in0 usingState:(rwkv_coreml_stateful_implState *)state error:(NSError * _Nullable __autoreleasing * _Nullable)error {
    rwkv_coreml_stateful_implInput *input_ = [[rwkv_coreml_stateful_implInput alloc] initWithIn0:in0];
    return [self predictionFromFeatures:input_ usingState:state error:error];
}

@end
