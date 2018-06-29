const assert = require("assert");
const path = require("path");

const wallet = require("@vigcoin/wallet");
const {Reader} = wallet;

const {
    echo,
    enc,
    dec
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

    it('should dec', async () => {
        const filename = path.resolve(__dirname, '../wallet/test.wallet');
        const password = '';
        const reader = new Reader(filename, password);
        const read = await reader.read(filename, password);
        console.log("after read");
        console.log(read);
        console.log("before destruct");
        const {
            buffer,
            iv,
            cipher
        } = read;
        console.log(cipher.length, " ciper");
        const data = dec(iv, "", cipher);
        console.log("buffer size");
        console.log(data.length);
    });
});