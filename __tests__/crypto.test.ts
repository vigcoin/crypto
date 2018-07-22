const assert = require("assert");
const path = require("path");

const {
    echo,
    enc,
    dec,
    Key,
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
        assert(add.address === "BKC4AgG14PnQyfpEeEQSNSYjhBo5237Yf1pScL4c9rQ4LQTngRWHeEuJcSmW8cc6AjA3vgGSLR3odRtphDGnQAVHEuJN8p9");
    });

    it('should Wallet', async () => {

        const filename = path.resolve(__dirname, '../wallet/test.wallet');
        const password = '';

        const wallet = new Wallet(filename, password);

        await wallet.read();

        assert(wallet.getAddress() === "BKC4AgG14PnQyfpEeEQSNSYjhBo5237Yf1pScL4c9rQ4LQTngRWHeEuJcSmW8cc6AjA3vgGSLR3odRtphDGnQAVHEuJN8p9");
    });

    it('should generate keys', async () => {
        let key: Key = Key.generate();
    });
});