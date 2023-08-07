# World Cup Update Subscription Service

Stay updated with the World Cup through a community-led update subscription service. This project implements a server and client system that allows users to subscribe to a game channel, report game events, and receive updates from other subscribed users. The server is implemented in Java and provides STOMP (Simple-Text-Oriented-Messaging-Protocol) server services, while the client is implemented in C++.

## Table of Contents
- [General Description](#general-description)
- [STOMP Overview](#stomp-overview)
- [STOMP Server](#stomp-server)
- [STOMP Client](#stomp-client)
- [Implementation Details](#implementation-details)
  - [Server](#server)
  - [Client](#client)
- [Game Event](#game-event)

## General Description
The goal of this project is to create a World Cup update subscription service. Users can subscribe to a game channel and report and receive reports about the game events from other subscribed users. The project includes both a server and a client.

### STOMP Overview
STOMP (Simple-Text-Oriented-Messaging-Protocol) is used for asynchronous message passing between clients and servers. This project uses STOMP for passing messages between the client and the server. The STOMP protocol defines the frame format and semantics of data packet exchanges.

### STOMP Server
The STOMP server is implemented in Java and provides two server patterns: Thread-Per-Client (TPC) and Reactor. The server maintains topics (queues) to which messages can be sent. Clients can subscribe to topics and send messages to them. The server forwards messages to all registered clients of a topic.

### STOMP Client
The STOMP client is implemented in C++. It allows users to perform various commands, including logging in, joining game channels, reporting game events, summarizing games, and logging out. The client interacts with the server using STOMP frames and handles communication through multiple threads.

## Implementation Details
The project consists of both server and client components.

### Server
The server is implemented in Java and supports both TPC and Reactor server patterns. It uses the Connections interface to manage active client connections and implements the StompMessagingProtocol for handling message processing.

### Client
The client is implemented in C++ and provides a command-line interface for users. It supports various commands such as logging in, joining game channels, reporting game events, summarizing games, and logging out. The client parses game events from JSON files and communicates with the server using STOMP frames.

## Game Event
Game events are reported using JSON files. Each game event contains properties like event name, description, and time. General game updates and updates for each team are also included. The project provides a parser for these files in C++.

This project allows World Cup enthusiasts to stay informed about game events and updates through a community-led subscription service. Users can interact with the system to subscribe to game channels, report events, and receive updates from other users.

---
*Note: This README provides an overview of the project and its components. For detailed technical information, refer to the project documentation and code.*
