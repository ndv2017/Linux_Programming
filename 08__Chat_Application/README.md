# Chat Application Project Description

For a detailed description of the assignment requirements, see [Assignment Description](./Chat_Application_Assignment.md).

## Overview
The `chat_app` project is a C-based, command-line chat application that enables peer-to-peer communication over TCP sockets. It allows users to connect to other instances of the application, send messages, list active connections, and terminate connections. The application uses non-blocking sockets and the `select` system call for concurrent handling of multiple connections and user input. It supports commands like `help`, `myip`, `myport`, `connect`, `list`, `terminate`, `send`, and `exit`.

The project is modular, split into multiple source files for maintainability, and includes a Makefile for building and running the application.

## Project Structure
The project is organized as follows:

```
project_directory/
├── inc/
│   ├── network.h          # Network-related declarations and structures
│   └── chat.h             # UI and utility function declarations
├── src/
│   ├── main.c             # Main program and command-line interface
│   ├── chat.c             # User interface utilities
│   └── network.c          # Network communication logic
├── Makefile               # Build script for compiling and running
├── obj/                   # Object files (generated during build)
├── bin/                   # Executable (chat_app, generated during build)
└── libs/
    └── shared/            # Shared library (libchat_app.so, generated during build)
```

### File Descriptions
- **inc/network.h**: Contains network-related structures, macros, and function prototypes for socket operations, peer management, and message handling.
- **inc/chat.h**: Contains UI-related function prototypes and utility declarations for displaying help messages and local network information.
- **src/main.c**: Implements the main program entry point and command-line interface for processing user commands.
- **src/chat.c**: Handles user interface tasks, such as displaying help messages and local network information.
- **src/network.c**: Manages network operations, including socket setup, peer connections, and message handling.
- **Makefile**: Automates building and running the application.

## Features
- **Peer-to-Peer Communication**: Connects to other instances using IP addresses and port numbers.
- **Non-Blocking I/O**: Handles multiple connections and user input concurrently.
- **Command-Line Interface**: Supports commands:
  - `help`: Show available commands.
  - `myip`: Display local IP address.
  - `myport`: Display listening port.
  - `connect <ip> <port>`: Connect to a peer.
  - `list`: List active connections.
  - `terminate <id>`: Close a connection.
  - `send <id> <message>`: Send a message.
  - `exit`: Close connections and exit.
- **Signal Handling**: Gracefully handles `SIGINT` and `SIGTERM`.
- **Shared Library**: Core functionality encapsulated in a shared library for modularity.

## Prerequisites
- **Compiler**: GCC (`gcc`) for compiling C code.
- **Valgrind**: Optional, for memory leak checking (`sudo apt install valgrind` on Debian/Ubuntu).
- **Operating System**: Linux or POSIX-compliant system.

## How to Build
1. Ensure the project files are organized as described above.
2. Open a terminal in the `project_directory/`.
3. Run:
   ```bash
   make
   ```
   This creates:
   - `obj/` with object files.
   - `bin/chat_app` (executable).
   - `libs/shared/libchat_app.so` (shared library).

## How to Run
The application requires a port number to listen for connections. Use the `PORT` variable with `make run`.

1. Run with a specific port (e.g., 12345):
   ```bash
   make run PORT=12345
   ```
2. If no port is specified, defaults to `8080`:
   ```bash
   make run
   ```
3. The application starts, shows the help menu, and waits for commands:
   ```
   *********************************** Chat Application *********************************

   List of commands:
     help                           Display UI options or commands
     myip                           Display IP address of this process
     myport                         Display listening port
     connect <destination_ip> <destination_port_no>  Connect to another process
     list                           List all connections
     terminate <connection_id>      Terminate a connection
     send <connection_id> <message> Send a message to a connection
     exit                           Close all connections & exit the application

   *********************************** Chat Application *********************************

   Listening on port 12345...
   Enter a command:
   ```

## Example Usage
1. Start two instances on localhost:
   - Terminal 1: `make run PORT=12345`
   - Terminal 2: `make run PORT=54321`
2. In Terminal 1, connect to Terminal 2:
   ```
   Enter a command: connect 127.0.0.1 54321
   Connection established with 127.0.0.1 as connection ID 0
   ```
3. Send a message from Terminal 1:
   ```
   Enter a command: send 0 Hello, peer!
   Message sent to connection 0.
   ```
4. In Terminal 2, see the message:
   ```
   Message from connection 0: "Hello, peer!"
   ```
5. List connections in Terminal 1:
   ```
   Enter a command: list
   ID      IP Address      Port
   ---     ----------      ----
   0       127.0.0.1       54321
   ```
6. Terminate the connection:
   ```
   Enter a command: terminate 0
   Connection 0 terminated.
   ```
7. Exit:
   ```
   Enter a command: exit
   All connections closed.
   Exiting...
   ```

## Memory Checking
To check for memory leaks:
```bash
make check_valgrind PORT=12345
```
This runs `bin/chat_app 12345` with Valgrind to detect memory issues.

## Cleaning Up
To remove generated files:
```bash
make clean
```
This deletes `obj/`, `bin/`, and `libs/shared/` directories.
