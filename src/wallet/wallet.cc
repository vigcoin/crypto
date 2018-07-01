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

  Crypto::generate_chacha8_key(context, password, key);
  cipher.resize(plain.size());
  Crypto::chacha8(plain.data(), plain.size(), key, iv, &cipher[0]);

  return cipher;
}

bool Wallet::verify_key(const SecretKey& sec, const PublicKey& expected_pub)
{
  PublicKey pub;
  bool r = Crypto::secret_key_to_public_key(sec, pub);
  return r && expected_pub == pub;
}
