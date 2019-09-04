#include "mpwrappers.h"

MPObjectiveWrap::MPObjectiveWrap(const Napi::CallbackInfo& info) : Napi::ObjectWrap<MPObjectiveWrap>(info)
{ 
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if (info.Length() <= 0 || info[0].Type() != napi_external)
  {
    throw Napi::Error::New(info.Env(), "Invalid call to MPObjectiveWrap constructor.");
  }

  this->objective = info[0].As<Napi::External<MPObjective>>().Data();
};

Napi::FunctionReference MPObjectiveWrap::constructor;

Napi::Object MPObjectiveWrap::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "MPObjective", {
    InstanceMethod("setCoefficient", &MPObjectiveWrap::SetCoefficient),
    InstanceMethod("setMinimization", &MPObjectiveWrap::SetMinimization),
    InstanceMethod("setMaximization", &MPObjectiveWrap::SetMaximization),
    InstanceMethod("value", &MPObjectiveWrap::Value)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("MPObjective", func);
  return exports;
}

Napi::Value MPObjectiveWrap::SetMinimization(const Napi::CallbackInfo& info)
{
  this->objective->SetMinimization();
  return info.Env().Undefined();
}

Napi::Value MPObjectiveWrap::SetMaximization(const Napi::CallbackInfo& info)
{
  this->objective->SetMaximization();
  return info.Env().Undefined();
}

Napi::Value MPObjectiveWrap::Value(const Napi::CallbackInfo& info)
{
  return  Napi::Number::New(info.Env(), this->objective->Value());
}

Napi::Value MPObjectiveWrap::SetCoefficient(const Napi::CallbackInfo& info)
{
  if (info.Length() < 2) {
    THROW_FUNC_ERR(info, "Invalid call to setCoefficient, 2 arguments required.");
  }
  if (!info[0].IsObject()) {
    THROW_FUNC_TYPE_ERR(info, "First parameter should be a MPVariable.");
  }
  if (!info[1].IsNumber()) {
    THROW_FUNC_TYPE_ERR(info, "Second parameter should be a Number.");
  }
  MPVariableWrap* wrapper;
  try {
    wrapper = MPVariableWrap::Unwrap(info[0].As<Napi::Object>());
  }
  catch(const std::exception& e) {
    THROW_FUNC_TYPE_ERR(info, "First parameter should be a MPVariable.");
  }
  this->objective->SetCoefficient(
    wrapper->variable(),
    info[1].As<Napi::Number>().DoubleValue()
  );
  return info.Env().Undefined();
}

Napi::Object MPObjectiveWrap::NewInstance(Napi::Value arg) {
  Napi::Object obj = constructor.New({ arg });
  return obj;
}
