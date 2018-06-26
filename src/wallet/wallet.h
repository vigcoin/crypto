

#include <iostream>
#include "../crypto/hash.h"
#include "../crypto/chacha8.h"
#include "../crypto/crypto.h"

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
    inline Crypto::chacha8_iv iv()
    {
      return Crypto::rand<Crypto::chacha8_iv>();
    }
  };
}