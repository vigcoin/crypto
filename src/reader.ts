import * as fs from 'fs';
import { Readable, Writable } from 'stream';
import { promisify } from 'util';

export class Reader {
  private filename: string;
  private password: string;
  private content?: Uint8Array[];
  private version?: number;
  private iv?: Buffer;
  private cipher?: Buffer;
  private plain?: Buffer;

  constructor(filename: string, password: string) {
    this.filename = filename;
    this.password = password;
  }

  public async getBuffer(readable: Readable): Promise<Buffer> {
    const bytes: Buffer[] = [];
    const length = 0;
    return new Promise<Buffer>((resolve, reject) => {
      readable.on('data', (chunk: Buffer) => {
        bytes.push(chunk);
      });

      readable.on('end', () => {
        resolve(Buffer.concat(bytes));
      });
      readable.on('error', e => {
        reject(e);
      });
    });
  }

  public async getVarInt(buffer: Buffer, offset: number) {
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

  public async readVersion(buffer: Buffer) {
    const version = await this.getVarInt(buffer, 0);
    this.version = version.value;
    return version;
  }

  public async readIV(buffer: Buffer, offset: number) {
    const CHACHA8_KEY_SIZE = 8;
    const iv = new Buffer(CHACHA8_KEY_SIZE);
    buffer.copy(iv, 0, offset, offset + CHACHA8_KEY_SIZE);
    this.iv = iv;
    return { value: iv, bytes: CHACHA8_KEY_SIZE };
  }

  public async readCipher(buffer: Buffer, offset: number) {
    const cipherInt = await this.getVarInt(buffer, offset);
    offset += cipherInt.bytes;
    const cipher = new Buffer(cipherInt.value);
    buffer.copy(cipher, 0, offset, offset + cipherInt.value);

    this.cipher = cipher;
    return { value: cipher, bytes: cipherInt.bytes + cipherInt.value };
  }

  public async read(filename: string, password: string) {
    const stream = fs.createReadStream(filename);

    // Should Not Set Encoding! which will convert stream to string
    // No!  stream.setEncoding("binary");
    const buffer = await this.getBuffer(stream);

    const version = await this.readVersion(buffer);

    this.version = version.value;
    const iv = await this.readIV(buffer, version.bytes);
    const cipher = await this.readCipher(buffer, version.bytes + iv.bytes);

    return {
      cipher: cipher.value,
      iv: iv.value,
      version: version.value
    };
  }
}
