#include "wallet.h"

std::string Wallet::decrypt(Crypto::chacha8_iv iv, const std::string &password, const std::string &cipher)
{
  Crypto::chacha8_key key;
  Crypto::cn_context context;
  std::string plain;
  Crypto::generate_chacha8_key(context, password, key);

  plain.resize(cipher.size());

  Crypto::chacha8(cipher.data(), cipher.size(), key, iv, &plain[0]);

  return plain;
}

std::string Wallet::encrypt(Crypto::chacha8_iv iv, const std::string &password, const std::string &plain)
{
  Crypto::chacha8_key key;
  Crypto::cn_context context;
  std::string cipher;
  cout << "inside enc !" << endl;

  Crypto::generate_chacha8_key(context, password, key);
  cout << "generated key !" << endl;

  cipher.resize(plain.size());
  Crypto::chacha8(plain.data(), plain.size(), key, iv, &cipher[0]);
  cout << "generated cipher !" << endl;

  return cipher;
}
