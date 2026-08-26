💬 Multi-Client Chat Application in C

Welcome to the Multi-Client Chat Application! 🚀

This is a simple chat application built using C socket programming where multiple clients can connect to a single server and communicate with each other.

Think of it like this:

                 🖥️ Server
                    │
          ┌─────────┼─────────┐
          │         │         │
       👤 Client 1 👤 Client 2 👤 Client 3
          │         │         │
          └─────────┴─────────┘
             💬 Chat Together!


The goal of this project is to understand how client-server communication works using sockets in C.

📱 Application Overview

The application has two parts:

🖥️ Server

The server is the main hub of the chat application.

It:

Starts and waits for clients.
Accepts client connections.
Handles communication between multiple clients.
Runs on a specific port.
👤 Client

The client is the application that users run to join the chat.

It:

Connects to the server.
Uses the server IP address and port.
Allows multiple clients to join the same chat.

🎯 Simple idea:

Start the server first → Start multiple clients → Everyone joins the same chat! 🎉

📂 Project Structure
mutli-chat-app-in-c/
│
├── 📄 CMakeLists.txt
├── 📄 README.md
│
├── 📁 inc/
│   ├── client.h
│   ├── define.h
│   └── server.h
│
└── 📁 src/
    ├── client.c
    ├── main.c
    └── server.c

🛠️ Technologies Used

This project uses some good old-school technology 😎:

💻 C
🔌 Socket Programming
🌐 TCP/IP
⚙️ CMake
🔨 Make
🐧 Linux / POSIX
🖥️ Build and Run the Server

The server must be started first because clients need a running server to connect to.

1️⃣ Go to the project directory
cd mutli-chat-app-in-c

2️⃣ Configure the server
cmake -S . -B build -DBUILD_TYPE=server

3️⃣ Go to the build directory
cd build

4️⃣ Build the server

Use make to build the application:

make


🎉 The server executable will now be available inside the build directory.

5️⃣ Start the server

The server requires a port number.

./server 8080


Here, 8080 is the port used by the chat server.

🚀 Server is now ready for clients!

👤 Build and Run the Client

Now let's connect some clients! 🎉

Before starting the client, make sure the server is already running.

1️⃣ Go back to the project directory

If you are currently inside build:

cd ..

2️⃣ Remove the existing build

The project uses the same build directory for either the server or client.

So, remove the previous server build:

rm -rf build/

3️⃣ Configure the client
cmake -S . -B build -DBUILD_TYPE=client

4️⃣ Go to the build directory
cd build

5️⃣ Build the client
make


🎉 The client executable is now ready.

6️⃣ Start the client

The client requires:

./client <port_number> <server_ip_address>


For example:

./client 8080 192.168.1.10


Where:

8080 → Server port
192.168.1.10 → IP address of the machine running the server
🌐 Real Demo — Multiple Clients Chatting

Let's make it interesting! 😎🔥

Imagine you have three computers connected to the same Wi-Fi/network.

              🌐 Same Network / Wi-Fi
                       │
          ┌────────────┼────────────┐
          │            │            │
       🖥️ PC 1      💻 PC 2      💻 PC 3
       Server       Client 1      Client 2

🖥️ PC 1 — Start the Server

Find the IP address of PC 1.

For example:

Server IP = 192.168.1.10


Start the server:

./server 8080


Now PC 1 is waiting for clients. 👀

💻 PC 2 — Start Client 1

PC 2 must be connected to the same network as PC 1.

Run:

./client 8080 192.168.1.10


Client 1 is now connected! 🎉

💻 PC 3 — Start Client 2

PC 3 should also be connected to the same network.

Run:

./client 8080 192.168.1.10


Now we have:

                 🖥️ PC 1
              🏠 Chat Server
              192.168.1.10
                    │
             🌐 Same Network
              ┌─────┴─────┐
              │           │
           💻 PC 2     💻 PC 3
          Client 1     Client 2


🎉 Both clients are connected to the same server!

Now imagine:

👤 Client 1: Hello everyone! 👋

👤 Client 2: Hey! What's up? 😄

👤 Client 1: Welcome to the chat! 🎉


And you can add even more clients:

                    🖥️ Server
                       │
          🌐 Same Network / Wi-Fi
                       │
       ┌───────────────┼───────────────┐
       │               │               │
    💻 Client 1     💻 Client 2     💻 Client 3
       │               │               │
       └───────────────┼───────────────┘
                       │
                    💬 CHAT!


🚀 The more clients, the more fun!

⚠️ Important

For the real multi-computer demo:

🖥️ The server computer and client computers should be on the same network.
🔢 All clients should use the same server port.
🌐 Clients must use the server computer's IP address, not their own IP.
🚀 Start the server before starting the clients.
🔥 Make sure the server port is allowed through the firewall if clients cannot connect.
🎯 Quick Start

If you just want to try everything on one computer, you can use:

Server
cmake -S . -B build -DBUILD_TYPE=server
cd build
make
./server 8080


Then open another terminal and build the client:

cd ..
rm -rf build/
cmake -S . -B build -DBUILD_TYPE=client
cd build
make
./client 8080 127.0.0.1


For multiple computers, replace 127.0.0.1 with the server computer's IP address. 🌐

🤝 Have Fun!

This project is a great way to learn how:

C + Sockets + TCP/IP = Client-Server Communication 💻🔌🌐

Start the server, invite some clients, and let the chatting begin! 💬🔥

Happy Coding! 🚀
