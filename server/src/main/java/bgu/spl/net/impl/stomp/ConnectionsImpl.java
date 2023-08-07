package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.BlockingConnectionHandler;
import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.NonBlockingConnectionHandler;
// import bgu.spl.net.srv.Reactor;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Vector;
// import java.util.Map;
// import java.util.concurrent.ConcurrentHashMap;
// import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ConcurrentHashMap;

public class ConnectionsImpl<T> implements Connections<T> {

    private Map<Integer, User> loggedInUsers;// hashMap for all logged in users <Id,user>
    private Vector<String> exists;
    private Map<Integer, User> allExistingUsers;// hashMap for all registers users <Id,user>
    private Map<String, Integer> allExistingUsersByName;// hashMap for all registers users <Id,user>

    private Map<String, LinkedList<userAndSubscription>> topicsAndUsers;// hashMap for each topic a list of all the
                                                                            // users that subscribed to this topic
                                                                            // <topic(Gamename),List of usersAndSubscripton>
                                                                            
    private Map<Integer, ConnectionHandler<T>> clientIdToConnectionHandler; // hashMap for eac client id his
                                                                                // connectionhandler, using the
                                                                                // interface because it migh be both
                                                                                // Blockin/NonBlocking connectionHandlar
    private ConnectionsImpl() {
        // connectionIdCounter = new AtomicInteger(1);
        // msgIdCounter = new AtomicInteger(1);
        loggedInUsers = new ConcurrentHashMap<>();
        allExistingUsers = new ConcurrentHashMap<>();
        topicsAndUsers = new ConcurrentHashMap<>();
        clientIdToConnectionHandler = new ConcurrentHashMap<>();
        exists = new Vector<String>();
        allExistingUsersByName = new ConcurrentHashMap<>();

    }
    public Map<Integer, ConnectionHandler<T>> getClientIdToConnectionHandler ()
    {
        return clientIdToConnectionHandler;
    }

    // creating a private instance of the class connections
    private static class ConnectionsHolder {
        // private static ConnectionsImpl<Frame> connectionsObject = new ConnectionsImpl<>();
        // private static ConnectionsImpl <T> connectionsObject = new
        private static ConnectionsImpl<String> connectionsObject = new ConnectionsImpl<>();

    }
    

    // public static ConnectionsImpl <Frame> getInstance() {
    public static ConnectionsImpl<String> getInstance() {
        return ConnectionsHolder.connectionsObject;
    }

    // sends a message T to client represented by the given connectionId.
    @Override
    public boolean send(int connectionId, T msg) {
        if (clientIdToConnectionHandler.containsKey(connectionId)) {
            clientIdToConnectionHandler.get(connectionId).send(msg);
            return true;
        }
        return false;

    }

    // Sends a message T to clients subscribed to channel.
    public void send(String channel, T msg) {
        //check if channel is legal
        if(topicsAndUsers.containsKey(channel))
        {
            LinkedList<userAndSubscription> list = topicsAndUsers.get(channel);
            //for each user iin the channel send msg
            for(userAndSubscription userAndSub : list)
            {
             int connectionId =   userAndSub.getUser().getConnectionId();
             ConnectionHandler <T> handler = clientIdToConnectionHandler.get(connectionId);
             handler.send(msg);
             //need to add the subscription header?????

            }


        }
        

    }

    @Override

    // Removes an active client connectionId from the map
    public void disconnect(int connectionId) {
        User user = allExistingUsers.get(connectionId);
        loggedInUsers.remove(connectionId); // remove the user from the logged in users list

        // all user's topics
        LinkedList<String> userTopics = user.getTopics();
        // for each topic remove the user.
        for (String topic : userTopics) {
            removeUserFromTopic(user, topic);
        }
        // remove all user's topics- not future communication is needed!
        user.removeAllTopics();

    }

    // subscribe a client to a game channel
    /*
     * destination - game_name
     * connectionId - client's id
     * subscriptionId - clien id for each game
     */
    public void subscribeChannel(String destination, int connectionId, int _subscriptionId) {
        User user = allExistingUsers.get(connectionId);
        userAndSubscription userAndSub = new userAndSubscription(user, _subscriptionId);
        String subscriptionId = String.valueOf(_subscriptionId);
        // if the topic already exist
        if (topicsAndUsers.containsKey((destination))) {
            topicsAndUsers.get(destination).add(userAndSub);
            user.subscribe(destination, subscriptionId);
        }
        // else the topic doesnt exist
        else {

            topicsAndUsers.put(destination, new LinkedList<userAndSubscription>()); // create new key with empty list
            topicsAndUsers.get(destination).add(userAndSub);
            user.subscribe(destination, subscriptionId);

        }

    }

    // unsunscribe a client from a game channel
    public void unsubscribe(int connectionId, String subscriptionId) {
        User user = allExistingUsers.get(connectionId);
        String destination = user.getTopicBySubscriptionId(subscriptionId);

        removeUserFromTopic(user, destination);
        // remove the topic fron the use hashmap
        user.removeTopic(subscriptionId);

    }

    // connect a user to the system (login-if exist, signup-if not exist)
    public String connectToUser(String userName, String password, int connectionId) {
        // boolean isExist = allExistingUsers.containsKey(connectionId); // check if the user loggedIn
        boolean isExist = allExistingUsersByName.containsKey(userName);
        if (isExist) {
            int con = allExistingUsersByName.get(userName);
            boolean isLoggedIn = loggedInUsers.containsKey(con); // check if the user loggedIn
            // exist and loggedIn
            if (isLoggedIn) {
                return "User Is Already Logged In";
            }
            // exist and not loggedIn
            else {
                String checkPass = allExistingUsers.get(con).getPassword();
                String checkUsername = allExistingUsers.get(con).getUserName();
                // if credentials are correct
                if (checkPass.equals(password) && checkUsername.equals(userName)) {
                    loggedInUsers.put(con, allExistingUsers.get(con));
                    return "Login successful";
                }
                // credentials are wrong
                else {
                    return "Wrong password";
                }

            }

        }
        // not exist - create new user
        else {
            User user = new User(userName, password, connectionId);
            allExistingUsers.put(connectionId, user);
            allExistingUsersByName.put(userName, connectionId);

            loggedInUsers.put(connectionId, user);
            exists.add(userName);
            return "Login successful";

        }
    }

    // remove a user from specific topic
    private void removeUserFromTopic(User user, String destination) {
        LinkedList<userAndSubscription> list = topicsAndUsers.get(destination);
        // find the right user to delete
        for (userAndSubscription u : list) {
            if (u.getUser().equals(user)) {
                topicsAndUsers.get(destination).remove(u);
                break;
            }
        }
    }
    
 public void addConnectionHandler(int _connectionId, NonBlockingConnectionHandler<T> handler) {
        clientIdToConnectionHandler.put(_connectionId, handler);
    }
    public void addConnectionHandler(int connectionId, BlockingConnectionHandler<T> handler) {
        clientIdToConnectionHandler.put(connectionId, handler);
    }

   
}