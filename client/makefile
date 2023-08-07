CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lpthread

all: StompWCIClient

EchoClient: bin/ConnectionHandler.o bin/echoClient.o
	g++ -o bin/EchoClient bin/ConnectionHandler.o bin/echoClient.o $(LDFLAGS)

StompWCIClient: bin/ConnectionHandler.o bin/StompClient.o bin/Database.o bin/event.o bin/SocketReader.o  bin/SocketWriter.o  bin/StompProtocol.o 
	g++ -o bin/StompWCIClient bin/ConnectionHandler.o bin/StompClient.o bin/Database.o bin/event.o bin/SocketReader.o  bin/SocketWriter.o  bin/StompProtocol.o  $(LDFLAGS)


StompClient: bin/ConnectionHandler.o bin/StompClient.o bin/Database.o bin/event.o bin/SocketReader.o  bin/SocketWriter.o  bin/StompProtocol.o 
	g++ -o  bin/StompWCIClient bin/ConnectionHandler.o bin/StompClient.o bin/Database.o bin/event.o bin/SocketReader.o  bin/SocketWriter.o  bin/StompProtocol.o  $(LDFLAGS)

# StompClient: bin/ConnectionHandler.o bin/StompClient.o
# 	g++ -o bin/StompClient bin/ConnectionHandler.o bin/StompClient.o $(LDFLAGS)

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/echoClient.o: src/echoClient.cpp
	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp

bin/event.o: src/event.cpp
	g++ $(CFLAGS) -o bin/event.o src/event.cpp

bin/Database.o: src/Database.cpp
	g++ $(CFLAGS) -o bin/Database.o src/Database.cpp

bin/StompClient.o: src/StompClient.cpp
	g++ $(CFLAGS) -o bin/StompClient.o src/StompClient.cpp

bin/SocketReader.o: src/SocketReader.cpp
	g++ $(CFLAGS) -o bin/SocketReader.o src/SocketReader.cpp
	
bin/SocketWriter.o: src/readerWriterThreads/SocketWriter.cpp
	g++ $(CFLAGS) -o bin/SocketWriter.o src/readerWriterThreads/SocketWriter.cpp

	
bin/StompProtocol.o: src/StompProtocol.cpp
	g++ $(CFLAGS) -o bin/StompProtocol.o src/StompProtocol.cpp

		
# bin/ConnectionHandler.o: src/ConnectionHandler.cpp
# 	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp
	
.PHONY: clean
clean:
	rm -f bin/*
	