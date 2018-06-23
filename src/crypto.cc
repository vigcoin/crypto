#include <napi.h>

Napi::Value echo(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }
  return Napi::String::New(env, info[0].As<Napi::String>().Utf8Value());
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "echo"),
              Napi::Function::New(env, echo));
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
