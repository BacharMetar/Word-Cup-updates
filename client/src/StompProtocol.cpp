#include "StompProtocol.h"
#include <iostream>
#include "event.h"
#include "map"
#include <sstream>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;



StompProtocol::StompProtocol(Database &database) : database(database), isTerminated(false) {}

    /*
    create a frame format from a string thread took  from the queue
    the function is called from SocketWriter Thread 
    */
    vector<string> StompProtocol::createFrame(string msg)
    {
        vector<string> result;

        if (msg.size() != 0) {
        vector<string> wordsVector = splitToWordsToVector(msg, ' ');
        if (wordsVector.size() > 0) {
            string command = wordsVector.at(0);
            if (command == "login") 
            {
                printError("user already logged-in");
            } 
            else if (command == "join") 
                {
                 if (wordsVector.size() == 2) {
                    string gameName = wordsVector.at(1);
                    int subscriptionId = database.subscribeGame(gameName);
                    if (subscriptionId > 0) {//if the user not subsrbed already
                        int receiptId = database.addReciptToMessage("Joined game " + gameName);
                        vector<pair<string, string>> headers;// creating a vector of headers
                        //start creating the frame format
                        pair<string, string> destinationHeader("destination", gameName);
                        pair<string, string> idHeader("id", to_string(subscriptionId));
                        pair<string, string> receiptIdHeader("receipt", to_string(receiptId));

                        headers.push_back(destinationHeader);
                        headers.push_back(idHeader);
                        headers.push_back(receiptIdHeader);

                        result.push_back(encodeFrame("SUBSCRIBE", headers, ""));
                        return result;

                        } else {
                            printError("user already subscribed to this genre");
                        }
                     }
                        else{
                            printError("illegal join command!!");

                        }
                }
            
             else if (command == "exit") {
                if (wordsVector.size() == 2) {
                    string gameName = wordsVector.at(1);
                    int subscriptionId = database.unsubscribeGame(gameName);
                    if (subscriptionId > 0) {
                        int receiptId = database.addReciptToMessage("Exited Game " + gameName);
                        vector<pair<string, string>> headers;
                         //start creating the frame format

                        pair<string, string> idHeader("id", to_string(subscriptionId));
                        // pair<string, string> destinationHeader("destination", gameName);
                        pair<string, string> receiptIdHeader("receipt", to_string(receiptId));
                        headers.push_back(idHeader);
                        // headers.push_back(destinationHeader);
                        headers.push_back(receiptIdHeader);

                        result.push_back(encodeFrame("UNSUBSCRIBE", headers, ""));
                        return result;

                    } else {
                        printError("user wasn't subscribed to this genre");
                    }
                }
                 else{
                     printError("illegal Exit command!!");

                }
            }
            
             else if (command == "logout") {
                if (wordsVector.size() == 1) {
                    int receiptId = database.addReciptToMessage("DISCONNECT");
                    vector<pair<string, string>> headers;
                    //start creating the frame format

                    pair<string, string> receiptIdHeader("receipt", to_string(receiptId));
                    headers.push_back(receiptIdHeader);

                    result.push_back(encodeFrame("DISCONNECT", headers, ""));
                    return result;

                } else {
                    printError("illegal LogOut command");
                }
            }
            
             else if (command == "report") {
                string jsonPath = wordsVector.at(1);
                //creating the object from the json File
                names_and_events nne = parseEventsFile(jsonPath);
                //crating a vector with all event from the jsonFile
                vector<Event> events = nne.events;
                string teamA = nne.team_a_name;
                string teamB = nne.team_b_name;
                string gameName = teamA + "_" + teamB; 
                for(Event event : events)
                {
                    vector<pair<string, string>> headers;
                    pair<string, string> destinationHeader("destination", gameName);

                    pair<string, string> user ("user",database.getUserName());
                    pair<string, string> teamAH ("team a",teamA); 
                    pair<string, string> teamBH ("team b",teamB);  
                    pair<string, string> eventName ("eventName",event.get_name());
                    pair<string, string> time ("time",to_string(event.get_time()));
                    pair<string, string> generalGameUpdates ("general game updates","");
                    headers.push_back(destinationHeader);
                    headers.push_back(user);
                    headers.push_back(eventName);
                    headers.push_back(time);
                    headers.push_back(generalGameUpdates);
                    map<string,string> generalUpdates = event.get_game_updates();

                    for(const auto &update : generalUpdates)
                    {
                         pair<string, string> updateP(update.first,update.second);
                         headers.push_back(updateP);

                    }

                    pair<string, string> teamAUpdates ("team a updates","");
                    headers.push_back(teamAUpdates);
                    map<string,string> teamAUpd = event.get_team_a_updates();   
                    for(const auto &update : teamAUpd)
                    {
                         pair<string, string> updateP(update.first,update.second);
                         headers.push_back(updateP);

                    }

                    pair<string, string> teamBUpdates ("team b updates","");
                    headers.push_back(teamBUpdates);
                    map<string,string> teamBUpd = event.get_team_b_updates();   
                    for(const auto &update : teamBUpd)
                    {
                         pair<string, string> updateP(update.first,update.second);
                         headers.push_back(updateP);

                    }

                     pair<string, string> decriptionH ("description",event.get_discription());
                     headers.push_back(decriptionH);
                    //  string frameBody = event.get_discription();
                    string frameBody = "";
                    result.push_back(encodeFrame("SEND", headers, frameBody));
                }

                return result;
            
             }  
             else if (command == "summary") {
                //TODO:IMPLEMENT!!!!!!!
                string gameName = wordsVector.at(1);
                string userName = wordsVector.at(2);

                string summary ="";
                summary = database.getSummary(gameName,userName);
                //
                string filePath = wordsVector.at(3);
                //TODO: write summary into file
                string report = summary;
                string path = filePath;
                // json reportJson = json::parse(report);

                // std::ofstream outFile;
                // outFile.open(path);
                // outFile << reportJson.dump();
                // outFile.close();

                std::ofstream outFile;
                outFile.open(path);
                outFile << report;
                outFile.close();
                

                

              }
         }
        }

         return result;
    }

    void StompProtocol::proccesMessageFromServer(string msg)
    {
      vector<string> lines = splitToWordsToVector(msg, '\n');
        if (lines.size() == 0) {
            return;
        }

        string stompCommand = lines.at(0);
        vector<pair<string, string>> headers;
        string frameBody;
        int i = 1;


        while ((unsigned)i < lines.size() && lines.at(i).length() != 0) { //headers maker
            vector<string> headerNameAndValue = splitToWordsToVector(lines.at(i), ':');
            pair<string, string> header(headerNameAndValue.at(0), headerNameAndValue.at(1));
            headers.push_back(header);
            i++;
        }
        i++;

        frameBody = "";
        if ((unsigned)i < lines.size()) {
            frameBody = lines.at(i);
        }


        if (stompCommand == "CONNECTED") {
            cout << "Login successful." << endl;
        } 
        else if (stompCommand == "RECEIPT") {
            string receiptId;
            for (auto &header : headers) {
                if (header.first == "receipt-id") {
                    receiptId = header.second;
                }
            }
            string command = database.getCommandFromReceipt(stoi(receiptId));
            if (command == "DISCONNECT") {
                terminate();
            } 
            else if (command == "DISCONNECT") {
                terminate();
            }

            else {
                cout << command << endl;
            }

        } 
        
            //else it is a report
            else if (stompCommand == "MESSAGE") {
                //printing the report on client side
                string frame = makeDecodedFrameSend(headers,frameBody);

                cout << frame << endl;
                //insert the data in frame to the realted tabkes in database
                insertFrameData(frame);
                }
            else if (stompCommand == "ERROR") {
                cout << frameBody << endl;
            }

        }
    


    void StompProtocol::printError(string error) {
        cout << error << endl;
    }


    string StompProtocol::encodeFrame(string stompCommand, vector<pair<string, string>> headers, string frameBody) {
    string ret = stompCommand;
    for (auto &header : headers)
        ret += "\n" + header.first + ":" + header.second;
    ret += "\n\n" + frameBody;

    return ret;
}

