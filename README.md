# World Cup Update Subscription Service

Stay updated with the World Cup through a community-led update subscription service. This project implements a server-client system in Java and C++ that allows users to subscribe to game channels, report game events, and receive updates. Immerse yourself in client-server programming while combining two languages.

## Table of Contents
- [General Description](#general-description)
- [STOMP Overview](#stomp-overview)
- [STOMP Server Implementation (Java)](#stomp-server)
- [STOMP Client Implementation (C++)](#stomp-client)
- [Implementation Details](#implementation-details)
  - [Server (Java)](#server)
  - [Client (C++)](#client)
- [Game Event Reporting](#game-event)
- [Project Goals](#project-goals)

## General Description
Experience the World Cup like never before with the World Cup Update Subscription Service. This project showcases the synergy of client-server programming by combining Java and C++. Users can subscribe to a game channel, report events, and receive updates from other enthusiasts in real-time.

### STOMP Overview
Utilizing the Simple-Text-Oriented-Messaging-Protocol (STOMP), this project enables seamless asynchronous communication between clients and servers. STOMP facilitates the exchange of messages, with clearly defined frame formats and data exchange semantics.

### STOMP Server Implementation (Java)
The Java-based STOMP server boasts both Thread-Per-Client (TPC) and Reactor server patterns. Managing topics (queues) for message broadcasting, the server connects subscribers and forwards messages to relevant subscribers of a topic.

### STOMP Client Implementation (C++)
The C++ STOMP client serves as the user's window into the World Cup action. Through a command-line interface, users can log in, join game channels, report events, summarize games, and log out. Multi-threaded communication ensures smooth real-time interactions.

## Implementation Details
This project showcases the integration of Java and C++ in a client-server architecture.

### Server (Java)
The server, written in Java, harmonizes TPC and Reactor patterns. Leveraging the Connections interface, it manages active client connections. The StompMessagingProtocol handles incoming messages and processing.

### Client (C++)
The C++ client brings the World Cup to life for users. The command-line interface accommodates various commands: login, channel subscription, event reporting, game summaries, and logout. The client efficiently parses JSON-based game events and communicates using STOMP frames.

## Game Event Reporting
JSON files encapsulate game events, encompassing event name, description, and time. Specific updates for each team and general game progress are included. A dedicated C++ parser interprets these files.

## Project Goals
- Combine the prowess of Java and C++ in a unified project to deepen understanding of client-server programming.
- Develop a functional subscription service allowing users to subscribe to game channels, report events, and receive real-time updates.
- Showcase proficiency in multi-threading, socket handling, and asynchronous communication.
- Practice parsing JSON files to extract essential game event details.

---
