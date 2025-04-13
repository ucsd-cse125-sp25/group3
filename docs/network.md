Note: These commands are relative to the root directory 
# Building files
Compile the server and client files seperately and indicate location of asio files
g++ -c ./network/*.cpp -I  ./external/asio-1.34.2/include/ -pthread -std=c++11
# Linking files
g++ server.o -o network/server -pthread -std=c++11
g++ client.o -o network/client -pthread -std=c++11
# Run files
./network/server 2000
./network/client 127.0.0.1 2000

Feel free to change 2000 to your desired port #, just make sure client and server are using the same one
127.0.0.1 for localhost as the host address