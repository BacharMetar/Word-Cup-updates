package bgu.spl.net.impl.stomp;

import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;

import javax.lang.model.util.ElementScanner6;

public class Frame {
    private String stompCommand;
    private List<Header> headers;
    private String frameBody;

    public Frame(String stompCommand, List<Header> headers, String frameBody) {
        this.stompCommand = stompCommand;
        this.headers = headers;
        this.frameBody = frameBody;
    }

    public List<Header> getHeaders() {
        return headers;
    }

    public String getFrameBody() {
        return frameBody;
    }

    public String getStompCommand() {
        return stompCommand;
    }

    public void addHeader(String headerName, String headerValue) {
        headers.add(new Header(headerName, headerValue));
    }

    public String makeEncodeString() {
        String result = stompCommand;
        for (Header header : headers)
         result += "\n" + header.getName() + ":" + header.getValue();
     result += "\n\n" + frameBody;
        return result;
    }

    public  String makeDecodedFrameSend() {    
        String result ="";
        boolean flag = false;
        
        result += "MESSAGE";
        
        for (Header header : headers)
        {
                if(header.getName().equals("destination") )
                {   
                    flag = false;
                result += "\n" + header.getName() + ":" + header.getValue();
                    flag = true;
                }
                if(header.getName().equals("general game updates") )
                {   
                    flag = false;
                 result += "\n" + header.getName() + ":" + header.getValue();
                    flag = true;
                }
                else if(header.getName().equals("team a updates") )
                {   
                    flag = false;
                 result += "\n" + header.getName() + ":" + header.getValue();
                    flag = true;
                }
                else if(header.getName().equals("team b updates") )
                {   
                    flag = false;
                 result += "\n" + header.getName() + ":" + header.getValue();
                    flag = true;
                }
                else if(header.getName().equals("description") )
                {   
                    flag = false;
                 result += "\n" + header.getName() + ":" + header.getValue();                    
                }
                else if(flag)
                {
                 result += "\t";
                 result += "\t";
                 result += "\n" + header.getName() + ":" + header.getValue();
                }
                else
                {
                 result += "\n" + header.getName() + ":" + header.getValue();
 
                }
              
            }
     result += "\n\n" + frameBody;
        return result;
      
}

    public static Frame makeDecodedFrame(String frameString) {
        if (frameString == null) {
            return null;
        }
        String stompCommand = "";
        List<Header> headersList = new LinkedList<>();
        String frameBody = "";

        Scanner scanner = new Scanner(frameString);
        if (scanner.hasNextLine())
            stompCommand = scanner.nextLine();
        while (scanner.hasNextLine()) {
            String line = scanner.nextLine();
            if (line.equals(""))
                break;
            String[] parts = line.split(":");
            if(parts.length == 1)
                headersList.add(new Header(parts[0], "")); 
            else
                headersList.add(new Header(parts[0], parts[1]));   

            
        }
        if (scanner.hasNextLine())
            frameBody = scanner.nextLine();
        scanner.close();

        return new Frame(stompCommand, headersList, frameBody);
    }



    public void removeHeader(String topicId) {
        for (Header header : headers) {
            if (header.getName() == topicId) {
                headers.remove(header);
                break;
            }
        }
    }

    public void setStompCommandToMessage()
    {
        this.stompCommand = "MESSAGE";
    }

    
}
