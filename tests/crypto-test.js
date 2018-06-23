const assert = require( "assert");

const {
    echo,
    add
} = require('../');
describe('crypto', () => {
    it('should hello', () => {
        let str = 'Hello world!';
        assert(str === echo(str));
        console.log(str);
    });
});