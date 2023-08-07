package bgu.spl.net.impl.stomp;


public class userAndSubscription {
    private User user;
    private int subscriptionId;
    
    public userAndSubscription(User user, int subscriptionId) {
        this.user = user;
        this.subscriptionId = subscriptionId;
    }
    public int getSubscriptionId() {
        return subscriptionId;
    }

    public void setSubscriptionId(int subscriptionId) {
        this.subscriptionId = subscriptionId;
    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    
}
