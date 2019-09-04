const addon = require('../../')
const Faker = require('faker')

describe('MPVariable creation', async () => {
  it(`Solver should makeIntVar() returning valid MPVariable`, async () => {
    const solver = new addon.MPSolver('', addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    const x = solver.makeIntVar(0, 5, 'x')
    expect(x).to.be.a('MPVariable')
  })
})
describe('MPVariable methods sanity', async () => {
  it(`Should get name() returning valid name`, async () => {
    const name = Faker.lorem.word()
    const solver = new addon.MPSolver('', addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    const x = solver.makeIntVar(0, 5, name)
    expect(x.name()).to.equal(name)
  })
  it(`Should get solution_value() returning valid number`, async () => {
    const solver = new addon.MPSolver('', addon.MPSolver.CBC_MIXED_INTEGER_PROGRAMMING)
    const x = solver.makeIntVar(0, 5, 'x')
    expect(x.solution_value()).to.be.a('number')
  })
})
