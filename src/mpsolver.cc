#include "mpwrappers.h"
#include "promiseworker.h"

using namespace std;

static const vector<int> SUPPORTED_SOLVERS = {
  static_cast<int>(MPSolver::CBC_MIXED_INTEGER_PROGRAMMING)
};

static bool is_supported_solver(int solver)
{
  for (auto s : SUPPORTED_SOLVERS) {
    if (solver == s) {
      return true;
    }
  }
  return false;
}

class SolverWorker : public PromiseWorker
{
  public:
    SolverWorker(MPSolver* solver, Napi::Promise::Deferred const& d): PromiseWorker(d), solver(solver) {}
    ~SolverWorker() {}
    void Resolve(Napi::Promise::Deferred const& deferred) override {
      deferred.Resolve(Napi::Number::New(this->Env(), this->status));
    }
    void Execute() {
      this->status = this->solver->Solve();
    }
  private:
    MPSolver* solver;
    MPSolver::ResultStatus status;
};

Napi::FunctionReference MPSolverWrap::constructor;

Napi::Object MPSolverWrap::Init(Napi::Env env, Napi::Object exports)
{
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "MPSolver", {
    InstanceMethod("infinity", &MPSolverWrap::Infinity),
    InstanceMethod("makeIntVar", &MPSolverWrap::MakeIntVar),
    InstanceMethod("mutableObjective", &MPSolverWrap::MutableObjective),
    InstanceMethod("makeRowConstraint", &MPSolverWrap::MakeRowConstraint),
    InstanceMethod("solve", &MPSolverWrap::Solve),
    InstanceMethod("name", &MPSolverWrap::Name),
    InstanceMethod("numVariables", &MPSolverWrap::NumVariables),
    InstanceMethod("numConstraints", &MPSolverWrap::NumConstraints),
    InstanceMethod("wall_time", &MPSolverWrap::WallTime),
    InstanceMethod("iterations", &MPSolverWrap::Iterations),
    InstanceMethod("nodes", &MPSolverWrap::Nodes),
    StaticValue("CBC_MIXED_INTEGER_PROGRAMMING", Napi::Number::New(env, MPSolver::CBC_MIXED_INTEGER_PROGRAMMING))
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("MPSolver", func);
  return exports;
}

MPSolverWrap::MPSolverWrap(const Napi::CallbackInfo& info) : Napi::ObjectWrap<MPSolverWrap>(info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if (info.Length() < 2) {
    THROW_ERR(info, "Invalid call to MPSolver constructor, 2 arguments required.");
    return;
  }
  if (!info[0].IsString()) {
    THROW_TYPE_ERR(info, "First parameter should be a string.");
    return;
  }
  if (!info[1].IsNumber()) {
    THROW_TYPE_ERR(info, "Second parameter should be a number.");
    return;
  }
  int s = info[1].As<Napi::Number>().Int32Value();
  if (!is_supported_solver(s)) {
    THROW_ERR(info, "Unsupported solver.");
    return;
  }  
  string name = info[0].As<Napi::String>().Utf8Value();
  this->solver = new MPSolver(name, static_cast<MPSolver::OptimizationProblemType>(s));
  this->deferred = NULL;
}

Napi::Value MPSolverWrap::Infinity(const Napi::CallbackInfo& info)
{
  return Napi::Number::New(info.Env(), this->solver->infinity());
}

Napi::Value MPSolverWrap::MakeRowConstraint(const Napi::CallbackInfo& info)
{
  if (info.Length() < 2) {
    THROW_FUNC_ERR(info, "Invalid call to makeRowConstraint, 2 arguments required.");
  }
  if (!info[0].IsNumber()) {
    THROW_FUNC_TYPE_ERR(info, "First parameter should be a Number.");
  }
  if (!info[1].IsNumber()) {
    THROW_FUNC_TYPE_ERR(info, "Second parameter should be a Number.");
  }
  MPConstraint* constraint = this->solver->MakeRowConstraint(
      info[0].As<Napi::Number>().DoubleValue(),
      info[1].As<Napi::Number>().DoubleValue()
    );
  auto asExternalVar = Napi::External<MPConstraint>::New(info.Env(), constraint);
  return MPConstraintWrap::NewInstance(asExternalVar);
}

Napi::Value MPSolverWrap::MutableObjective(const Napi::CallbackInfo& info)
{
  MPObjective* objective = this->solver->MutableObjective();
  auto asExternalVar = Napi::External<MPObjective>::New(info.Env(), objective);
  return MPObjectiveWrap::NewInstance(asExternalVar);
}

Napi::Value MPSolverWrap::MakeIntVar(const Napi::CallbackInfo& info)
{
  if (info.Length() < 3) {
    THROW_FUNC_ERR(info, "Invalid call to makeIntVar, 3 arguments required.");
  }
  if (!info[0].IsNumber()) {
    THROW_FUNC_TYPE_ERR(info, "First parameter should be a Number.");
  }
  if (!info[1].IsNumber()) {
    THROW_FUNC_TYPE_ERR(info, "Second parameter should be a Number.");
  }
  if (!info[2].IsString()) {
    THROW_FUNC_TYPE_ERR(info, "Third parameter should be a String.");
  }
  MPVariable* var = this->solver->MakeIntVar(
      info[0].As<Napi::Number>().DoubleValue(),
      info[1].As<Napi::Number>().DoubleValue(),
      info[2].As<Napi::String>().Utf8Value()
    );
  auto asExternalVar = Napi::External<MPVariable>::New(info.Env(), var);
  return MPVariableWrap::NewInstance(asExternalVar);
}

Napi::Value MPSolverWrap::Solve(const Napi::CallbackInfo& info) {
  this->deferred = new Napi::Promise::Deferred(info.Env());
  SolverWorker* worker = new SolverWorker(this->solver, *this->deferred);
  worker->Queue();
  return this->deferred->Promise();
}

Napi::Value MPSolverWrap::Name(const Napi::CallbackInfo& info) {
  return  Napi::String::New(info.Env(), this->solver->Name());
}

Napi::Value MPSolverWrap::NumVariables(const Napi::CallbackInfo& info) {
  return  Napi::Number::New(info.Env(), this->solver->NumVariables());
}

Napi::Value MPSolverWrap::NumConstraints(const Napi::CallbackInfo& info) {
  return  Napi::Number::New(info.Env(), this->solver->NumConstraints());
}

Napi::Value MPSolverWrap::WallTime(const Napi::CallbackInfo& info) {
  return  Napi::Number::New(info.Env(), this->solver->wall_time());
}

Napi::Value MPSolverWrap::Iterations(const Napi::CallbackInfo& info) {
  return  Napi::Number::New(info.Env(), this->solver->iterations());
}

Napi::Value MPSolverWrap::Nodes(const Napi::CallbackInfo& info) {
  return  Napi::Number::New(info.Env(), this->solver->nodes());
}
