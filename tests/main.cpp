
#include "Client.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {

    std::string id = "test";
    std::string host = "localhost";
    int port = 1883;

    Client client = Client(host, port);
    client.start();
}