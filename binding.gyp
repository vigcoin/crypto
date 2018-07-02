{
    "targets": [{
        "target_name": "crypto",
        "sources": [
            "src/crypto.cc",
            "src/cryptonote/crypto/hash.c",
            "src/cryptonote/crypto/chacha8.c",
            "src/cryptonote/crypto/blake256.c",
            "src/cryptonote/crypto/keccak.c",
            "src/cryptonote/crypto/groestl.c",
            "src/cryptonote/crypto/hash-extra-blake.c",
            "src/cryptonote/crypto/hash-extra-groestl.c",
            "src/cryptonote/crypto/hash-extra-jh.c",
            "src/cryptonote/crypto/hash-extra-skein.c",
            "src/cryptonote/crypto/slow-hash.c",
            "src/cryptonote/crypto/context.cpp",
            "src/cryptonote/crypto/crypto.cpp",
            "src/cryptonote/crypto/crypto-ops.c",
            "src/cryptonote/crypto/crypto-ops-data.c",
            "src/cryptonote/common/base58.cpp",
            "src/wallet/wallet.cc"

        ],
        "include_dirs": [".", "<!@(node -p \"require('node-addon-api').include\")"],
        "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],

        "cflags_cc!": ["-fno-exceptions"],
        "cflags": [
            "-msse2",
            "-maes"
        ],
        "cflags_cc": [
            "-fexceptions",
            "-std=c++11"
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