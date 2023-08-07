package bgu.spl.net.impl.stomp;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.ConnectionHandler;


public class StompMessagingProtocolImpl implements StompMessagingProtocol {
    private int connectionId;
    // private Connections<String> connections;
    private ConnectionsImpl<String> connections;
    private boolean shouldTerminate  = false;
    /**
	 * Used to initiate the current client protocol with it's personal connection ID and the connections implementation
	**/
     public void start(int _connectionId, ConnectionsImpl<String> _connections)
    {
            this.connectionId = _connectionId;
            this.connections = _connections;

    }
    
    public void process(String message)
	{

        Frame frame = Frame.makeDecodedFrame(message);
        //takes all parts of the frame
        // String commanHeader = frame.getStompCommand();
        List<Header> headerList = frame.getHeaders();
        String frameBody = frame.getFrameBody();
        //print the frame to the server
        System.out.println(connectionId);
        for(Header header : headerList)
        {
            System.out.println(header.toString());
        }
        System.out.println(frameBody);
        //Done printing frame

        HashMap<String,String> hashHeaders = getHeadersHashMap(headerList);

        //check if the frame include "recipt" header 
        String receipt = "receipt";
        if(hashHeaders.containsKey(receipt)) 
        {
            //need to reply the client with this receipt
            String receiptVal = hashHeaders.get(receipt);
            Header header = new Header("receipt-id", receiptVal);
            List<Header> headerListReceipt = new LinkedList<Header>();
            headerListReceipt.add(header);

            Frame frameReceipt = new Frame("RECEIPT", headerListReceipt,"");            ConnectionHandler<String> handler = connections.getClientIdToConnectionHandler().get(connectionId);
            String msg = frameReceipt.makeEncodeString();
            handler.send(msg);
        }

        //check all commands options
        //check if command is connect
        if (frame.getStompCommand().equals("CONNECT")) 
        {
            
            String userName = hashHeaders.get("login");
            String password =  hashHeaders.get("passcode");

            String result = connections.connectToUser(userName, password, connectionId);
            //check if result is connect or not
            if(result.equals("Login successful"))
            {
                String versionId = hashHeaders.get("accept - version");
                Header h = new Header ("version",versionId);
                List <Header> connectList = new LinkedList<Header>();
                connectList.add(h);
                Frame frameConnect = new Frame("CONNECTED", connectList, "");
                String frameResult = frameConnect.makeEncodeString();
                ConnectionHandler<String> handlerConnect = connections.getClientIdToConnectionHandler().get(connectionId);
                handlerConnect.send(frameResult);

            }
            //error occured
            else{
                Frame failedConnect = new Frame("ERROR", new LinkedList<>(), result);
                String failedConnectFrame = failedConnect.makeEncodeString();
                ConnectionHandler<String> handlerConnect = connections.getClientIdToConnectionHandler().get(connectionId);
                handlerConnect.send(failedConnectFrame);
                

            }

        }
        //check if command is SUBSCRIBE 
                else if (frame.getStompCommand().equals("SUBSCRIBE")) 
        { 
            String destination = hashHeaders.get("destination");
            String subscriptionID =  hashHeaders.get("id");
            int intSubscriptionId  =Integer.valueOf(subscriptionID);

            connections.subscribeChannel(destination,connectionId,intSubscriptionId);
        }
        //check if command is UNSUBSCRIBE 
        else if (frame.getStompCommand().equals("UNSUBSCRIBE")) 
        { 
            String subscriptionID =  hashHeaders.get("id");
            connections.unsubscribe(connectionId,subscriptionID);
           
        }
        //check if command is send
        else if (frame.getStompCommand().equals("SEND"))
        {
            //take the destination
            // String destination = hashHeaders.get("destination");
            // if (headersMap.containsKey("destination")) {
                // String sendFrame = frame.makeDecodedFrameSend();
                Frame newFrame = Frame.makeDecodedFrame(message);
                newFrame.setStompCommandToMessage();
                String sendFrame = newFrame.makeEncodeString();
                String destination = hashHeaders.get("destination");    
                // headers.add(new Header("message-id", String.valueOf(connectionsInstance.getNewMessageId())));
                // headers.add(new Header("destination", destination));

                // Frame sendFrame = new Frame("MESSAGE", headers, message.getFrameBody());
                connections.send(destination, sendFrame);       
       } 

       else if (frame.getStompCommand().equals("DISCONNECT")) { //todo: remove data from stracture
        connections.disconnect(connectionId);
        shouldTerminate = true;
    }
        

    }
    
    private HashMap<String,String> getHeadersHashMap(List<Header> headerList)
    {
        HashMap<String,String> hashHeaders = new HashMap<>(); // hasmap for all headers<header name, header value>
        for(Header header : headerList)
        {
            hashHeaders.put(header.getName(), header.getValue());
        }
        return hashHeaders;
    }

	/**
     * @return true if the connection should be terminated
     */
    public boolean shouldTerminate()
    {
        return shouldTerminate;
    }
}
