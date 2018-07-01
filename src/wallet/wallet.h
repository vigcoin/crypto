

#include <iostream>
#include "../cryptonote/crypto/hash.h"
#include "../cryptonote/crypto/chacha8.h"
#include "../cryptonote/crypto/crypto.h"

using namespace std;
using namespace Crypto;

extern "C"
{

  class Wallet
  {
  public:
    Wallet(){};
    ~Wallet(){};
    static std::string decrypt(Crypto::chacha8_iv iv, const std::string &password, const std::string &cipher);
    static std::string encrypt(Crypto::chacha8_iv iv, const std::string &password, const std::string &plain);
    static bool verify_key(const SecretKey &sec, const PublicKey &expected_pub);
    inline Crypto::chacha8_iv iv()
    {
      return Crypto::rand<Crypto::chacha8_iv>();
    }
  };
}