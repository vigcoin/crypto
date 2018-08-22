
const sh = require('bindings')('crypto.node')
const assert = require("assert");

export const generate_key_pair = sh.generate_key_pair;


export interface KeyPair {
  public: Uint8Array;
  private: Uint8Array;
};

export class AddressKey {
  spendPrivateKeyStr: string;
  spendKey: KeyPair;
  viewKey: KeyPair;
  constructor(spendPrivateKey: string, viewPrivateKey:string) {

  }
}

export class Key {
  keyPair: KeyPair;
  constructor(keyPair: KeyPair) {
    this.keyPair = keyPair;
    assert(this.verify(this.keyPair.private, this.keyPair.public));
  }

  verify(secretKey: Uint8Array, publicKey: Uint8Array) {
    return sh.verify(secretKey, publicKey);
  }

  get(): KeyPair {
    return this.keyPair;
  }

  static toHex(key: Uint8Array) {
    return Buffer.from(key).toString('hex');
  }

  static toBuffer() {

  }

  static generate(): Key {
    const key: KeyPair = generate_key_pair();
    return new Key(key);
  }
}