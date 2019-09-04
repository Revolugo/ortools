#include "promiseworker.h"

PromiseWorker::PromiseWorker(Napi::Promise::Deferred const& d) : AsyncWorker(this->get_fake_callback(d.Env()).Value()), deferred(d) {}

void PromiseWorker::OnOK()
{
  this->Resolve(this->deferred);
}

void PromiseWorker::OnError(Napi::Error const& error)
{
  this->deferred.Reject(error.Value());
}

Napi::Value PromiseWorker::noop(Napi::CallbackInfo const& info)
{
  return info.Env().Undefined();
}

Napi::Reference<Napi::Function> const& PromiseWorker::get_fake_callback(Napi::Env const& env)
{
  static Napi::Reference<Napi::Function> fcall;
  fcall = Napi::Reference<Napi::Function>::New(Napi::Function::New(env, this->noop), 1);
  return fcall;
}
