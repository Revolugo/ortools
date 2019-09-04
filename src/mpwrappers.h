#ifndef __MPWRAPPER_H__
#define __MPWRAPPER_H__

#include <napi.h>

#ifndef USE_CBC
#define USE_CBC
#endif

#include "ortools/linear_solver/linear_solver.h"

using namespace operations_research;

#define THROW_TYPE_ERR(info, msg) \
  Napi::TypeError::New(info.Env(), msg).ThrowAsJavaScriptException()

#define THROW_ERR(info, msg) \
  Napi::Error::New(info.Env(), msg).ThrowAsJavaScriptException()

#define THROW_FUNC_ERR(info, msg) \
  THROW_ERR(info, msg); \
  return info.Env().Undefined()

#define THROW_FUNC_TYPE_ERR(info, msg) \
  THROW_TYPE_ERR(info, msg); \
  return info.Env().Undefined()

class MPVariableWrap : public Napi::ObjectWrap<MPVariableWrap> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Value arg);
  MPVariableWrap(const Napi::CallbackInfo& info);
  MPVariable* variable() { return this->var; };

 private:
  static Napi::FunctionReference constructor;

  Napi::Value Name(const Napi::CallbackInfo& info);
  Napi::Value SolutionValue(const Napi::CallbackInfo& info);

  MPVariable* var;
};

class MPConstraintWrap : public Napi::ObjectWrap<MPConstraintWrap> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Value arg);
  MPConstraintWrap(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;

  Napi::Value SetCoefficient(const Napi::CallbackInfo& info);

  MPConstraint* constraint;
};

class MPObjectiveWrap : public Napi::ObjectWrap<MPObjectiveWrap> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Value arg);
  MPObjectiveWrap(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;

  Napi::Value SetCoefficient(const Napi::CallbackInfo& info);
  Napi::Value SetMaximization(const Napi::CallbackInfo& info);
  Napi::Value SetMinimization(const Napi::CallbackInfo& info);
  Napi::Value Value(const Napi::CallbackInfo& info);

  MPObjective* objective;
};

class MPSolverWrap : public Napi::ObjectWrap<MPSolverWrap> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  MPSolverWrap(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;

  Napi::Value MutableObjective(const Napi::CallbackInfo& info);
  Napi::Value MakeRowConstraint(const Napi::CallbackInfo& info);
  Napi::Value MakeIntVar(const Napi::CallbackInfo& info);
  Napi::Value Solve(const Napi::CallbackInfo& info);
  Napi::Value Infinity(const Napi::CallbackInfo& info);
  Napi::Value Name(const Napi::CallbackInfo& info);
  Napi::Value NumVariables(const Napi::CallbackInfo& info);
  Napi::Value NumConstraints(const Napi::CallbackInfo& info);
  Napi::Value WallTime(const Napi::CallbackInfo& info);
  Napi::Value Iterations(const Napi::CallbackInfo& info);
  Napi::Value Nodes(const Napi::CallbackInfo& info);

  MPSolver* solver;
  Napi::Promise::Deferred* deferred;
};

#endif  // __MPWRAPPER_H__