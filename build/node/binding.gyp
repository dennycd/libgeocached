{
    'target_defaults' : {
        'conditions' : [
            #mac os
            ['OS=="mac"', {
             
                 'xcode_settings' : {
                    'OTHER_CPLUSPLUSFLAGS' : ["-fexceptions", "-stdlib=libstdc++", "-std=c++11"],
                    'GCC_ENABLE_CPP_EXCEPTIONS' : "YES",
                 }
             
             }],
                        
            #linux
            ['OS=="linux"', {
    
                #compiler flags
                'cflags!' : ["-fno-exceptions","-stdlib=libstdc++", "-std=c++11"],
                'cflags_cc!' : ["-fno-exceptions"],
                'cflags_cc' : ["-fexceptions"]
             }]
        ]
    },
    
    'targets' : [
        {
                 'target_name' : "geocached",
                 'sources' : ["geocached.cpp"]
        }
    ]
}