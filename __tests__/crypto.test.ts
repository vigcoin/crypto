const assert = require("assert");
const path = require("path");
const fs = require("fs");

import { Readable } from 'stream';

const {
    echo,
    enc,
    dec,
    Key,
    KeyPair,
    Address,
    Reader,
    Wallet
} = require('../');

describe('crypto', () => {
    it('should dec', () => {
        let iv = new Buffer(8);
        iv[0] = 'i'.charCodeAt(0);
        iv[1] = 'v'.charCodeAt(0);

        const password = "password 1";
        const cipher = new Buffer("cipher xxx 2");

        const data = dec(iv, password, cipher);

    });

    it('should get wrong buffer', async () => {
        var mockedStream = new Readable();
        mockedStream._read = function (size) { /* do nothing */ };
        let catched = false;
        try {
            const stream = fs.createReadStream(mockedStream);
            const buffer = await this.getBuffer(mockedStream);
            mockedStream.emit('error', new Error("error occure"));
        } catch (e) {
            catched = true;
        }
        assert(catched);

    });
    it('should dec', async () => {
        const filename = path.resolve(__dirname, '../wallet/test.wallet');


        const password = '';
        const reader = new Reader(filename, password);
        const read = await reader.read(filename, password);
        const {
            iv,
            cipher
        } = read;
        const plain = dec(iv, "", cipher);
        const add = new Address(plain, 0x3d);
        const address1 = add.getAddress(0x3d);
        assert(!address1);
        const address = add.getAddress();
        assert(add.address === address);
        assert(add.address === "BKC4AgG14PnQyfpEeEQSNSYjhBo5237Yf1pScL4c9rQ4LQTngRWHeEuJcSmW8cc6AjA3vgGSLR3odRtphDGnQAVHEuJN8p9");
    });

    it('should Wallet', async () => {

        const filename = path.resolve(__dirname, '../wallet/test.wallet');
        const password = '';

        const wallet = new Wallet(filename, password);

        await wallet.read();

        assert(wallet.getAddress() === "BKC4AgG14PnQyfpEeEQSNSYjhBo5237Yf1pScL4c9rQ4LQTngRWHeEuJcSmW8cc6AjA3vgGSLR3odRtphDGnQAVHEuJN8p9");
    });

    it('should Wallet', async () => {

        const filename = path.resolve(__dirname, '../wallet/password.wallet');
        const password = '';
        let catched = false;

        try {
            const wallet = new Wallet(filename, password);
            await wallet.read();

        } catch (e) {
            console.log(e);
            catched = true;
        }
        assert(catched);
    });

    it('should Wallet', async () => {

        const filename = path.resolve(__dirname, '../wallet/password.wallet');
        const password = '1234';
        let catched = false;

        try {
            const wallet = new Wallet(filename, password);
            await wallet.read();
            assert(wallet.getAddress() === "BGXjsWEJGoEW9hQm4rWyzvefLKEsr72RaJqjpryicRRfiuUQkpadGzBb3UtsinaDQTd48LPCQtgQf2pegsz8W3r46Y8fN9e");
            const keys = wallet.getSecretKeys();
            console.log(keys);
            assert(keys.send);
            assert(keys.view);
        } catch (e) {
            catched = true;
        }
        assert(!catched);
    });

    it('should generate keys', async () => {
        let key: Key = Key.generate();
        let pair: KeyPair = key.get();
        assert(pair.public);
        assert(pair.private);
    });
});