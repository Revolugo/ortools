const chai = require('chai')
const chaiAsPromised = require('chai-as-promised')

const expect = chai.expect
chai.use(chaiAsPromised)

global.expect = expect
