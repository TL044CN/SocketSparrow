#include "Socket.hpp"
#include "Util.hpp"
#include "Exceptions.hpp"

#include <sys/socket.h>
#include <sys/unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>

#include <error.h>

namespace SocketSparrow {
using namespace Util;

Socket::Socket(int fd, std::shared_ptr<Endpoint> endpoint, SocketType protocol)
    : mNativeSocket(fd),
    mProtocol(protocol),
    mAddressFamily(endpoint->getAddressFamily()),
    mEndpoint(endpoint) {
    if ( mNativeSocket == -1 ) {
        throw SocketException(errno, "Failed to create Socket");
    }
    mState = SocketState::Open;
}

Socket::Socket(AddressFamily af, SocketType protocol)
    : mAddressFamily(af), mProtocol(protocol) {
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
    : mAddressFamily(af),
    mProtocol(SocketType::TCP) {
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


void Socket::bind(std::shared_ptr<Endpoint> endpoint) {
    if ( mProtocol != SocketType::TCP ) {
        throw SocketException("Cannot bind a UDP socket to an endpoint");
    }

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

void Socket::setBroadcast(bool enable) {
    int opt = enable ? 1 : 0;
    if ( setsockopt(mNativeSocket, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt)) == -1 ) {
        throw SocketException("Failed to set broadcast mode");
    }
}

void Socket::enablePortReuse(bool enable) {
    int opt = enable ? 1 : 0;
    if ( setsockopt(mNativeSocket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1 ) {
        throw SocketException("Failed to set port reuse mode");
    }
}

ssize_t Socket::send(std::vector<char> data) const {
    ssize_t sent = ::send(mNativeSocket, data.data(), data.size(), 0);
    if ( sent == -1 ) {
        throw SendError(errno, "Failed to send");
    }
    return sent;
}

ssize_t Socket::recv(std::vector<char>& buffer) const {
    ssize_t received = ::recv(mNativeSocket, buffer.data(), buffer.size(), 0);
    if ( received == -1 ) {
        throw RecvError(errno, "Failed to receive");
    }
    return received;
}

ssize_t Socket::recv(std::vector<char>& buffer, size_t size) const {
    buffer.resize(size);
    return recv(buffer);
}

ssize_t Socket::sendTo(std::vector<char> data, std::shared_ptr<Endpoint> endpoint) {
    ssize_t sent = ::sendto(mNativeSocket, data.data(), data.size(), 0, endpoint->c_addr(), endpoint->c_size());
    if ( sent == -1 ) {
        throw SendError(errno, "Failed to send");
    }
    return sent;
}

ssize_t Socket::sendTo(UDPPacket packet) {
    //todo: implement
    return 0;
}

UDPPacket Socket::recvFrom() const {
    // todo: implement
    return {};
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