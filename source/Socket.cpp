#include "Socket.hpp"
#include "Util.hpp"
#include "Exceptions.hpp"

#include <sys/socket.h>
#include <sys/unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>

namespace SocketSparrow {
using namespace Util;

Socket::Socket(int fd, std::shared_ptr<Endpoint> endpoint, SocketType protocol)
    : mNativeSocket(fd), mEndpoint(endpoint), mProtocol(protocol) {
    if ( mNativeSocket == -1 ) {
        throw SocketException();
    }
}

Socket::Socket(AddressFamily af, SocketType protocol)
    : mAddressFamily(af), mProtocol(protocol) {
    mNativeSocket = socket(
        getNativeAddressFamily(mAddressFamily),
        getNativeSocketType(mProtocol),
        0
    );
    if ( mNativeSocket == -1 ) {
        throw SocketException();
    }
}

Socket::Socket(AddressFamily af, std::shared_ptr<Endpoint> endpoint)
    : mAddressFamily(af), mProtocol(SocketType::TCP) {
    mNativeSocket = socket(
        getNativeAddressFamily(mAddressFamily),
        getNativeSocketType(mProtocol),
        0
    );

    if ( mNativeSocket == -1 ) {
        throw SocketException();
    }

    bind(endpoint);
}

Socket::~Socket() {
    close(mNativeSocket);
}


void Socket::bind(std::shared_ptr<Endpoint> endpoint) {
    if ( mProtocol != SocketType::TCP ) {
        throw SocketException();
    }

    mEndpoint = endpoint;
    if ( ::bind(mNativeSocket, mEndpoint->c_addr(), mEndpoint->c_size()) == -1 ) {
        throw SocketException();
    }
}

void Socket::bindToPort(uint16_t port) {
    mEndpoint = std::make_shared<Endpoint>(mAddressFamily, port);
    if ( ::bind(mNativeSocket, mEndpoint->c_addr(), mEndpoint->c_size()) == -1 ) {
        throw SocketException();
    }
}

void Socket::connect(std::shared_ptr<Endpoint> endpoint) {
    if ( mProtocol != SocketType::TCP ) {
        throw SocketException();
    }

    if ( ::connect(mNativeSocket, endpoint->c_addr(), endpoint->c_size()) == -1 ) {
        throw SocketException();
    }
}

void Socket::listen(int backlog) {
    if ( mProtocol != SocketType::TCP ) {
        throw SocketException();
    }

    if ( ::listen(mNativeSocket, backlog) == -1 ) {
        throw SocketException();
    }
}

std::shared_ptr<Socket> Socket::accept() {
    if ( mProtocol != SocketType::TCP ) {
        throw SocketException();
    }

    sockaddr_storage clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    int clientSocket = ::accept(mNativeSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    if ( clientSocket == -1 ) {
        throw SocketException();
    }

    std::shared_ptr<Endpoint> clientEndpoint = std::make_shared<Endpoint>(clientAddr, clientAddrSize);
    Socket* Connection = new Socket(clientSocket, clientEndpoint, mProtocol);
    return std::shared_ptr<Socket>(Connection);
}

void Socket::setBroadcast(bool broadcast) {
    int opt = broadcast ? 1 : 0;
    if ( setsockopt(mNativeSocket, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt)) == -1 ) {
        throw SocketException();
    }
}

void Socket::enablePortReuse(bool reuse) {
    int opt = reuse ? 1 : 0;
    if ( setsockopt(mNativeSocket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1 ) {
        throw SocketException();
    }
}

ssize_t Socket::send(std::vector<char> data) const {
    ssize_t sent = ::send(mNativeSocket, data.data(), data.size(), 0);
    if ( sent == -1 ) {
        throw SendError();
    }
    return sent;
}

ssize_t Socket::recv(std::vector<char>& buffer) const {
    ssize_t received = ::recv(mNativeSocket, buffer.data(), buffer.size(), 0);
    if ( received == -1 ) {
        throw RecvError();
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
        throw SendError();
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