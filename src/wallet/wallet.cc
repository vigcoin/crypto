#include "wallet.h"
#include <fstream>

#include "../cryptonote/serialization/BinaryOutputStreamSerializer.h"
#include "../cryptonote/wallet/KeysStorage.h"
#include "../cryptonote/core/CryptoNoteSerialization.h"

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

bool Wallet::verify_key(const SecretKey &sec, const PublicKey &expected_pub)
{
  PublicKey pub;
  bool r = Crypto::secret_key_to_public_key(sec, pub);
  return r && expected_pub == pub;
}

bool Wallet::create(const std::string &filename, const std::string &password, const std::string &spendKey, const std::string &viewKey)
{
  Crypto::SecretKey viewSecretKey;
  Crypto::SecretKey spendSecretKey;
  if (!Common::podFromHex(viewKey, viewSecretKey))
  {
    std::cout << "Cannot parse view secret key: " << viewKey;
    return false;
  }
  if (!Common::podFromHex(spendKey, spendSecretKey))
  {
    std::cout << "Cannot parse send secret key: " << spendKey;
    return false;
  }

  Crypto::PublicKey spendPubKey;
  Crypto::PublicKey viewPubKey;

  if (!Crypto::secret_key_to_public_key(spendSecretKey, spendPubKey))
  {
    std::cout << "Cannot get send public key from secret key: " << spendKey;
    return false;
  }

  if (!Crypto::secret_key_to_public_key(viewSecretKey, viewPubKey))
  {
    std::cout << "Cannot get send public key from secret key: " << viewKey;
    return false;
  }

  char keyStore[sizeof(Crypto::PublicKey) * 2];

  memcpy(keyStore, &spendPubKey, sizeof(Crypto::PublicKey));
  memcpy(keyStore + sizeof(Crypto::PublicKey), &viewPubKey, sizeof(Crypto::PublicKey));

  CryptoNote::AccountKeys keys;
  keys.address.spendPublicKey = spendPubKey;
  keys.address.viewPublicKey = viewPubKey;
  keys.spendSecretKey = spendSecretKey;
  keys.viewSecretKey = viewSecretKey;

  std::ofstream file;

  file.open(filename, std::ios_base::binary | std::ios_base::out | std::ios::trunc);
  if (file.fail())
  {
    std::cout << "error opening file: " << filename << endl;
    return false;
  }
  try
  {
    std::stringstream plainArchive;
    StdOutputStream plainStream(plainArchive);
    CryptoNote::BinaryOutputStreamSerializer serializer(plainStream);

    // Saving Keys;

    CryptoNote::KeysStorage ks;

    ks.creationTimestamp = time(NULL);
    ks.spendPublicKey = keys.address.spendPublicKey;
    ks.spendSecretKey = keys.spendSecretKey;
    ks.viewPublicKey = keys.address.viewPublicKey;
    ks.viewSecretKey = keys.viewSecretKey;

    ks.serialize(serializer, "keys");

    bool hasDetails = false;

    serializer(hasDetails, "has_details");

    std::string cache("");

    serializer.binary(const_cast<std::string &>(cache), "cache");

    std::string plain = plainArchive.str();
    std::string cipher;
    Crypto::chacha8_key key;
    Crypto::cn_context context;
    Crypto::generate_chacha8_key(context, password, key);

    cipher.resize(plain.size());

    Crypto::chacha8_iv iv = Crypto::rand<Crypto::chacha8_iv>();
    Crypto::chacha8(plain.data(), plain.size(), key, iv, &cipher[0]);

    uint32_t version = 1;
    StdOutputStream output(file);
    CryptoNote::BinaryOutputStreamSerializer s(output);
    s.beginObject("wallet");
    s(version, "version");
    s(iv, "iv");
    s(cipher, "data");
    s.endObject();

    file.flush();
    file.close();
  }
  catch (const std::exception &e)
  {
    std::cout << "failed to generate new wallet: " << e.what();
    return false;
  }
  return true;
}