{
    "targets": [{
        "target_name": "crypto",
        "sources": [
            "src/crypto.cc",
            "src/crypto/hash.c",
            "src/crypto/chacha8.c",
            "src/crypto/hash-extra-blake.c",
            "src/crypto/hash-extra-groestl.c",
            "src/crypto/hash-extra-jh.c",
            "src/crypto/hash-extra-skein.c",
            "src/crypto/slow-hash.c",
            "src/crypto/cn-context.cpp",
            "src/crypto/crypto.cpp",
            "src/wallet/wallet.cc"

        ],
        "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
        "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],

        "cflags_cc!": ["-fno-exceptions", "-fno-rtti"],
        "cflags_cc": [
            "-std=c++0x",
            "-fexceptions",
            "-frtti",
            "-Wno-unused-private-field"
        ],
        "xcode_settings": {
            "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
            "CLANG_CXX_LIBRARY": "libc++",
            "MACOSX_DEPLOYMENT_TARGET": "10.7"
        },
        "msvs_settings": {
            "VCCLCompilerTool": {
                "ExceptionHandling": 1
            }
        }
    }]
}