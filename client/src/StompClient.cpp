
// #include "../include/readerWriterThreads/SocketReader.h"

#include "../include/SocketReader.h"
#include "../include/readerWriterThreads/SocketWriter.h"
#include "../include/StompProtocol.h"
#include <thread>
#include <string>

void start(ConnectionHandler &connectionHandler, string userName, string passcode, string host);

vector<string> splitToWordsToVector(string str, char splitter);

string createFrameString(string stompCommand, vector<pair<string, string>> headers, string frameBody);

int main(int argc, char *argv[]) {

 string command;
    getline(cin, command);
    while (command != "bye") {
        vector<string> commandWords = splitToWordsToVector(command, ' ');
        if (commandWords.size() == 4 && commandWords.at(0) == "login") {
            vector<string> hostAndPort = splitToWordsToVector(commandWords.at(1), ':');
            string host = hostAndPort.at(0);
            short port = stoi(hostAndPort.at(1));
            string userName = commandWords.at(2);
            string passcode = commandWords.at(3);
            ConnectionHandler connectionHandler(host, port);
            if (!connectionHandler.connect()) {
                cerr << "Could not connect to server" << endl;
            } else {
                vector<pair<string, string>> headers;
                pair<string, string> acceptVersionHeader("accept-version", "1.2");
                pair<string, string> hostHeader("host", host);
                pair<string, string> loginHeader("login", userName);
                pair<string, string> passcodeHeader("passcode", passcode);
                headers.push_back(acceptVersionHeader);
                headers.push_back(hostHeader);
                headers.push_back(loginHeader);
                headers.push_back(passcodeHeader);

                string frameString = createFrameString("CONNECT", headers, "");
                connectionHandler.sendLine(frameString);
                string answer = "";
                connectionHandler.getLine(answer);
                if (answer.find("CONNECTED") != string::npos) {
                    cout << "Login successful" << endl;
                    start(connectionHandler, userName, passcode, host);
                } else if (answer.find("ERROR") != string::npos) {
                    if (answer.find("User already connected") != string::npos)
                        cout << "User already connected" << endl;
                    else if (answer.find("Wrong password") != string::npos)
                        cout << "Wrong password" << endl;
                }
            }
        }
        getline(cin, command);
    }
}

void start(ConnectionHandler &connectionHandler, string userName, string passcode, string host) {
    bool debugMode = false;

    // Database *database = new Database(userName);
    Database database(userName);
    // StompProtocol stompProtocol(*database);
    // SocketReader socketReaderThread(connectionHandler, *database, stompProtocol);
    // SocketWriter socketWriterThread(connectionHandler, *database, stompProtocol, host, userName, passcode);
    StompProtocol stompProtocol(database);
    SocketReader socketReaderThread(connectionHandler, database, stompProtocol);
    SocketWriter socketWriterThread(connectionHandler, database, stompProtocol, host, userName, passcode);

    thread socketReaderThreadRunner(&SocketReader::run, &socketReaderThread);
    thread socketWriterThreadRunner(&SocketWriter::run, &socketWriterThread);

    string command;
    getline(cin, command);
    while (command != "logout") {
        
        // database->addMessage(command);
        database.addMessage(command);
        getline(cin, command);
    }

    // int receiptId = database->addReciptToMessage("DISCONNECT");
     int receiptId = database.addReciptToMessage("DISCONNECT");
    vector<pair<string, string>> headers;
    pair<string, string> receiptIdHeader("receipt", to_string(receiptId));
    headers.push_back(receiptIdHeader);

    string frameString = createFrameString("DISCONNECT", headers, "");
    connectionHandler.sendLine(frameString);
    string answer = "";

    socketReaderThreadRunner.join();
    socketWriterThreadRunner.join();
    connectionHandler.close();
    // delete (database);

    cout << "Disconnected" << endl;
}

vector<string> splitToWordsToVector(string str, char splitter) {
    vector<string> wordsVector;
    string word = "";
    for (auto x : str) {
        if (x == splitter) {
            wordsVector.push_back(word);
            word = "";
        } else {
            word = word + x;
        }
    }
    wordsVector.push_back(word);
    return wordsVector;
}

string createFrameString(string stompCommand, vector<pair<string, string>> headers, string frameBody) {
    string ret = stompCommand;
    for (auto &header : headers)
        ret += "\n" + header.first + ":" + header.second;
    ret += "\n\n" + frameBody;

    return ret;
}