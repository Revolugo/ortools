#ifndef __PROMISEWORKER_H__
#define __PROMISEWORKER_H__

#include <napi.h>

class PromiseWorker : public Napi::AsyncWorker
{
  public:
    PromiseWorker(Napi::Promise::Deferred const& deferred);
    virtual void Resolve(Napi::Promise::Deferred const& deferred) = 0;
    void OnOK() override;
    void OnError(Napi::Error const& error) override;
  private:
    static Napi::Value noop(Napi::CallbackInfo const& info);
    Napi::Reference<Napi::Function> const& get_fake_callback(Napi::Env const& env);
    Napi::Promise::Deferred deferred;
};

#endif
