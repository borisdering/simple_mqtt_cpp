#include "mosquitto.h"
#include <string>
#include <functional>

#ifndef CLIENT_H
#define CLIENT_H

class client {
private:

    std::string id;
    std::string topic;
    std::string host;
    int port;

    struct mosquitto *mosquitto;

    static const int keepalive = 60;

public:
    client(std::string host, int port);

    void start();

    void publish(std::string message, int qos, bool retained, std::function<void> callback);

    void subscribe(std::string topic, int qos, std::function<void()> callback);

    // void isConnected();

    // void onConnect();
    // void onDisconnect();
    // void onError();

    // ~Client();
};

#endif // !CLIENT_H
