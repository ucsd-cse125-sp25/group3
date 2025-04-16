On Windows, compile using 
`
g++ main.cpp ClientGame.cpp ClientNetwork.cpp NetworkServices.cpp ServerGame.cpp ServerNetwork.cpp -lws2_32
`

On Mac, compile server using
`
clang++ server.cpp NetworkServices.cpp ServerGame.cpp ServerNetwork.cpp -std=c++1y
`