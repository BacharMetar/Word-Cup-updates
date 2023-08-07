
#include "Database.h"
#include <iostream>
#include <map>
#include <vector>
#include <utility>


Database::Database(string userName) : messageLock(),messageQueue(),subscriptionIdToGameLock(),subscriptionIdToGame(),subscriptionId(0),receiptId(0), userName(userName)
 {

 }


/*
//subscribe the user to the game, if he is not alreafy subscribed
*/
int Database::subscribeGame(string game) {
    subscriptionIdToGameLock.lock();
    if (getKeyByValue(game) < 0) { // not subscribed to topic already
        subscriptionId += 1;
        subscriptionIdToGame.emplace(subscriptionId, game);
        subscriptionIdToGameLock.unlock();
        return subscriptionId;
    }
    subscriptionIdToGameLock.unlock();
    return -1;
}



/*
return the subscription id of a specific game
*/
int Database::getKeyByValue(string val) {
    int toReturn = -1;
    for (auto &game: subscriptionIdToGame) {
        if (game.second == val) {
            toReturn = game.first;
        }
    }
    return toReturn;
}

/*
unsubscribe the user to the game, if he is alreafy subscribed
*/
int Database::unsubscribeGame(string game) {
    subscriptionIdToGameLock.lock();
    int subscriptionId = getKeyByValue(game);
    if (subscriptionId > 0) {
        subscriptionIdToGame.erase(subscriptionId);
        subscriptionIdToGameLock.unlock();
        return subscriptionId;
    }
    subscriptionIdToGameLock.unlock();
    return -1;
}

string Database::getUserName()
{
    return userName;
}

/*
return a game name by its subscriptionId
*/
 string Database::getGameBySubscriptionId(int subscriptionId)
 {
    subscriptionIdToGameLock.lock();
    string game = subscriptionIdToGame.at(subscriptionId);
    subscriptionIdToGameLock.unlock();
    return game;
 }

/*
//adding a message from the cient to the message Queue
*/
    void Database::addMessage(string msg)
    {
        messageLock.lock();
        messageQueue.emplace(msg);
        messageLock.unlock();
    }
/*
remove a mmesage form the messageQueue and return it to the user
*/
    string Database::getMassage()
    {
       messageLock.lock();
    if (messageQueue.size() > 0) {
        string msg = messageQueue.front();
        messageQueue.pop();
        messageLock.unlock();
        return msg;
    } else {
        messageLock.unlock();
        return "";
    }
    }

    /*
    check if messageQueue is empty
    */
   bool Database::isMessageQueueEmpty()
   {
        return (messageQueue.size() == 0);

   }

/*
distructor
*/
   Database::~Database() {
    subscriptionIdToGame.clear();
    events.clear();
    descriptionMap.clear();
    while (!messageQueue.empty()) {
        messageQueue.pop();
    }
}
/*
// adding a message recived from userReported, to the specific game
// */
// void Database::addMessageToGame(string gameName, string userreported,string msg)
// {

//     gameMessages[gameName][userreported].push_back(msg);

// }
/*
// return all user messages on a specific game
// */
// vector<string> Database::getMessagesFromUserOnGame(string gameName,string userName)
// {
//     vector<string> result = gameMessages[gameName][userName];
//     return result;
// }

/*
adding amessage command and generate a recipt and return it
*/

  int Database::addReciptToMessage(string messageCommand)
  {
    reciptToMessageLock.lock();
    receiptId+= 1;
    reciptToMessage.emplace(receiptId, messageCommand);
    reciptToMessageLock.unlock();
    return receiptId;
  }

/*
reurn the command type by his recipt id
*/
  string Database::getCommandFromReceipt(int receipt)
  {
    reciptToMessageLock.lock();
    string result = "no available receipt";
    if (reciptToMessage.find(receipt) != reciptToMessage.end())
        result = reciptToMessage.at(receipt);
    reciptToMessageLock.unlock();
    return result;
  }
    //add value to events map
  void Database::addEvents(string destination, string userName,string event, pair<string,string> data )
  {
    events[destination][userName][event].push_back(data);
  }

//add value to decriptionMap
  void Database::addDescription(string destination,string userName,pair<string,string> addEventPair, string description)
  {
    pair<pair<string,string>,string> newPair;
    newPair.first = addEventPair;
    newPair.second = description;

    descriptionMap[destination][userName].push_back(newPair);


  }


map<string,vector<pair<string,string>>> Database::getdict(string destination,string user)
{
    return events.at(destination).at(user);

}
/*
return a string as a summary
*/
string Database::getSummary(string destination, string userName)
{
    string result = "";
    // auto mapKey = events[destination][userName];
    // map<string,vector<pair<string,string>>> mapKey = events[destination][userName];
    map<string,vector<pair<string,string>>> mapKey = getdict(destination,userName);
    // vector<string> keys = getKeysString(mapKey);
    vector<string> keys = {"general game updates","team a updates","team b updates"};
    for(string eventName :keys )
    // for(pair<string,string> p : events[destination][userName] )
    {
        result += eventName + ":";
        result += "\n";

        vector<pair<string,string>> pairs = events[destination][userName][eventName];
        for(pair<string,string> p : pairs)
        {     
            result +="\t" ;
            result +="\t" + p.first + " : " + p.second;
            result += "\n";
        }
    }

        //TODO: get the description map vector of pairs and add it to thes result string
        result += "Game event reports :";
        result += "\n";
    

    // vector<pair<pair<string,string>,string>> pair = destinationMap[description][userName];

    // map<string,vector<pair<pair<string,string>,string>>> usersmap = getUsersMap(descriptionMap);
    // map<string,vector<pair<pair<string,string>,string>>> usermap = descriptionMap.at(destination);

    // for(pair<pair<string,string>,string> p : destinationMap.at(description).at(userName) )
    // {

    // }    
   
    return result;
}

vector<string> Database::getKeysString( map<string,vector<pair<string,string>>> mapKey)
{
        vector<std::string> keys;
        for(auto it = mapKey.begin(); it != mapKey.end(); ++it) keys.push_back(it->first);
        return keys;
}

// vector<pair<pair<string,string>,string>> Database::getPairs(map<string,map<string,map<string,vector<pair<string,string>>>>>mapPair , const string description ,const string username )
// {
//  for (auto const [description, inner_map] : mapPair) {
//     for (auto const [username, pairs] : inner_map) {
//         // do something with the description, username and pairs
//         // ...

//     }


// }
// }

map<string,vector<pair<pair<string,string>,string>>> Database::getUsersMap (map<string,map<string,vector<pair<pair<string,string>,string>>>> descriptionMap, string gameName)
    {
        return descriptionMap.at(gameName);
    }



  


