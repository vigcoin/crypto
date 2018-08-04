const sh = require('bindings')('crypto.node')
const assert = require("assert");
import { Reader } from "./reader";
import { FILE } from "dns";
export { Reader } from "./reader";

export const echo = sh.echo;
export const enc = sh.enc;
export const dec = sh.dec;
export const verify = sh.verify;
export const to_address = sh.to_address;
export const generate_key_pair = sh.generate_key_pair;

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

  get(): KeyPair {
    return this.keyPair;
  }

  static toHex(key: Uint8Array) {
    return Buffer.from(key).toString('hex');
  }

  static generate(): Key {
    const key: KeyPair = generate_key_pair();
    return new Key(key);
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
    let key = Buffer.concat([spendPublic.value, viewPublic.value]);
    this.address = this.getAddress(this.base58Prefix, key);
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

export class Wallet {
  prefix: number;
  reader: Reader;
  _address: Address;
  filename: string;
  password: string;

  public address: string;

  constructor(filename: string, password: string, prefix: number = 0x3d) {
    this.reader = new Reader(filename, password);
    this.prefix = prefix;
    this.filename = filename;
    this.password = password;
  }

  static create() {

  }

  async read() {
    const read = await this.reader.read(this.filename, this.password);
    const {
      iv,
      cipher
    } = read;
    // assert(iv);
    // assert(cipher);
    const plain = dec(iv, this.password, cipher);
    this._address = new Address(plain, this.prefix);
    this.address = this._address.address;
  }

  getAddress(): string {
    return this.address;
  }

  getSecretKeys() {
    return {
      send: Key.toHex(this._address.sendKeys.keyPair.private),
      view: Key.toHex(this._address.viewKeys.keyPair.private)
    }
  }
}
