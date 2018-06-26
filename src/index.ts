
const sh = require('bindings')('crypto.node')

export const echo = sh.echo;
export const enc = sh.enc;
export const dec = sh.dec;
