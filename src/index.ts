const sh = require('bindings')('crypto.node')

export const echo = sh.echo;
export const enc = sh.enc;
export const dec = sh.dec;

export interface KeyPair {
  public: Uint8Array;
  private: Uint8Array;
};

export class Key {
  keyPair: KeyPair;
  constructor(keyPair:KeyPair) {
    this.keyPair = keyPair;
  }
  verify() {

  }
}

export class Address {
  data: Buffer;

  createdTime: number;

  sendKeys: KeyPair;
  viewKeys: KeyPair;


  constructor(data: Buffer, Base85Prefix: number) {
    this.data = data;
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

  parseAddress(buffer:Buffer, offset: number) {
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

    this.sendKeys = {public: spendPublic.value, private: spendPrivate.value};

    offset += spendPrivate.bytes;
    const viewPublic = this.parseAddress(this.data, offset);


    offset += viewPublic.bytes;
    const viewPrivate = this.parseAddress(this.data, offset);

    this.viewKeys = {public: viewPublic.value, private: viewPrivate.value};
  }

}
