#include <napi.h>
#include "wallet/wallet.h"
#include <sstream>
#include <string>

using namespace Napi;

using namespace CryptoNote;


Napi::Value checkParam(const Napi::Env env, const Napi::CallbackInfo &info)
{

  if (info.Length() != 3)
  {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsBuffer())
  {
    Napi::TypeError::New(env, "Wrong arguments 1: Buffer required!").ThrowAsJavaScriptException();
    return env.Null();
  }

  Buffer<uint8_t> ivIn = info[0].As<Buffer<uint8_t>>();
  if (ivIn.Length() != 8)
  {
    Error::New(info.Env(), "Incorrect buffer length.").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[1].IsString())
  {
    Napi::TypeError::New(env, "Wrong arguments 2: String required!").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[2].IsBuffer())
  {
    Napi::TypeError::New(env, "Wrong arguments 3: String required!").ThrowAsJavaScriptException();
    return env.Null();
  }

  return Napi::Boolean::New(env, true);
}

Napi::Value enc(const Napi::CallbackInfo &info)
{
  const Napi::Env env = info.Env();
  if (checkParam(env, info) == env.Null())
  {
    return env.Null();
  }

  Buffer<uint8_t> ivIn = info[0].As<Buffer<uint8_t>>();

  std::string password = info[1].As<String>();
  std::string plain = info[2].As<String>();

  uint8_t *pIv = ivIn.Data();
  chacha8_iv iv;
  memcpy(&iv, pIv, sizeof(iv));

  std::string cipher = Wallet::encrypt(iv, password, plain);

  return Napi::String::New(env, cipher);
}

Napi::Value decrypt(const Napi::CallbackInfo &info)
{
  const Napi::Env env = info.Env();
  if (checkParam(env, info) == env.Null())
  {
    return env.Null();
  }

  Buffer<uint8_t> ivIn = info[0].As<Buffer<uint8_t>>();

  uint8_t *pIv = ivIn.Data();
  chacha8_iv iv;
  memcpy(&iv, pIv, sizeof(iv));

  std::string password = info[1].As<String>().Utf8Value();

  Buffer<uint8_t> cipherBuffer = info[2].As<Buffer<uint8_t>>();

  uint8_t *pCipher = cipherBuffer.Data();
  size_t cipherSize = cipherBuffer.Length();

  std::ostringstream convert;
  for (size_t a = 0; a < cipherSize; a++)
  {
    convert << (uint8_t)pCipher[a];
  }

  std::string cipher = convert.str();
  std::string plain = Wallet::decrypt(iv, password, cipher);

  std::ostringstream oss;

  const uint8_t *data = (uint8_t *)plain.data();
  size_t length = plain.size();

  uint8_t *des = new uint8_t[length];
  memcpy(des, data, length);

  return Napi::Buffer<uint8_t>::New(env, des, length);
}

Napi::Value echo(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  if (info.Length() < 1)
  {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString())
  {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }
  return Napi::String::New(env, info[0].As<Napi::String>().Utf8Value());
}

Napi::Value verify(const Napi::CallbackInfo &info)
{
  const Napi::Env env = info.Env();

  if (info.Length() != 2)
  {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsBuffer())
  {
    Napi::TypeError::New(env, "Wrong arguments 1: Buffer required!").ThrowAsJavaScriptException();
    return env.Null();
  }

  Buffer<uint8_t> secretKey = info[0].As<Buffer<uint8_t>>();
  if (secretKey.Length() != 32)
  {
    Error::New(info.Env(), "Incorrect buffer length.").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[1].IsBuffer())
  {
    Napi::TypeError::New(env, "Wrong arguments 1: Buffer required!").ThrowAsJavaScriptException();
    return env.Null();
  }

  Buffer<uint8_t> publicKey = info[1].As<Buffer<uint8_t>>();
  if (publicKey.Length() != 32)
  {
    Error::New(info.Env(), "Incorrect buffer length.").ThrowAsJavaScriptException();
    return env.Null();
  }

  SecretKey secret;
  PublicKey pub;

  const uint8_t *pSecretKey = secretKey.Data();
  const uint8_t *pPublicKey = publicKey.Data();

  memcpy(&secret, pSecretKey, sizeof(SecretKey));
  memcpy(&pub, pPublicKey, sizeof(PublicKey));

  bool res = Wallet::verify_key(secret, pub);

  return Napi::Boolean::New(env, res);
}

// Napi::Value address(const Napi::CallbackInfo &info)
// {
//   const Napi::Env env = info.Env();

//   if (info.Length() != 3)
//   {
//     Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
//     return env.Null();
//   }

//   if (!info[0].IsBuffer())
//   {
//     Napi::TypeError::New(env, "Wrong arguments 1: Buffer required!").ThrowAsJavaScriptException();
//     return env.Null();
//   }

//   Buffer<uint8_t> sendKey = info[0].As<Buffer<uint8_t>>();
//   if (sendKey.Length() != 32)
//   {
//     Error::New(info.Env(), "Incorrect buffer length.").ThrowAsJavaScriptException();
//     return env.Null();
//   }

//   if (!info[1].IsBuffer())
//   {
//     Napi::TypeError::New(env, "Wrong arguments 1: Buffer required!").ThrowAsJavaScriptException();
//     return env.Null();
//   }

//   Buffer<uint8_t> viewKey = info[1].As<Buffer<uint8_t>>();
//   if (viewKey.Length() != 32)
//   {
//     Error::New(info.Env(), "Incorrect buffer length.").ThrowAsJavaScriptException();
//     return env.Null();
//   }

//   if (!info[2].IsNumber())
//   {
//     Napi::TypeError::New(env, "Wrong arguments 1: Uint64 required!").ThrowAsJavaScriptException();
//     return env.Null();
//   }

//   PublicKey send;
//   PublicKey view;

//   const uint8_t *pSend = sendKey.Data();
//   const uint8_t *pView = viewKey.Data();

//   memcpy(&send, pSend, sizeof(PublicKey));
//   memcpy(&view, pView, sizeof(PublicKey));

//   AccountPublicAddress address;
//   address.spendPublicKey = send;
//   address.viewPublicKey = view;

//   uint64_t prefix58 = info[2].As<Number>().Int64Value();

//   std::string addressStr = Wallet::get_address(prefix58, address);

//   return Napi::String::New(env, addressStr);
// }

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "echo"),
              Napi::Function::New(env, echo));

  exports.Set(Napi::String::New(env, "enc"),
              Napi::Function::New(env, enc));

  exports.Set(Napi::String::New(env, "dec"),
              Napi::Function::New(env, decrypt));

  exports.Set(Napi::String::New(env, "verify"),
              Napi::Function::New(env, verify));
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
