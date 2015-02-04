{
    'targets':[{
        'variables':{
            'root_path'     :'./..',
        },
        'target_name'   :'tinyxml2',
        'include_dirs':[
            '.',
        ],
        
        'conditions': [[
            'OS=="win"', {
                    'type'          :'shared_library',
                    'msvs_configuration_attributes': {
                        # 中间目录
                        'IntermediateDirectory':'$(SolutionDir)/$(Configuration)/$(ProjectName)',
                        # 输出目录
                        'OutputDirectory':'$(SolutionDir)/../bin/$(Configuration)',
                    },
                    'configurations':{
                        'Debug':{
                            'defines'       :['DEBUG', '_DEBUG'],
                            'msvs_settings' : {                                
                                'VCCLCompilerTool': {
                                   # MTD
                                   'RuntimeLibrary': '1',
                                },
                                'VCLinkerTool': {
                                    # vs引用第三方库路径设置
                                   'AdditionalLibraryDirectories': [],
                                    # 引用依赖库
                                   'AdditionalDependencies': [],
                                   'GenerateDebugInformation': 'true',
                                   'ProgramDatabaseFile': '$(OutDir)$(ProjectName)_d.pdb',
                                   'OutputFile':'$(OutDir)$(ProjectName)_d$(TargetExt)',
                                },
                            },
                        },
                        'Release':{'defines':['RELEASE', '_RELEASE']}
                    },
                    'defines':['TINYXML2_EXPORT','WIN32', '_LIB', '_WINDLL', '_UNICODE', 'UNICODE'],
                    'sources':[]
                },
                { #OS != "win",
                    'type'          :'static_library',
                }                
            ],
            ['OS == "linux"',{
                }
            ]
        ],
        
        'sources':[
            'tinyxml2.cpp',
        ], 
      },
    ],
}