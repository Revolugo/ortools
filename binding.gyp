{
  'targets': [
    {
      'target_name': 'revolugo_ortools',
      'sources': [
        'src/addon.cc',
        'src/promiseworker.cc',
        'src/mpsolver.cc',
        'src/mpobjective.cc',
        'src/mpconstraint.cc',
        'src/mpvariable.cc'
      ],
      'includes': ['common.gypi'],
      'cflags_cc': [
        '-std=c++11',
        '-O4',
        '-fexceptions',
        '-Wno-deprecated'    
      ],
      'cflags!': [ 
        '-fno-exceptions'
      ],
      'cflags_cc!': [
        '-fno-rtti',
        '-fno-exceptions'
      ],
      'include_dirs': [
        '<!@(node -p "require(\'node-addon-api\').include")',
        '<(module_root_dir)/dependencies/ortools/include'
      ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_RTTI': 'YES',
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          },
          'cflags_cc': [
            '-framework CoreFoundation'
          ]
        }]
      ],
      'defines': [
        'NAPI_VERSION=4',
        'NAPI_CPP_EXCEPTIONS',
        'USE_CBC'
      ],
      'link_settings': {
        'library_dirs': [
          '<(module_root_dir)/dependencies/ortools/lib'
        ],
        'libraries': [
          '-Wl,-rpath,<(module_root_dir)/dependencies/ortools/lib',
          '-Wl,-rpath,<(module_root_dir)',
          '-lz',
          '-labsl_bad_any_cast_impl',
          '-labsl_bad_optional_access',
          '-labsl_bad_variant_access',
          '-labsl_base',
          '-labsl_city',
          '-labsl_civil_time',
          '-labsl_debugging_internal',
          '-labsl_demangle_internal',
          '-labsl_dynamic_annotations',
          '-labsl_examine_stack',
          '-labsl_failure_signal_handler',
          '-labsl_graphcycles_internal',
          '-labsl_hash',
          '-labsl_hashtablez_sampler',
          '-labsl_int128',
          '-labsl_leak_check',
          '-labsl_malloc_internal',
          '-labsl_optional',
          '-labsl_raw_hash_set',
          '-labsl_spinlock_wait',
          '-labsl_stacktrace',
          '-labsl_str_format_internal',
          '-labsl_strings',
          '-labsl_strings_internal',
          '-labsl_symbolize',
          '-labsl_synchronization',
          '-labsl_throw_delegate',
          '-labsl_time',
          '-labsl_time_zone',
          '-lprotobuf',
          '-lglog',
          '-lgflags',
          '-lCbcSolver',
          '-lCbc',
          '-lOsiCbc',
          '-lCgl',
          '-lClpSolver',
          '-lClp',
          '-lOsiClp',
          '-lOsi',
          '-lCoinUtils',
          '-lortools'
        ],
      }
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
          "destination": "<(module_path)"
        }
      ]
    }
  ]
}
