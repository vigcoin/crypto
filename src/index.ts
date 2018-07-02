const sh = require('bindings')('crypto.node')
const assert = require("assert");

export { Reader } from "./reader";

export const echo = sh.echo;
export const enc = sh.enc;
export const dec = sh.dec;
export const verify = sh.verify;
export const to_address = sh.to_address;

export interface KeyPair {
  public: Uint8Array;
  private: Uint8Array;
};

export class Key {
  keyPair: KeyPair;
  constructor(keyPair: KeyPair) {
    this.keyPair = keyPair;
    assert(this.verify(this.keyPair.private, this.keyPair.public));
  }
  verify(secretKey: Uint8Array, publicKey: Uint8Array) {
    return verify(secretKey, publicKey);
  }
}

export class Address {
  data: Buffer;

  createdTime: number;

  sendKeys: Key;
  viewKeys: Key;

  base58Prefix: number;

  public address: string;

  constructor(data: Buffer, base58Prefix: number) {
    this.data = data;
    this.base58Prefix = base58Prefix;
    this.parse();
  }

  public getVarInt(buffer: Buffer, offset: number) {
    let value = 0;
    let shift = 0;
    let bytes = 0;

    for (let i = offset; i < buffer.length; i++) {
      const piece = buffer[i];
      value |= (piece & 0x7f) << shift;
      shift += 7;
      bytes++;
      if ((piece & 0x80) === 0) {
        break;
      }
    }

    return { value, bytes };
  }

  parseCreateTime(offset: number) {
    const vint = this.getVarInt(this.data, 0);
    this.createdTime = vint.value;
    return vint;
  }

  parseAddress(buffer: Buffer, offset: number) {
    const size = 32;
    const address = new Buffer(32);
    buffer.copy(address, 0, offset, offset + size);
    return {
      value: address,
      bytes: size
    };
  }

  parse() {
    let offset = 0;
    const time = this.parseCreateTime(0);
    offset += time.bytes;
    const spendPublic = this.parseAddress(this.data, time.bytes);

    offset += spendPublic.bytes;
    const spendPrivate = this.parseAddress(this.data, time.bytes + spendPublic.bytes);

    this.sendKeys = new Key({ public: spendPublic.value, private: spendPrivate.value });

    offset += spendPrivate.bytes;
    const viewPublic = this.parseAddress(this.data, offset);


    offset += viewPublic.bytes;
    const viewPrivate = this.parseAddress(this.data, offset);

    this.viewKeys = new Key({ public: viewPublic.value, private: viewPrivate.value });
    if (this.base58Prefix) {
      let key = Buffer.concat([spendPublic.value, viewPublic.value]);
      this.address = this.getAddress(this.base58Prefix, key);
    }
  }

  public getAddress(base58Prefix?: number, key?: Buffer) {
    if (!base58Prefix && !key) {
      return this.address;
    }
    if (base58Prefix && key) {
      const buffer = new Buffer(8);

      buffer.fill(0);

      buffer.writeUInt32LE(base58Prefix & 0xFFFFFFFF, 0);
      buffer.writeUInt32LE(0, 4);
      return to_address(buffer, key);
    }
  }



}
