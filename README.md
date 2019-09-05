# @revolugo/ortools
[![Build Status](https://travis-ci.com/Revolugo/ortools.svg?branch=master)](https://travis-ci.com/Revolugo/ortools) [![Dependency Status](https://img.shields.io/david/Revolugo/ortools.svg)](https://david-dm.org/Revolugo/ortools) [![MIT Licensed](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

A node.js wrapper for Google OR-Tools.

Current implementation only wraps integer programming necessary classes and methods. It exposes a Promise interface for the `solver.solve()`method.

## Supported features

The following OR-Tools C++ Library values, classes and methods are supported.

**note** : corresponding javascript class methods are **camelCase-d** (e.g. `makeIntVar()`)

### MPSolver

**Supported optimization problem type**:   
MPSolver::CBC_MIXED_INTEGER_PROGRAMMING

**Methods**:  
MPSolver::Name()  
MPSolver::NumVariables()  
MPSolver::NumConstraints()  
MPSolver::MakeIntVar()  
MPSolver::MakeRowConstraint()  
MPSolver::MakeMutableObjective()  
MPSolver::Solve()  
MPSolver::infinity()  
MPSolver::wall_time()  
MPSolver::iterations()  
MPSolver::nodes()  

### MPVariable

**Methods**:  
MPVariable::Name()  
MPVariable::solution_value()  

### MPConstraint

**Methods**:  
MPConstraint::SetCoefficient()  

### MPObjective

**Methods**:  
MPObjective::SetCoefficient()  
MPObjective::SetMaximization()  
MPObjective::SetMinimization()  
MPObjective::Value()  

## Installation

```bash
npm install @revolugo/ortools --save
```

**note**: the package includes a script that detects your platform (OS/version) and downloads the right binaries and include files from Google OR-Tools repository releases. Building the module requires standard C/C++ build tools and zlib. Supported platforms are currently:

- Ubuntu 16.04, 18.04
- CentOS 7
- Debian 9
- Mac OS X

## Usage

```javascript
const ortools = require('@revolugo/ortools')

// Create the linear solver with the CBC backend.
const solver = new ortools.MPSolver('My solver', ortools.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)

// Create the variables x and y.
const x = solver.makeIntVar(0, solver.infinity(), 'x')
const y = solver.makeIntVar(0, solver.infinity(), 'y')

// Create a linear constraint, x + 7y <= 17.5.
const c0 = solver.makeRowConstraint(-solver.infinity(), 17.5)
c0.setCoefficient(x, 1)
c0.setCoefficient(y, 7)

// Create another linear constraint, x <= 3.5.
const c1 = solver.makeRowConstraint(-solver.infinity(), 3.5)
c1.setCoefficient(x, 1)
c1.setCoefficient(y, 0)

// Create the objective function, x + 10y.
const objective = solver.mutableObjective()
objective.setCoefficient(x, 1)
objective.setCoefficient(y, 10)
// We want the objective to be at a maximum value
objective.setMaximization()

// Try to find the best solution using solve()
// Returns mapped MPSolver::ResultStatus enum value
//    ortools.MPSolver.RESULT_OPTIMAL
//    ortools.MPSolver.RESULT_FEASIBLE
//    ortools.MPSolver.RESULT_INFEASIBLE
//    ortools.MPSolver.RESULT_UNBOUNDED
//    ortools.MPSolver.RESULT_ABNORMAL
//    ortools.MPSolver.RESULT_NOT_SOLVED
//    ortools.MPSolver.RESULT_MODEL_INVALID
const result = await solver.solve()
if (result === ortools.MPSolver.RESULT_OPTIMAL) {
  console.log('Solution:')
  console.log(`x = ${ x.solution_value() }`)
  console.log(`y = ${ y.solution_value() }`)
  console.log(`objective = ${ objective.value() }`)
}
else {
  console.log(`Non optimal result ${ result }!`)
}

```









