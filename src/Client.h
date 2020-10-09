#include "mosquitto.h"
#include <string>

#ifndef CLIENT_H
#define CLIENT_H

class Client {
private:

    std::string m_id;
    std::string m_topic;
    std::string m_host;
    int m_port;

    struct mosquitto *m_mosq;

    static const int keepalive = 60;

public:
    Client(std::string host, int port);

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
