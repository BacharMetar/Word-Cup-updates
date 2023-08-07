
// #include <readerWriterThreads/SocketRerader.h>

#include "../include/SocketReader.h"


SocketReader::SocketReader(ConnectionHandler &connectionHandler, Database &database,
                                       StompProtocol &stompProtocol) : connectionHandler(connectionHandler),
                                                                       database(database),
                                                                      stompProtocol(stompProtocol) {}

void SocketReader::run() {
    while (!stompProtocol.shouldTerminate()) {
        string msg = "";
        if (connectionHandler.getLine(msg)) {
            stompProtocol.proccesMessageFromServer(msg);
        }
    }
}