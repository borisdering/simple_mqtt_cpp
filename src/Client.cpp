#include <string>
#include <iostream>
#include <thread>
#include "mosquitto.h"
#include "Client.h"

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
static void onConnect(struct mosquitto* mosq, void *userdata, int rc) {

    // due to the fact that we receive the context
    // we try to call the actual callback.
    class Client *c = (class Client*) userdata;

    std::function<void()> f_display_42 = []() { std::cout << "subscribed" << std::endl; };

    c->subscribe("#", 0, f_display_42);
}

/**
 * Simple C static on message callback wrapper.
 * @param mosq
 * @param userdata
 * @param message
 */
void onMessage(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {

    // due to the fact that we receive the context
    // we try to call the actual callback.
    std::string payload((char*) message->payload);

    // need to dispatch to node js context 

    std::cout << "on message: " << payload << std::endl;
}

Client::Client(std::string host, int port) {

    // must be called before any other mosquitto functions.
    int result = mosquitto_lib_init();

    // check for a positive result in this case == 0
    // if (result != 0)
    //     throw new std::runtime_error("unable to initilaize mosquitto library");

    this->m_id = "hackerman";
    this->m_topic = "client";
    this->m_host = host;
    this->m_port = port;

    // create a new mosquitto client instance.
    this->m_mosq = mosquitto_new(this->m_id.c_str(), true, this);

    // Used to tell the library that your application is using threads, but not
    // using <mosquitto_loop_start>. The library operates slightly differently when
    // not in threaded mode in order to simplify its operation. If you are managing
    // your own threads and do not use this function you will experience crashes
    // due to race conditions.
    mosquitto_threaded_set(this->m_mosq, true);

    // connect to an MQTT broker and set on connect callback
    mosquitto_connect_callback_set(this->m_mosq, onConnect);
    
    
    mosquitto_loop_start(this->m_mosq);

    mosquitto_connect_async(this->m_mosq, this->m_host.c_str(), this->m_port, Client::keepalive);

    // Set the message callback. This is called when a message is received from the
    // broker.
    mosquitto_message_callback_set(this->m_mosq, onMessage);
}

/**
 * Start listening to messages. 
 */
void Client::start() {

    std::thread main([](struct mosquitto* mosq){

        // mosquitto_loop_start(mosq);
        // mosquitto_loop_forever(mosq, 5, 100);
        // mosquitto_
    }, this->m_mosq);

    main.join();
}

void Client::subscribe(std::string topic, int qos, std::function<void()> callback) {

    // subscribe to a topic.
    mosquitto_subscribe(this->m_mosq, NULL, topic.c_str(), qos);
}

//Client::~Client() {
//
//    //
//    mosquitto_destroy(this->m_mosq);
//
//    // call to free resources associated with the library.
//    mosquitto_lib_cleanup();
//}
