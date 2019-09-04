const util = require('util')
const fs = require('fs')
const path = require('path')
const tar = require('tar')
const request = require('request')
const progress = require('progress-stream')
const SingleBar = require('cli-progress').SingleBar;

const PREREQS_MAP = {
  'Ubuntu-18.04': 'https://github.com/google/or-tools/releases/download/v7.3/or-tools_ubuntu-18.04_v7.3.7083.tar.gz',
  'Ubuntu-16.04': 'https://github.com/google/or-tools/releases/download/v7.3/or-tools_ubuntu-16.04_v7.3.7083.tar.gz',
  'CentOS-7': 'https://github.com/google/or-tools/releases/download/v7.3/or-tools_centos-7_v7.3.7083.tar.gz',
  'Debian-9': 'https://github.com/google/or-tools/releases/download/v7.3/or-tools_debian-9_v7.3.7083.tar.gz',
  'MacOSX': 'https://github.com/google/or-tools/releases/download/v7.3/or-tools_MacOsX-10.14.4_v7.3.7083.tar.gz'
};

const TEMP_PATH = path.resolve(__dirname, './_tmp')
const ORTOOLS_PATH = path.resolve(__dirname, './dependencies/ortools')

const _installed = async () => {
  const _access = util.promisify(fs.access)
  return _access(path.resolve(ORTOOLS_PATH, 'include'))
    .then(() => {
      return _access(path.resolve(ORTOOLS_PATH, 'lib'))
    })
    .then(() => true)
    .catch(err => false)
}

const _streamFetchOrtoolsBinaries = async (platform) => {
  return new Promise((resolve, reject) => {
    try {
      fs.mkdirSync(TEMP_PATH, { recursive: true })
      const prog = progress({ time: 100 })
      const bar = new SingleBar()
      const stream = request.get(PREREQS_MAP[platform])
        .on('response', (response) => {
          const length = parseInt(response.headers['content-length'])
          prog.setLength(length)
          bar.start(length, 0)
        })
        .pipe(prog)
        .pipe(tar.x({ cwd: TEMP_PATH }))
      prog.on('progress', (progress) => {
        bar.update(progress.transferred)
      })
      stream.on('close', () => { 
        bar.stop()
        const files = fs.readdirSync(TEMP_PATH)
        if (files.length === 0) {
          return reject(new Error('Empty download directory, fetching prerequisites failed.'))
        }
        if (files.length > 1) {
          console.warn(`Multiple files found in download directory - returning first found: ${files[0]}`)
        }
        resolve(path.resolve(TEMP_PATH, files[0]))
      })
      stream.on('error', (err) => {
        bar.stop()
        reject(err)
      })
    }
    catch(err) {
      reject(err)
    }
 })
}

(async () => {

  const _exec = util.promisify(require('child_process').exec)

  if (process.argv.length > 2 && process.argv[2] === 'clean') {
    await _exec(`rm -rf ${TEMP_PATH}`)
    await _exec(`find ${ORTOOLS_PATH} -mindepth 1 -type d -exec rm -rf {} +`)
  }
  else if (await _installed()) {
    console.log('Prerequisites already installed, skipping.')
    process.exit(0)
  }

  let platform
  try {
    const { stdout, stderr } = await _exec('sw_vers -productName')
    if (stdout.trim() == 'Mac OS X') {
      platform = 'MacOSX'
    }
    else {
      throw new Error(`Unknown sw_vers platform.`)
    }
  }
  catch(err) {
    const dist = await _exec('lsb_release -is')
    platform = dist.stdout.trim()
    const version = await _exec('lsb_release -rs')
    platform += `-${version.stdout.trim()}`
  }
  finally {
    if (!PREREQS_MAP[platform]) {
     console.log(`Unsupported platform, cannot install prerequisites.`)
     process.exit(1)
    }
  }
  
  try {
    const ortoolsDir = await _streamFetchOrtoolsBinaries(platform)
    await _exec(`mv ${path.resolve(ortoolsDir, 'lib')} ${path.resolve(ORTOOLS_PATH, 'lib')}`)
    await _exec(`mv ${path.resolve(ortoolsDir, 'include')} ${path.resolve(ORTOOLS_PATH, 'include')}`)
    await _exec(`rm -rf ${TEMP_PATH}`)
  }
  catch(err) {
    console.error(err)
    process.exit(1)
  }

})()


