#ifndef BOOST_ECHO_CLIENT_DATABASE_H
#define BOOST_ECHO_CLIENT_DATABASE_H


#include <mutex>
#include <vector>
#include <map>
#include <queue>
using namespace std;

class Database{
public:

    Database(string userName);

    int subscribeGame(string game);

    int unsubscribeGame(string game);

    string getUserName();

    string getGameBySubscriptionId(int subscriptionId);

    int getSubscriptionIdByGame(string gameName);

    void addMessage(string msg);

    string getMassage();

    bool isMessageQueueEmpty();

    map<string,vector<string>> getGameMessages();

    void addMessageToGame(string gameName, string userreported,string msg);

    vector<string> getMessagesFromUserOnGame(string gameName,string userName);

    int addReciptToMessage(string messageCommand);

    string getCommandFromReceipt(int receipt);

   ~Database();

    int getKeyByValue(string gameName); // function that return the subcscription if for a specific game

    void addEvents(string destination, string userName,string event, pair<string,string> data);

    string getSummary(string destination, string userName);

    void addDescription(string destination,string userName,pair<string,string> addEventPair, string description);


    vector<string> getKeysString( map<string,vector<pair<string,string>>> mapKey); //return the map keys

    vector<string> splitLineToVector(string str, char splitter); //split a line to vecotr

    // vector<pair<pair<string,string>,string>> getPairs (map<string,vector<pair<pair<string,string>,string>>>);

     vector<pair<pair<string,string>,string>> getPairs( map<string,map<string,map<string,vector<pair<string,string>>>>> MapPairs, const string descriptin, const string userName);
    
    map<string,vector<pair<pair<string,string>,string>>> getUsersMap (  map<string,map<string,vector<pair<pair<string,string>,string>>>> descriptionMap, string gameName);

    map<string,vector<pair<string,string>>> getdict(string destination,string user);

private:

    mutex messageLock;
    queue<string> messageQueue;

    mutex subscriptionIdToGameLock;
    map<int, string> subscriptionIdToGame; // map for each subscriptionId its game

    int subscriptionId;
    int receiptId;

    string userName;

    // map<string,map<string,map<string,vector<pair<string,string>>>>> summuryMap; // <GameName,<userReprted<caption,reprtes>>>>
   
    // map<string,map<string,map<pair<pair<string,string>,string>>>> events;
  
    //all message for each game <GameName,<usserReported, their meessage> >  
    map<string,map<string,map<string,vector<pair<string,string>>>>> events; //<GameName,<usserReported<pair-header,,decription>

    map<string,map<string,vector<pair<pair<string,string>,string>>>> descriptionMap; //<<GameName,<username<reports>>>
    mutex reciptToMessageLock;
    map<int, string> reciptToMessage; // for each reipt the message that was sent

    // int getKeyByValue(string gameName); // function that return the subcscription if for a specific game
};

#endif //BOOST_ECHO_CLIENT_DATABASE_H
