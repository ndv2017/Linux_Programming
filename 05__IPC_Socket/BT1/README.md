**BT1**. Rewrite 4 types of Socket below for sending and receiving a simple data string:

- [IPv4 Stream Socket (TCP)](./IPv4_Stream_Socket/)
- [IPv4 Datagram Socket (UDP)](./IPv4_Datagram_Socket/)
- [Unix Domain Stream Socket](./Unix_Stream_Socket/)
- [Unix Domain Datagram Socket](./Unix_Datagram_Socket/)

---

*Summary Table*

| Socket Type        | Address/Path | Connection Type | Description |
|--------------------|--------------|-----------------|-------------|
| IPv4 Stream (TCP)   | IP + Port     | Connection-Oriented | Reliable byte stream |
| IPv4 Datagram (UDP) | IP + Port     | Connectionless   | Message-based, unreliable |
| Unix Stream         | Filesystem path | Connection-Oriented | Local-only reliable stream |
| Unix Datagram       | Filesystem path | Connectionless   | Local-only message-based |

<br>

*Notes*

- **Unix domain sockets** create files like `./mysock`, `./my_server_sock`, `./my_client_sock`.  
  These socket files are automatically **removed** when the programs exit.
- Type `"exit"` in the chat to gracefully **terminate** the server and client.
- Use `lsof` to check for open sockets, for example:
    - After running the server and client of Unix domain stream socket, run this command:
    ```bash
    lsof -U | grep mysock
    ```
    - And here's the output:
    ```bash
    server    9467 vion    3u  unix 0x0000000000000000      0t0 108441 ./mysock type=STREAM
    server    9467 vion    4u  unix 0x0000000000000000      0t0 108442 ./mysock type=STREAM
    ```
