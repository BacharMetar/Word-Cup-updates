package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        if (args[1].equals("reactor")){
            System.out.println("reactor started!!!!");
            Server.reactor(10, Short.valueOf(args[0]), () -> new StompMessagingProtocolImpl(), () -> new StompMessageEncoderDecoder<String>()).serve(); //TODO: check num of threads
        }
        else if (args[1].equals("tpc")){
            System.out.println("reactor started!!!!");
            Server.threadPerClient(Short.valueOf(args[0]), ()-> new StompMessagingProtocolImpl(), () -> new StompMessageEncoderDecoder<String>()).serve();
        }
            // Server.threadPerClient(7777, ()-> new StompMessagingProtocolImpl(), () -> new StompMessageEncoderDecoder<String>()).serve();

        
            // System.out.println("reactor started!!!!");
            // Server.reactor(10, 7777, () -> new StompMessagingProtocolImpl(), () -> new StompMessageEncoderDecoder<String>()).serve(); //TODO: check num of threads
        
    }
}
