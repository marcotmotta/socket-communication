# TCP/UDP server and client implementation using sockets in C 

## 1. Single Client Communication

Everytime the client sends a message the server will invert the sentence and change lower case letters to upper case letters, and vice versa. The client will then close but the server will stay listening in case another client sends a new message.

> $ /single-client

### To compile:
> $ make
### To run:
<b>Server TCP:</b> <br>

> $ ./servidorTCP \<port\>

<b>Server UDP:</b>

> $ ./servidorUDP \<port\>
  
<b>Client TCP:</b>

> $ ./clienteTCP \<ip\> \<port\>

<b>Client UDP:</b>

> $ ./clienteUDP \<ip\> \<port\>
  
<b>Note:</b> The port on the server and on the client must be the same for the communication to happen.

## 2. Multiclient Communication

> $ /multiclient

### To compile:
> $ make
### To run:
> WIP
