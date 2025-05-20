# A Chat Application for Remote Message Exchange

## 1. Objective
- **Getting Started:** Familiarize yourself with socket programming.
- **Implement:** Develop a simple chat application for message exchange among remote peers.

## 2. Getting Started
- Check [Stream Socket](../05__IPC_Socket/BT1/IPv4_Stream_Socket/) for a basic understanding of implementing a TCP socket server and client (IPv4 Stream Socket).

## 3. Implementation

### 3.1. Programming Environment
- You may use any programming language you are comfortable with.
- **NOTE:** You must:
  1. Use TCP Sockets in your peer connection implementation.
  2. Handle multiple socket connections.
  3. Integrate both client-side and server-side code into one program and run it on each peer.

### 3.2. Running Your Program
- Your program should accept one command line parameter: the port on which it will listen for incoming connections.
- Example:
  ```bash
  ./chat 4322
  ```
  Here, `4322` is the listening port.
- Run your program on three computers and perform message exchange.

### 3.3. Program Functionality
When launched, your process should work like a UNIX shell, accepting incoming connections and providing a user interface with the following commands:

1. **help**
   - Display information about available user interface options or command manual.

2. **myip**
   - Display the IP address of this process.
   - *Note:* The IP should not be your “Local” address (127.0.0.1), but the actual IP of the computer.

3. **myport**
   - Display the port on which this process is listening for incoming connections.

4. **connect \<destination\> \<port no\>**
   - Establish a new TCP connection to the specified `<destination>` at the specified `<port no>`.
   - The `<destination>` is the IP address of the computer.
   - Invalid IPs, self-connections, and duplicate connections should be rejected with suitable error messages.
   - Success or failure in connections should be indicated by both peers.

5. **list**
   - Display a numbered list of all connections this process is part of (both initiated and received).
   - Output format:
     ```bash
     1: 192.168.21.20 4545
     2: 192.168.21.21 5454
     3: 192.168.21.23 5000
     4: 192.168.21.24 5000
     ```

6. **terminate \<connection id\>**
   - Terminate the connection listed under the specified number from the `list` command.
   - Example: `terminate 2` ends the connection with `192.168.21.21`.
   - Display an error if the connection ID is invalid.
   - If a remote machine terminates a connection, display a message.

7. **send \<connection id\> \<message\>**
   - Send the message to the host on the connection designated by the number from the `list` command.
   - Example: `send 3 Oh! This project is a piece of cake`
   - The message can be up to 100 characters long, including spaces.
   - On success, display: `Message sent to <connection id>`
   - On receiving a message, display:
     ```
     Message received from <sender IP>
     Sender’s Port: <sender port>
     Message: "<received message>"
     ```

8. **exit**
   - Close all connections and terminate the process.
   - Other peers should update their connection list by removing the peer that exits.

---

## 4. Important Key Points
- Submit only one program (not separate client and server).
- Error handling is very important—display appropriate messages for errors.
- If in doubt, consult during office hours.
- Submission deadline is strict—no extensions.
- Do not submit binaries, object files, or test files.
