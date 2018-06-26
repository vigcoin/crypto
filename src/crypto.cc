#include <napi.h>
#include "wallet/wallet.h"

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


  if (!info[2].IsString())
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

Napi::Value dec(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  if (info.Length() != 3)
  {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString())
  {
    Napi::TypeError::New(env, "Wrong arguments 1: String required!").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[1].IsString())
  {
    Napi::TypeError::New(env, "Wrong arguments 2: String required!").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[2].IsBuffer())
  {
    Napi::TypeError::New(env, "Wrong arguments 3: Buffer required!").ThrowAsJavaScriptException();
    return env.Null();
  }
  // if ((!info[2].As<Napi::Int8Array>).length() != 8)
  // {
  //   Napi::TypeError::New(env, "Wrong arguments 3: Buffer length should be 8!").ThrowAsJavaScriptException();
  //   return env.Null();
  // }

  Buffer<uint8_t> ivIn = info[2].As<Buffer<uint8_t>>();
  if (ivIn.Length() != 8)
  {
    Error::New(info.Env(), "Incorrect buffer length.").ThrowAsJavaScriptException();
    return env.Null();
  }

  cout << (char *)ivIn.Data() << endl;

  return Napi::String::New(env, info[0].As<Napi::String>().Utf8Value() + info[1].As<Napi::String>().Utf8Value());
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
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
