#include <napi.h>
#include "wallet/wallet.h"
#include <sstream>
#include <string>

using namespace Napi;

Napi::Value checkParam(const Napi::Env env, const Napi::CallbackInfo &info)
{

  if (info.Length() != 3)
  {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  cout << "get info 1" << endl;

  if (!info[0].IsBuffer())
  {
    Napi::TypeError::New(env, "Wrong arguments 1: Buffer required!").ThrowAsJavaScriptException();
    return env.Null();
  }
  cout << "get info 2" << endl;

  Buffer<uint8_t> ivIn = info[0].As<Buffer<uint8_t>>();
  if (ivIn.Length() != 8)
  {
    Error::New(info.Env(), "Incorrect buffer length.").ThrowAsJavaScriptException();
    return env.Null();
  }
  cout << "get info 3" << endl;

  if (!info[1].IsString())
  {
    Napi::TypeError::New(env, "Wrong arguments 2: String required!").ThrowAsJavaScriptException();
    return env.Null();
  }

  cout << "get info 4" << endl;

  if (!info[2].IsBuffer())
  {
    Napi::TypeError::New(env, "Wrong arguments 3: String required!").ThrowAsJavaScriptException();
    return env.Null();
  }

  return Napi::Boolean::New(env, true);
}

Napi::Value enc(const Napi::CallbackInfo &info)
{
  cout << "before get info" << endl;

  const Napi::Env env = info.Env();
  if (checkParam(env, info) == env.Null())
  {
    return env.Null();
  }

  cout << "out get info" << endl;

  Buffer<uint8_t> ivIn = info[0].As<Buffer<uint8_t>>();

  std::string password = info[1].As<String>();
  std::string plain = info[2].As<String>();

  uint8_t *pIv = ivIn.Data();
  chacha8_iv iv;
  memcpy(&iv, pIv, sizeof(iv));
  cout << "copied !" << endl;

  std::string cipher = Wallet::encrypt(iv, password, plain);
  cout << cipher << endl;

  return Napi::String::New(env, cipher);
}

Napi::Value decrypt(const Napi::CallbackInfo &info)
{
  cout << "inside decrypt" << endl;
  const Napi::Env env = info.Env();
  if (checkParam(env, info) == env.Null())
  {
    return env.Null();
  }

  Buffer<uint8_t> ivIn = info[0].As<Buffer<uint8_t>>();

  uint8_t *pIv = ivIn.Data();
  chacha8_iv iv;
  memcpy(&iv, pIv, sizeof(iv));
  cout << "copied !" << endl;

  for (size_t i = 0; i < ivIn.Length(); i++)
  {
    cout << (char)pIv[i] << endl;
  }

  std::string password = info[1].As<String>().Utf8Value();
  cout << "password: " << password << endl;

  Buffer<uint8_t> cipherBuffer = info[2].As<Buffer<uint8_t>>();

  cout << "cipher buffer size: " << cipherBuffer.Length() << endl;

  uint8_t *pCipher = cipherBuffer.Data();
  size_t cipherSize = cipherBuffer.Length();

  std::ostringstream convert;
  for (size_t a = 0; a < cipherSize; a++)
  {
    convert << (uint8_t)pCipher[a];
  }

  std::string cipher = convert.str();

  // std::cout << key_string << std::endl;

  // std::string cipher = Napi::String::New(env, (const char *)pCipher);

  // cout << cipher << endl;
  cout << "ciper size: " << cipher.size() << endl;
  std::string plain = Wallet::decrypt(iv, password, cipher);
  // cout << "plain: " << plain << endl;
  cout << "plain size: " << plain.size() << endl;

  std::ostringstream oss;

  const uint8_t *data =  (uint8_t *)plain.data();

  // for(size_t i = 0; i < plain.length(); i++) {
  //   uint8_t top = (data[i] >> 4) & 0xF;
  //   uint8_t bottom = data[i] & 0xF;
  //   oss << bottom ;
  //   oss << top ;
  //   // cout << std::hex << bottom << endl;
  //   // cout << std::hex << top << endl;
  // }

  // cout << oss.str().length() << endl;
  size_t length = plain.size();
  // cout << data << endl;
  cout << length << endl;

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

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "echo"),
              Napi::Function::New(env, echo));

  exports.Set(Napi::String::New(env, "enc"),
              Napi::Function::New(env, enc));

  exports.Set(Napi::String::New(env, "dec"),
              Napi::Function::New(env, decrypt));
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
