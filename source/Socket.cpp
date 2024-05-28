#include "Socket.hpp"
#include "Util.hpp"
#include "Exceptions.hpp"

#include <thread>
#include <cstring>

#include <sys/socket.h>
#include <sys/unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <error.h>

namespace SocketSparrow {

using namespace Util;

Socket::Socket(int fd, std::shared_ptr<Endpoint> endpoint, SocketType protocol)
    : mNativeSocket(fd),
    mProtocol(protocol),
    mAddressFamily(endpoint->getAddressFamily()),
    mEndpoint(endpoint) {
    if ( mNativeSocket == -1 ) {
        throw SocketException("Failed to create Socket");
    }
    mState = SocketState::Open;
}

Socket::Socket(AddressFamily af, SocketType protocol)
    : mProtocol(protocol), 
    mAddressFamily(af) {
    mNativeSocket = socket(
        getNativeAddressFamily(mAddressFamily),
        getNativeSocketType(mProtocol),
        0
    );

    if ( mNativeSocket == -1 ) {
        throw SocketException(errno, "Failed to create Socket");
    }
    mState = SocketState::Open;
}

Socket::Socket(AddressFamily af, std::shared_ptr<Endpoint> endpoint)
    : mProtocol(SocketType::TCP), 
    mAddressFamily(af) {
    mNativeSocket = socket(
        getNativeAddressFamily(mAddressFamily),
        getNativeSocketType(mProtocol),
        0
    );

    if ( mNativeSocket == -1 ) {
        throw SocketException(errno, "Failed to create Socket");
    }

    mState = SocketState::Open;

    bind(endpoint);
}

Socket::~Socket() {
    close(mNativeSocket);
    mState = SocketState::Closed;
}


void Socket::setState(SocketState state) {
    mState = state;
}


int Socket::getNativeSocket() const {
    return mNativeSocket;
}

SocketType Socket::getProtocol() const {
    return mProtocol;
}

AddressFamily Socket::getAddressFamily() const {
    return mAddressFamily;
}

std::shared_ptr<Endpoint> Socket::getEndpoint() const {
    return mEndpoint;
}

SocketState Socket::getState() const {
    return mState;
}


void Socket::bind(std::shared_ptr<Endpoint> endpoint) {
    mEndpoint = endpoint;
    if ( ::bind(mNativeSocket, mEndpoint->c_addr(), mEndpoint->c_size()) == -1 ) {
        throw SocketException(errno, "Failed to bind to endpoint");
    }

}

void Socket::bindToPort(uint16_t port) {
    mEndpoint = std::make_shared<Endpoint>(mAddressFamily, port);
    if ( ::bind(mNativeSocket, mEndpoint->c_addr(), mEndpoint->c_size()) == -1 ) {
        throw SocketException(errno, "Failed to bind to endpoint");
    }
}

void Socket::connect(std::shared_ptr<Endpoint> endpoint) {
    if ( mProtocol != SocketType::TCP ) {
        throw SocketException("Cannot connect a UDP socket");
    }

    if ( ::connect(mNativeSocket, endpoint->c_addr(), endpoint->c_size()) == -1 ) {
        throw SocketException(errno, "Failed to connect");
    }

    mState = SocketState::Connected;
}

void Socket::listen(int backlog) {
    if ( mProtocol != SocketType::TCP ) {
        throw SocketException("Cannot listen on a UDP socket");
    }

    if( mEndpoint == nullptr ) {
        throw SocketException("Cannot listen without binding to an endpoint");
    }

    if ( ::listen(mNativeSocket, backlog) == -1 ) {
        throw SocketException(errno, "Failed to listen");
    }

    mState = SocketState::Listening;
}

std::shared_ptr<Socket> Socket::accept() {
    if ( mProtocol != SocketType::TCP ) {
        throw SocketException("Cannot accept on a UDP socket");
    }

    if( mEndpoint == nullptr ) {
        throw SocketException("Cannot accept without binding to an endpoint");
    }

    if( mState != SocketState::Listening ) {
        throw SocketException("Cannot accept without listening");
    }

    sockaddr_storage clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);

    int clientSocket = ::accept(mNativeSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    if ( clientSocket == -1 ) {
        throw SocketException(errno, "Failed to accept");
    }

    std::shared_ptr<Endpoint> clientEndpoint = std::make_shared<Endpoint>(clientAddr, clientAddrSize);
    Socket* Connection = new Socket(clientSocket, clientEndpoint, mProtocol);
    Connection->mState = SocketState::Connected;
    
    return std::shared_ptr<Socket>(Connection);
}

void Socket::enableBroadcast(bool enable) {
    int opt = enable ? 1 : 0;
    if ( setsockopt(mNativeSocket, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt)) == -1 ) {
        throw SocketException(errno,"Failed to set socket option");
    }
}

