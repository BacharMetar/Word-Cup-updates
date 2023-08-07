package bgu.spl.net.srv;

import bgu.spl.net.impl.stomp.ConnectionsImpl;
// import bgu.spl.net.api.MessageEncoderDecoder;
// import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.impl.stomp.StompMessageEncoderDecoder;
import bgu.spl.net.impl.stomp.StompMessagingProtocolImpl;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.function.Supplier;

public abstract class BaseServer<T> implements Server<T> {

    private final int port;
    private final Supplier<StompMessagingProtocolImpl> protocolFactory;
    private final Supplier<StompMessageEncoderDecoder<T>> encdecFactory;
    private ServerSocket sock;
    private int connectionId;

    private ConnectionsImpl <String> connections;

    public BaseServer(
            int port,
            Supplier<StompMessagingProtocolImpl> protocolFactory,
            Supplier<StompMessageEncoderDecoder<T>> encdecFactory) {

        this.port = port;
        this.protocolFactory = protocolFactory;
        this.encdecFactory = encdecFactory;
		this.sock = null;
        this.connectionId = 1;
        connections = ConnectionsImpl.getInstance();
        
    }

    @Override
    public void serve() {

        try (ServerSocket serverSock = new ServerSocket(port)) {
			System.out.println("Server started");

            this.sock = serverSock; //just to be able to close

            while (!Thread.currentThread().isInterrupted()) {

                Socket clientSock = serverSock.accept();
                StompMessagingProtocolImpl stompMessagingProtocol = protocolFactory.get();
                int _connectionId = connectionId;
                connectionId++;
                stompMessagingProtocol.start(_connectionId, connections);
                BlockingConnectionHandler<T> handler = new BlockingConnectionHandler<>(
                        clientSock,
                        encdecFactory.get(),
                        // protocolFactory.get());
                        stompMessagingProtocol);
                        connections.addConnectionHandler(_connectionId,  (BlockingConnectionHandler<String>) handler);     
                execute(handler);
            }
        } catch (IOException ex) {
        }

        System.out.println("server closed!!!");
    }

    @Override
    public void close() throws IOException {
		if (sock != null)
			sock.close();
    }

    protected abstract void execute(BlockingConnectionHandler<T>  handler);

}
