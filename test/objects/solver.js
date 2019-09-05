const addon = require('../../')
const Faker = require('faker')

describe('MPSolver creation', async () => {
  it(`Should create valid MPSolver object`, async () => {
    const solver = new addon.MPSolver('This is my name.', addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    expect(solver).to.be.a('MPSolver')
  })
})
describe('MPSolver methods sanity', async () => {
  it(`check MPSolver static values`, async () => {
    expect(addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING).to.be.a('number')
    expect(addon.MPSolver.RESULT_OPTIMAL).to.be.a('number')
    expect(addon.MPSolver.RESULT_FEASIBLE).to.be.a('number')
    expect(addon.MPSolver.RESULT_INFEASIBLE).to.be.a('number')
    expect(addon.MPSolver.RESULT_UNBOUNDED).to.be.a('number')
    expect(addon.MPSolver.RESULT_ABNORMAL).to.be.a('number')
    expect(addon.MPSolver.RESULT_NOT_SOLVED).to.be.a('number')
    expect(addon.MPSolver.RESULT_MODEL_INVALID).to.be.a('number')
  })
  it(`MPSolver sanity check`, async () => {
    expect(() => { new addon.MPSolver(0, addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING) }).to.throw(TypeError)
    expect(() => { new addon.MPSolver('fail', 8999) }).to.throw()
  })
  it(`MPSolver name() sanity check`, async () => {
    const name = Faker.lorem.word()
    const solver = new addon.MPSolver(name, addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    expect(solver.name()).to.equal(name)
  })
  it(`Should get infinity() returning valid Infinity number`, async () => {
    const solver = new addon.MPSolver('', addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    const infinity = solver.infinity()
    expect(infinity).to.be.a('number')
    expect(infinity).to.equal(Infinity)
  })
  it(`Should get wall_time() returning valid number`, async () => {
    const solver = new addon.MPSolver('', addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    expect(solver.wall_time()).to.be.a('number')
  })
  it(`Should get iterations() returning valid number`, async () => {
    const solver = new addon.MPSolver('', addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    expect(solver.iterations()).to.be.a('number')
  })
  it(`Should get nodes() returning valid number`, async () => {
    const solver = new addon.MPSolver('', addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    expect(solver.nodes()).to.be.a('number')
  })
  it(`MPSolver makeIntVar sanity check`, async () => {
    const solver = new addon.MPSolver('', addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    expect(() => solver.makeIntVar('fail', 0, 'x')).to.throw(TypeError)
    expect(() => solver.makeIntVar('fail', 'fail', 'x')).to.throw(TypeError)
    expect(() => solver.makeIntVar('fail', 'fail', 8)).to.throw(TypeError)
    expect(() => solver.makeIntVar({ }, 0, 'x')).to.throw(TypeError)
  })
  it(`MPSolver makeRowConstraint sanity check`, async () => {
    const solver = new addon.MPSolver('', addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    expect(() => solver.makeRowConstraint('fail', 0)).to.throw(TypeError)
    expect(() => solver.makeRowConstraint('fail', 'fail')).to.throw(TypeError)
    expect(() => solver.makeRowConstraint({ }, 0)).to.throw(TypeError)
  })
  it(`MPSolver numVariables sanity check`, async () => {
    const solver = new addon.MPSolver('', addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    const num = Faker.random.number({ max: 10 })
    for (let i = 0; i < num; i++) {
      expect(solver.makeIntVar(-Infinity, Infinity, `var#${i}`)).to.be.a('MPVariable')
    }
    expect(solver.numVariables()).to.equal(num)
  })
  it(`MPSolver numConstraints sanity check`, async () => {
    const solver = new addon.MPSolver('', addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    const num = Faker.random.number({ max: 10 })
    for (let i = 0; i < num; i++) {
      expect(solver.makeRowConstraint(-Infinity, Infinity)).to.be.a('MPConstraint')
    }
    expect(solver.numConstraints()).to.equal(num)
  })
})
describe('MPSolver solve', async () => {
  it(`Should asynchronously solve MP problem`, async () => {
    const solver = new addon.MPSolver('My solver', addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    const x = solver.makeIntVar(0, solver.infinity(), 'x')
    const y = solver.makeIntVar(0, solver.infinity(), 'y')
    const c0 = solver.makeRowConstraint(-solver.infinity(), 17.5)
    c0.setCoefficient(x, 1)
    c0.setCoefficient(y, 7)
    const c1 = solver.makeRowConstraint(-solver.infinity(), 3.5)
    c1.setCoefficient(x, 1)
    c1.setCoefficient(y, 0)
    const objective = solver.mutableObjective()
    objective.setCoefficient(x, 1)
    objective.setCoefficient(y, 10)
    objective.setMaximization()
    const promise = solver.solve()
    return expect(promise).to.eventually.equal(addon.MPSolver.RESULT_OPTIMAL).then(() => {
      expect(x.solution_value()).to.equal(3),
      expect(y.solution_value()).to.equal(2),
      expect(objective.value()).to.equal(23)
    })
  })
})