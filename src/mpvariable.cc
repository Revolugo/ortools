#include "mpwrappers.h"

MPVariableWrap::MPVariableWrap(const Napi::CallbackInfo& info) : Napi::ObjectWrap<MPVariableWrap>(info)
{ 
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if (info.Length() <= 0 || info[0].Type() != napi_external)
  {
    throw Napi::Error::New(info.Env(), "Invalid call to MPVariableWrap constructor.");
  }
  this->var = info[0].As<Napi::External<MPVariable>>().Data();
};

Napi::FunctionReference MPVariableWrap::constructor;

Napi::Object MPVariableWrap::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "MPVariable", {
    InstanceMethod("name", &MPVariableWrap::Name),
    InstanceMethod("solution_value", &MPVariableWrap::SolutionValue)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("MPVariable", func);
  return exports;
}

Napi::Value MPVariableWrap::SolutionValue(const Napi::CallbackInfo& info) {
  return  Napi::Number::New(info.Env(), this->var->solution_value());
}

Napi::Value MPVariableWrap::Name(const Napi::CallbackInfo& info) {
  return  Napi::String::New(info.Env(), this->var->name());
}

Napi::Object MPVariableWrap::NewInstance(Napi::Value arg) {
  Napi::Object obj = constructor.New({ arg });
  return obj;
}
