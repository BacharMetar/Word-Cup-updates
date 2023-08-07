#include <readerWriterThreads/SocketWriter.h>
#include "Database.h"


SocketWriter::SocketWriter(ConnectionHandler &connectionHandler, Database &database,
                                       StompProtocol &stompProtocol, string host, string userName, string passcode)
        : connectionHandler(connectionHandler), database(database), stompProtocol(stompProtocol) {}

void SocketWriter::run() {
    while (!stompProtocol.shouldTerminate()) {
        if (!database.isMessageQueueEmpty()) {
            string msg = database.getMassage();
            vector<string> framedMsg = stompProtocol.createFrame(msg);
            for(string frame : framedMsg)
            {
                
            if (!frame.empty()) {
                connectionHandler.sendLine(frame);
            }
            }
        }
    }
}