void Socket::enablePortReuse(bool enable) {
    int opt = enable ? 1 : 0;
    if ( setsockopt(mNativeSocket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1 ) {
        throw SocketException(errno,"Failed to set socket option");
    }
}

void Socket::enableAddressReuse(bool enable) {
    int opt = enable ? 1 : 0;
    if ( setsockopt(mNativeSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1 ) {
        throw SocketException(errno,"Failed to set socket option");
    }
}

void Socket::enableNonBlocking(bool enable) {
    int flags = fcntl(mNativeSocket, F_GETFL, 0);
    if ( flags == -1 ) {
        throw SocketException(errno,"Failed to get socket flags");
    }

    if ( enable ) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }

    if ( fcntl(mNativeSocket, F_SETFL, flags) == -1 ) {
        throw SocketException(errno,"Failed to set socket flags");
    }
}

ssize_t Socket::send(const std::vector<char>& data) const {
    ssize_t sent = ::send(mNativeSocket, data.data(), data.size(), 0);
    if ( sent == -1 ) {
        throw SendError(errno, "Failed to send");
    }
    return sent;
}

ssize_t Socket::send(const std::string& data) const {
    return send(std::vector<char>(data.begin(), data.end()));
}

ssize_t Socket::recv(std::vector<char>& buffer, ExplicitBool autoresize) const {
    ssize_t totalReceived = 0;
    if(autoresize) {
        buffer.resize(1024);
    }
    ssize_t received = 1;
    while(received > 0) {
        received = ::recv(mNativeSocket, buffer.data() + totalReceived, 1024, 0);
        if ( received < 0 ) {
            throw RecvError(errno, "Failed to receive");
        }

        if(received == 0) {
            break;
        }

        totalReceived += received;
        if(autoresize && buffer.size() - totalReceived < buffer.size() / 2) {
            buffer.resize(buffer.size() * 2);
        }

        if(received < 1024) {
            break;
        }
    }
    if(autoresize) {
        buffer.resize(totalReceived);
    }

    return totalReceived;
}

ssize_t Socket::recv(std::vector<char>& buffer, size_t size) const {
    buffer.resize(size);
    return recv(buffer, ExplicitBool(false));
}

ssize_t Socket::recv(std::string& buffer) const {
    std::vector<char> vec;
    ssize_t received = recv(vec, ExplicitBool(true));
    buffer = std::string(vec.begin(), vec.end());
    return received;
}

ssize_t Socket::send_to(std::vector<char> data, std::shared_ptr<Endpoint> endpoint) {
    if(mProtocol != SocketType::UDP) {
        throw SocketException("Cannot send_to from a TCP socket");
    }
    ssize_t sent = ::sendto(mNativeSocket, data.data(), data.size(), 0, endpoint->c_addr(), endpoint->c_size());
    if ( sent == -1 ) {
        throw SendError(errno, "Failed to send");
    }
    return sent;
}

ssize_t Socket::send_to(const std::string& data, std::shared_ptr<Endpoint> endpoint) {
    return send_to(std::vector<char>(data.begin(), data.end()), endpoint);
}

ssize_t Socket::send_to(UDPPacket packet) {
    if(mProtocol != SocketType::UDP) {
        throw SocketException("Cannot send_to from a TCP socket");
    }

    return send_to(packet.data, packet.endpoint);
}

UDPPacket Socket::recv_from() const {
    if(mProtocol != SocketType::UDP) {
        throw SocketException("Cannot recv_from from a TCP socket");
    }

    UDPPacket packet;
    sockaddr addr;
    socklen_t size;
    ssize_t received = ::recvfrom(mNativeSocket, packet.data.data(), packet.data.size(), 0, &addr, &size);
    if ( received == -1 ) {
        throw RecvError(errno, "Failed to receive");
    }
    
    packet.data.resize(received);
    return packet;
}


ssize_t Socket::operator<<(const std::vector<char>& data) const {
    return send(data);
}

ssize_t Socket::operator<<(const std::string& data) const {
    return send(std::vector<char>(data.begin(), data.end()));
}

ssize_t Socket::operator>>(std::vector<char>& buffer) const {
    return recv(buffer);
}

ssize_t Socket::operator>>(std::string& buffer) const {
    std::vector<char> vec;
    ssize_t received = recv(vec);
    buffer = std::string(vec.begin(), vec.end());
    return received;
}

ssize_t Socket::operator>>(std::stringstream& stream) const {
    std::vector<char> vec;
    ssize_t received = recv(vec);
    stream.write(vec.data(), received);
    return received;
}

}   // namespace SocketSparrow