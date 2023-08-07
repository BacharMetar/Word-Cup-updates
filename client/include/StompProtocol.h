#pragma once

#include "ConnectionHandler.h"



#include "Database.h"

#ifndef BOOST_ECHO_CLIENT_STOMPPROTOCOL_H
#define BOOST_ECHO_CLIENT_STOMPPROTOCOL_H

// TODO: implement the STOMP protocol
class StompProtocol {
public:
    StompProtocol(Database &database);

    vector<string> createFrame(string msg);

    void proccesMessageFromServer(string msg);

    string getBookName(vector<string> commandLine, int startIndex);

    string encodeFrame(string stompCommand, vector<pair<string, string>> headers, string frameBody);

    static vector<string> splitToWordsToVector(string str, char splitter);

    void terminate();

    bool shouldTerminate();

    string makeDecodedFrameSend(vector<pair<string,string>>, string frameBody) ;

    string getName(pair<string,string> header);

    string getValue(pair<string,string> header);

    void insertFrameData(string frame);

private:
    Database &database;
    bool isTerminated;

    void printError(string errorMsg);
};

#endif //BOOST_ECHO_CLIENT_STOMPPROTOCOL_H
