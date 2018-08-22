

#include <iostream>
#include "../cryptonote/crypto/hash.h"
#include "../cryptonote/crypto/chacha8.h"
#include "../cryptonote/crypto/crypto.h"
#include "../cryptonote/include/CryptoNote.h"
#include "../cryptonote/common/StringTools.h"
#include "../cryptonote/common/StdOutputStream.h"
#include "../cryptonote/common/StdOutputStream.h"

using namespace std;
using namespace Crypto;

using namespace Common;

using namespace CryptoNote;

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
    static bool create(const std::string &filename, const std::string &password, const std::string &spendKey, const std::string &viewKey);
    inline Crypto::chacha8_iv iv()
    {
      return Crypto::rand<Crypto::chacha8_iv>();
    }
  };
}