/*"The game has started! What an exciting evening!
split a string to vector of string
*/
vector<string> StompProtocol::splitToWordsToVector(string str, char splitter) {
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

   string StompProtocol:: makeDecodedFrameSend(vector<pair<string,string>> headers, string frameBody) {    
        string result ="";
        bool flag = false;
        
        result += "MESSAGE";
        
        string destination ="";
        string user = "";
        

        for (pair<string,string> header : headers)
        {
            string name = getName(header);
            string value = getValue(header);

                if( name == "destination") 
                {   
                    flag = false;
                result += "\n" + name + ":" + value;
                    flag = true;
                }

                 else if( name == "user") 
                {   
                    flag = false;
                result += "\n" + name + ":" + value;
                    flag = true;
                }

                else if(name == "general game updates")
                {   
                    flag = false;
                 result += "\n" + name + ":" + value;
                    flag = true;
                }
                else if( name == "team a updates") 
                {   
                    flag = false;
                 result += "\n" + name + ":" + value;
                    flag = true;
                }
                else if(name == "team b updates") 
                {   
                    flag = false;
                 result += "\n" + name + ":" + value;
                    flag = true;
                }
                else if(name == "description") 
                {   
                    flag = false;
                 result += "\n" + name+ ":" + value;                    
                }
                else if(flag)
                {
                 result += "\t";
                 result += "\t";
                 result += "\n" + name + ":" + value;
                }
                else
                {
                 result += "\n" + name + ":" + value;
 
                }
              
            }
     result += "\n\n" + frameBody;
        return result;
      
}

string StompProtocol:: getName(pair<string,string> header){

        return header.first;
  }

string StompProtocol:: getValue(pair<string,string> header){
        
        return header.second;
  }

  void StompProtocol::insertFrameData(string frame)
  {
    vector<string> lines = splitToWordsToVector(frame, '\n');

        string stompCommand = lines.at(0);
        vector<pair<string, string>> headers;
        string frameBody;
        int i = 1;

     while ((unsigned)i < lines.size() && lines.at(i).length() != 0) { //headers maker
            vector<string> headerNameAndValue = splitToWordsToVector(lines.at(i), ':');
            pair<string, string> header(headerNameAndValue.at(0), headerNameAndValue.at(1));            
            headers.push_back(header);
            i++;
        }
        i++;

        frameBody = "";
        if ((unsigned)i < lines.size()) {
            frameBody = lines.at(i);
        }

        string destination = headers.at(0).second;
        string userName = headers.at(1).second;
        string eventName =headers.at(4).second;
        string time = headers.at(5).second;
       

        i = 0;

        while(headers.at(i).first != "general game updates")
        {
            i++;
        }
        i++;

        while(headers.at(i).first != "team a updates")
        {
            string path = "general game updates";
            database.addEvents(destination,userName,path,headers.at(i));
            i++;
        }
        i++;
         while(headers.at(i).first != "team b updates")
        {
            string path = "team a updates";
            database.addEvents(destination,userName,path,headers.at(i));
            i++;
        }
        i++;
         while(headers.at(i).first != "description")
        {
            string path = "team b updates";
            database.addEvents(destination,userName,path,headers.at(i));
            i++;
        }

                //frameBody not Description header

            pair<string,string> addEventPair;
            addEventPair.first = time;
            addEventPair.second = eventName;
            database.addDescription(destination,userName,addEventPair, frameBody);


  }
void StompProtocol::terminate()
{
    isTerminated = true;

}

bool StompProtocol::shouldTerminate()
{
    return isTerminated;

}


    
