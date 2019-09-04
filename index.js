'use strict'

const binary = require('node-pre-gyp')
const path = require('path')

const addon_path = binary.find(path.resolve(path.join(__dirname, './package.json')))
const addon = require(addon_path)
module.exports = addon
