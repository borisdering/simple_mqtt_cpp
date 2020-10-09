


g++ -shared -Wall -Werror -fpic -Wunused-value client.cpp -lmosquitto -o client.so

g++ -Wall -fpic -Wunused-value main.cpp Client.cpp -lmosquitto -o main && ./main


g++ -Wall -fpic -Wunused-value -Wno-deprecated-declarations main.cpp Client.cpp -lmosquitto -o main && ./main
g++ -Wall -fpic -Wunused-value -Wno-deprecated-declarations main.cpp Client.cpp -lmosquittopp -o main && ./main