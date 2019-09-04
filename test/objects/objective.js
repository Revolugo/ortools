const addon = require('../../')
const Faker = require('faker')

describe('MPObjective creation', async () => {
  it(`Solver should mutableObjective() returning valid MPObjective`, async () => {
    const solver = new addon.MPSolver('', addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    const objective = solver.mutableObjective()
    expect(objective).to.be.a('MPObjective')
  })
})
describe('MPObjective methods sanity', async () => {
  it(`Solver MPObjective setCoefficient() sanity check`, async () => {
    const solver = new addon.MPSolver('', addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    const x = solver.makeIntVar(0, 1, '')
    const objective = solver.mutableObjective()
    expect(() => objective.setCoefficient()).to.throw()
    expect(() => objective.setCoefficient(1)).to.throw()
    expect(() => objective.setCoefficient({ }, 1)).to.throw(TypeError)
    expect(() => objective.setCoefficient(null, null)).to.throw(TypeError)
    expect(() => objective.setCoefficient(23, null)).to.throw(TypeError)
    expect(() => objective.setCoefficient(23, 'null')).to.throw(TypeError)
    expect(() => objective.setCoefficient('null', 'null')).to.throw(TypeError)
    expect(() => objective.setCoefficient('null', 3)).to.throw(TypeError)
    expect(() => objective.setCoefficient('null', 3)).to.throw(TypeError)
    expect(() => objective.setCoefficient(x, 3)).to.not.throw()
  })
  it(`Solver MPObjective setMinimization() sanity check`, async () => {
    const solver = new addon.MPSolver('', addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    const objective = solver.mutableObjective()
    expect(() => objective.setMinimization()).to.not.throw()
  })
  it(`Solver MPObjective setMaximization() sanity check`, async () => {
    const solver = new addon.MPSolver('', addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    const objective = solver.mutableObjective()
    expect(() => objective.setMaximization()).to.not.throw()
  })
  it(`Solver MPObjective value() sanity check`, async () => {
    const solver = new addon.MPSolver('', addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    const objective = solver.mutableObjective()
    expect(objective.value()).to.be.a('number')
  })
})
