const ortools = require('..')

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
;(async () => {
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
})();
