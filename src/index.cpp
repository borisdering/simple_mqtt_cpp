#include "napi.h"
#include <string>
#include <iostream>
#include "greeting.h"
#include "mosquitto.h"

struct mosquitto *mosq;

class AsyncWorker : Napi::AsyncWorker
{
public:
    AsyncWorker(Napi::Function &callback)
        : Napi::AsyncWorker(callback) {}
    ~AsyncWorker() {}

    // Executed inside the worker-thread.
    // It is not safe to access JS engine data structure
    // here, so everything we need for input and output
    // should go on `this`.
    void Execute() { }

    void Execute(std::string message) {
        Callback().Call({Napi::String::New(Env(), message)});
    }

    // Executed when the async work is complete
    // this function will be run inside the main event loop
    // so it is safe to use JS engine data again
    void OnOK()
    {
        // Callback().Call({Env().Undefined(), Napi::Number::New(Env(), estimate)});
    }
};

AsyncWorker *onMessageWorker;

void on_message(struct mosquitto *m, void *userdata, const struct mosquitto_message *message)
{
    onMessageWorker->Execute(std::string((char*) message->payload));
}

Napi::Number init(const Napi::CallbackInfo &info)
{

    Napi::Env env = info.Env();

    int result = mosquitto_lib_init();

    return Napi::Number::New(env, result);
}

Napi::Number connect(const Napi::CallbackInfo &info)
{

    Napi::Env env = info.Env();

    Napi::String host = info[0].As<Napi::String>();
    Napi::Number port = info[1].As<Napi::Number>();

    char *id = "hackerman";

    // create a new mosquitto client instance.
    mosq = mosquitto_new(id, true, NULL);
    mosquitto_message_callback_set(mosq, on_message);
    int result = mosquitto_connect(mosq, std::string(host).c_str(), port.Int32Value(), 60);

    return Napi::Number::New(env, result);
}

void message(const Napi::CallbackInfo &info)
{

    Napi::Env env = info.Env();

    Napi::Function callback = info[0].As<Napi::Function>();

    onMessageWorker = new AsyncWorker(callback);
}

Napi::Number subscribe(const Napi::CallbackInfo &info)
{

    Napi::Env env = info.Env();

    Napi::String topic = info[0].As<Napi::String>();
    // Napi::Number qos = info[0].As<Napi::Number>();

    // missing qos,....

    int result = mosquitto_subscribe(mosq, NULL, std::string(topic).c_str(), 0);

    return Napi::Number::New(env, result);
}

Napi::Number publish(const Napi::CallbackInfo &info)
{

    Napi::Env env = info.Env();

    Napi::String topic = info[0].As<Napi::String>();
    Napi::String payload = info[0].As<Napi::String>();

    int result = mosquitto_publish(mosq, NULL, std::string(topic).c_str(), std::string(payload).length(), std::string(payload).c_str(), 0, false);

    return Napi::Number::New(env, result);
}

Napi::Number loop(const Napi::CallbackInfo &info)
{

    Napi::Env env = info.Env();

    int result = mosquitto_loop(mosq, 0, 100);

    return Napi::Number::New(env, result);
}

/**
 * 
 */
Napi::Object Init(Napi::Env env, Napi::Object exports)
{

    exports.Set(
        Napi::String::New(env, "mosquitto_init"),
        Napi::Function::New(env, init));

    exports.Set(
        Napi::String::New(env, "mosquitto_connect"),
        Napi::Function::New(env, connect));

    exports.Set(
        Napi::String::New(env, "mosquitto_subscribe"),
        Napi::Function::New(env, subscribe));

    exports.Set(
        Napi::String::New(env, "mosquitto_on_message"),
        Napi::Function::New(env, message));

    exports.Set(
        Napi::String::New(env, "mosquitto_publish"),
        Napi::Function::New(env, publish));

    exports.Set(
        Napi::String::New(env, "mosquitto_loop"),
        Napi::Function::New(env, loop));

    return exports;
}

NODE_API_MODULE(greet, Init);