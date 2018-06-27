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
    it('should hello', () => {
        let str = 'Hello world!';
        assert(str === echo(str));
        console.log(str);
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
        // console.log(cipher)
        const data = dec(iv, "", cipher.toString('binary'));
        // console.log(data);
    });
});