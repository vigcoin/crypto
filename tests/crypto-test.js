const assert = require("assert");

const {
    echo,
    enc
} = require('../');
describe('crypto', () => {
    it('should hello', () => {
        let str = 'Hello world!';
        assert(str === echo(str));
        console.log(str);
    });

    it('should enc', () => {
        let str = 'str1';
        let str1 = 'str2';
        let asserted = false;

        try {
            let res = enc(str1, str, new Buffer.from("heelo"));
            console.log(res);
        } catch (e) {
            console.log(e);
            asserted = true;
        }

        assert(asserted);

        try {
            let res = enc(new Buffer.from("heeloosi"), str1, str);
            console.log(res);
        } catch (e) {
            console.log(e);
            asserted = true;
        }



    });
});