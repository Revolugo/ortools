#include <napi.h>

#include "mpwrappers.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  
  google::InitGoogleLogging("");
  
  MPVariableWrap::Init(env, exports);
  MPConstraintWrap::Init(env, exports);
  MPObjectiveWrap::Init(env, exports);
  MPSolverWrap::Init(env, exports);

  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)