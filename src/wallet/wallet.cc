#include "wallet.h"

std::string Wallet::decrypt(Crypto::chacha8_iv iv, const std::string &password, const std::string &cipher)
{
  // cout << "inside dec !" << endl;

  Crypto::chacha8_key key;
  Crypto::cn_context context;
  std::string plain;
  // cout << "before key !" << endl;

  // cout << "password = " << password << endl;

  // cout << "ciper size = " << cipher.size() << endl;

  // cout << "context = " << (char *)context << endl;
  // cout << "key = " << (char *)key << endl;

  Crypto::generate_chacha8_key(context, password, key);
  // cout << "after key !" << endl;
  // cout << sizeof(key) << endl;
  // cout << cipher.size() << endl;

  plain.resize(cipher.size());
  // cout << "after resize !" << endl;

  // cout << cipher.size() << endl;

  Crypto::chacha8(cipher.data(), cipher.size(), key, iv, &plain[0]);
  // cout << "after dec !" << endl;
  // cout << plain << endl;

  // cout << plain.size() << endl;

  return plain;
}

std::string Wallet::encrypt(Crypto::chacha8_iv iv, const std::string &password, const std::string &plain)
{
  Crypto::chacha8_key key;
  Crypto::cn_context context;
  std::string cipher;
  // cout << "inside enc !" << endl;

  Crypto::generate_chacha8_key(context, password, key);
  // cout << "generated key !" << endl;

  cipher.resize(plain.size());
  Crypto::chacha8(plain.data(), plain.size(), key, iv, &cipher[0]);
  // cout << "generated cipher !" << endl;

  return cipher;
}
