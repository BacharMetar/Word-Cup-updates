package bgu.spl.net.srv;

// import bgu.spl.net.api.MessageEncoderDecoder;
// import bgu.spl.net.impl.stomp.Frame;
// import bgu.spl.net.impl.stomp.Header;
import bgu.spl.net.impl.stomp.StompMessageEncoderDecoder;
import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;
// import bgu.spl.net.api.MessageEncoderDecoder;
// import bgu.spl.net.api.MessagingProtocol;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;
// import java.util.LinkedList;
// import java.util.List;

public class BlockingConnectionHandler<T> implements Runnable, ConnectionHandler<T> {

    private final StompMessagingProtocolImpl protocol;
    private final StompMessageEncoderDecoder<T> encdec;
    private final Socket sock;
    private BufferedInputStream in;
    private BufferedOutputStream out;
    private volatile boolean connected = true;

    public BlockingConnectionHandler(Socket sock, StompMessageEncoderDecoder<T> reader, StompMessagingProtocolImpl protocol) {
        this.sock = sock;
        this.encdec = reader;
        this.protocol = protocol;
    }

    @Override
    public void run() {
        try (Socket sock = this.sock) { //just for automatic closing
            int read;

            in = new BufferedInputStream(sock.getInputStream());
            out = new BufferedOutputStream(sock.getOutputStream());

            while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
                String nextMessage = encdec.decodeNextByte((byte) read);
                if (nextMessage != null) {
                    // T response = protocol.process(nextMessage);
                    protocol.process(nextMessage);
                    
                    // if (response != null) {
                    //     out.write(encdec.encode(response));
                    //     out.flush();
                    // }
                }
            }

        } catch (IOException ex) {
            ex.printStackTrace();
        }

    }

    @Override
    public void close() throws IOException {
        connected = false;
        sock.close();
    }

    @Override
    public void send(T msg) {
        //IMPLEMENT IF NEEDED
        try {
            out.write(encdec.encode((String) msg));
            out.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
}
