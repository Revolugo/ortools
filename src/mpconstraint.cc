#include "mpwrappers.h"

MPConstraintWrap::MPConstraintWrap(const Napi::CallbackInfo& info) : Napi::ObjectWrap<MPConstraintWrap>(info)
{ 
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if (info.Length() <= 0 || info[0].Type() != napi_external)
  {
    throw Napi::Error::New(info.Env(), "Invalid call to MPConstraintWrap constructor.");
  }

  this->constraint = info[0].As<Napi::External<MPConstraint>>().Data();
};

Napi::FunctionReference MPConstraintWrap::constructor;

Napi::Object MPConstraintWrap::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "MPConstraint", {
    InstanceMethod("setCoefficient", &MPConstraintWrap::SetCoefficient)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("MPConstraint", func);
  return exports;
}

Napi::Value MPConstraintWrap::SetCoefficient(const Napi::CallbackInfo& info)
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
  this->constraint->SetCoefficient(
    wrapper->variable(),
    info[1].As<Napi::Number>().Uint32Value()
  );
  return info.Env().Undefined();
}

Napi::Object MPConstraintWrap::NewInstance(Napi::Value arg) {
  Napi::Object obj = constructor.New({ arg });
  return obj;
}
