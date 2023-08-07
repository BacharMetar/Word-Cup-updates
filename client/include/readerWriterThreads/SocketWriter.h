#include "../ConnectionHandler.h"
#include "../Database.h"
#include "../StompProtocol.h"
#include "mutex"

#ifndef BOOST_ECHO_CLIENT_SOCKETWRITERTHREAD_H
#define BOOST_ECHO_CLIENT_SOCKETWRITERTHREAD_H

#endif //BOOST_ECHO_CLIENT_SOCKETWRITERTHREAD_H
using namespace std;

class SocketWriter{
public:
    SocketWriter(ConnectionHandler &connectionHandler, Database &database, StompProtocol &stompProtocol,
                       string host, string userName, string passcode);

    void run();

private:
    ConnectionHandler &connectionHandler;
    Database &database;
    StompProtocol &stompProtocol;
};