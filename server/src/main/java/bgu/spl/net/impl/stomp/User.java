package bgu.spl.net.impl.stomp;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
// import java.util.List;
// import java.util.Map;
// import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentHashMap;

public class User {
    private String userName;
    private String password;
    private int connectionId;
    private Map<String, String> subscriptionAndTopic; //HashMap to all user subscription games <String subscriptionId,String topic>

    //each user has a unique userName, password and unique connectionId
    public User(String userName, String userPasscode, int connectionId) {
        this.userName = userName;
        this.password = userPasscode;
        this.connectionId = connectionId;
        subscriptionAndTopic = new ConcurrentHashMap<>();
    }

    //getters
    //------------------------------------
    public String getUserName() {
        return userName;
    }

    public String getPassword() {
        return password;
    }

    public int getConnectionId() {
        return connectionId;
    }

    /*
     * return a list with all user's topics
     */
    public LinkedList <String> getTopics()
    {
        LinkedList <String> result = new LinkedList<>();
        // adding all user's topics to a list
        for(String topic : subscriptionAndTopic.values()){
            result.add(topic);
        }


        return result;

    }

    //setters
    //-------------------------------------------------
    public void setConnectionId(int connectionId) {
        this.connectionId = connectionId;
    }

    public void subscribe(String destination, String TopicId)
    {
        subscriptionAndTopic.put(TopicId,destination );

    }
    
    public void removeAllTopics()
    {
        this.subscriptionAndTopic = new HashMap<>();
    }

    //return the topic of thr current subscriptionId
    public String getTopicBySubscriptionId(String subscriptionId)
    {
        return subscriptionAndTopic.get(subscriptionId);
    }

    public void removeTopic(String subscriptionId) {
        
        subscriptionAndTopic.remove(subscriptionId);

    }



}
