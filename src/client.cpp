#include <string>
#include <iostream>
#include <thread>
#include "mosquitto.h"
#include "client.h"

/**
 * Simple C static on connect callback wrapper.
 * @param mosq
 * @param userdata  in our case a client pointer itself
 * @param rc    the return code of the connection response, one of:
 *
 *  0 - success
 *  1 - connection refused (unacceptable protocol version)
 *  2 - connection refused (identifier rejected)
 *  3 - connection refused (broker unavailable)
 */
static void on_connect(struct mosquitto* mosq, void *userdata, int rc) {

    // due to the fact that we receive the context
    // we try to call the actual callback.
    class client *c = (class client*) userdata;

    std::function<void()> f_display_42 = []() { std::cout << "subscribed" << std::endl; };

    c->subscribe("#", 0, f_display_42);
}

/**
 * Simple C static on message callback wrapper.
 * @param mosq
 * @param userdata
 * @param message
 */
void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {

    // due to the fact that we receive the context
    // we try to call the actual callback.
    std::string payload((char*) message->payload);

    // need to dispatch to node js context 

    std::cout << "on message: " << payload << std::endl;
}

client::client(std::string host, int port) {

    // must be called before any other mosquitto functions.
    int result = mosquitto_lib_init();

    // check for a positive result in this case == 0
    // if (result != 0)
    //     throw new std::runtime_error("unable to initilaize mosquitto library");

    this->id = "hackerman";
    this->topic = "client";
    this->host = host;
    this->port = port;

    // create a new mosquitto client instance.
    this->mosquitto = mosquitto_new(this->id.c_str(), true, this);

    // Used to tell the library that your application is using threads, but not
    // using <mosquitto_loop_start>. The library operates slightly differently when
    // not in threaded mode in order to simplify its operation. If you are managing
    // your own threads and do not use this function you will experience crashes
    // due to race conditions.
    mosquitto_threaded_set(this->mosquitto, true);

    // connect to an MQTT broker and set on connect callback
    mosquitto_connect_callback_set(this->mosquitto, on_connect);

    mosquitto_loop_start(this->mosquitto);

    mosquitto_connect_async(this->mosquitto, this->host.c_str(), this->port, client::keepalive);

    // Set the message callback. This is called when a message is received from the
    // broker.
    mosquitto_message_callback_set(this->mosquitto, on_message);
}

/**
 * Start listening to messages. 
 */
void client::start() {

    std::thread main([](struct mosquitto* mosq){

        // mosquitto_loop_start(mosq);
        // mosquitto_loop_forever(mosq, 5, 100);
        // mosquitto_
    }, this->mosquitto);

    main.join();
}

void client::subscribe(std::string topic, int qos, std::function<void()> callback) {

    // subscribe to a topic.
    mosquitto_subscribe(this->mosquitto, NULL, topic.c_str(), qos);
}

//Client::~Client() {
//
//    //
//    mosquitto_destroy(this->m_mosq);
//
//    // call to free resources associated with the library.
//    mosquitto_lib_cleanup();
//}
