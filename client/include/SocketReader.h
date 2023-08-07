#include "ConnectionHandler.h"
#include "Database.h"
#include "StompProtocol.h"

#ifndef BOOST_ECHO_CLIENT_SOCKETREADERTHREAD_H
#define BOOST_ECHO_CLIENT_SOCKETREADERTHREAD_H

#endif //BOOST_ECHO_CLIENT_SOCKETREADERTHREAD_H
using namespace std;

class SocketReader {
public:
    SocketReader(ConnectionHandler &connectionHandler, Database &database, StompProtocol &stompProtocol);

    void run();

private:
    ConnectionHandler &connectionHandler;
    Database &database;
    StompProtocol &stompProtocol;
};