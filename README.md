# Socket Sparrow

<a href='https://jenkins.shodan.fyi/job/SocketSparrow/'><img src='https://jenkins.shodan.fyi/buildStatus/icon?job=SocketSparrow'></a>
[![Quality Gate Status](https://sonarqube.shodan.fyi/api/project_badges/measure?project=TL044CN_SocketSparrow_eef15895-f7ab-4bd5-85e4-90d29f7a3692&metric=alert_status&token=sqb_0a15bbb32f7343079dd1511720ec9b45a49d64f3)](https://sonarqube.shodan.fyi/dashboard?id=TL044CN_SocketSparrow_eef15895-f7ab-4bd5-85e4-90d29f7a3692)
[![Lines of Code](https://sonarqube.shodan.fyi/api/project_badges/measure?project=TL044CN_SocketSparrow_eef15895-f7ab-4bd5-85e4-90d29f7a3692&metric=ncloc&token=sqb_0a15bbb32f7343079dd1511720ec9b45a49d64f3)](https://sonarqube.shodan.fyi/dashboard?id=TL044CN_SocketSparrow_eef15895-f7ab-4bd5-85e4-90d29f7a3692)
[![Coverage](https://sonarqube.shodan.fyi/api/project_badges/measure?project=TL044CN_SocketSparrow_eef15895-f7ab-4bd5-85e4-90d29f7a3692&metric=coverage&token=sqb_0a15bbb32f7343079dd1511720ec9b45a49d64f3)](https://sonarqube.shodan.fyi/dashboard?id=TL044CN_SocketSparrow_eef15895-f7ab-4bd5-85e4-90d29f7a3692)

Socket Sparrow is a small Network Abstraction library

## Usage:

### Creating a TCP Server Socket

```c++
#include "Socket.hpp"
using namespace SocketSparrow;

// creating a basic TCP Socket
Socket myServer(AddressFamily::IPv4, SocketType::TCP);

// binding the Server Socket to a Port
myServer.bindToPort(8080);

// listen to new connections. In this case the socket has a backlog of 5.
myServer.listeh(5);

// accept new connections
auto myConnection = myServer.accept();

// At this point myConnection (which is a shared_ptr to a Socket)
// can be used to communicate with the connected client.
```

### Creating a TCP Client Socket

(Note: This should be done in another program. Alternatively multiple threads works too. but the calls are usually blocking for now)

```c++
// create another basic TCP Socket
Socket myClient(AddressFamily::IPv4, SocketType::TCP);

// define the Servers endpoint
Endpoint serverAddress("localhost", 8080);

// connect to the Server
myClient.connect(serverAddress);

// at this point myClient can communicate with the Server (if the connection was successful)

```

### This is a Stub! More Info will follow.
