[![Build Status](https://travis-ci.org/vigcoin/crypto.svg?branch=master)](https://travis-ci.org/vigcoin/crypto.svg?branch=master)
[![Coverage Status](https://coveralls.io/repos/github/vigcoin/crypto/badge.svg?branch=master)](https://coveralls.io/github/vigcoin/crypto?branch=master)
[![MIT license](http://img.shields.io/badge/license-MIT-brightgreen.svg)](http://opensource.org/licenses/MIT)

# Crypto wrap for vigcoin

1. you can have a lot of types

```ts
import {
  CNCheckHash,
  CNFashHash,
  CNSlowHash,
  CryptoHash,
  CryptonoteDifficulty,
  CryptoSignature,
  HASH_LENGTH,
  IHash,
  IKey,
  IKeyDerivation,
  IKeyImage,
  IPrivateKey,
  IPublicKey,
  ISignature,
  IsPublicKey,
  NULL_HASH,
  SIGNATURE_LENGTH
} from "../src/types";
```

2. you can hash a message with fast hash

```ts
import { Hash } from "../src/hash";
Hash.from(Buffer.from("Hello world!"))
```
