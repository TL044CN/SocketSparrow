#include "TLSSocket.hpp"
#include "Exceptions.hpp"

namespace SocketSparrow {

TLSSocket::TLSSocket(int fd, std::shared_ptr<Endpoint> endpoint, SocketType protocol)
    : Socket(fd, endpoint, protocol) {
    mOpenSSL_CTX = SSL_CTX_new(SSLv23_client_method());
    mOpenSSL_SSL = SSL_new(mOpenSSL_CTX);
    SSL_set_fd(mOpenSSL_SSL, getNativeSocket());
}

TLSSocket::TLSSocket(AddressFamily af, SocketType protocol)
    : Socket(af, protocol) {
    mOpenSSL_CTX = SSL_CTX_new(SSLv23_client_method());
    mOpenSSL_SSL = SSL_new(mOpenSSL_CTX);
    SSL_set_fd(mOpenSSL_SSL, getNativeSocket());
}

TLSSocket::TLSSocket(AddressFamily af, std::shared_ptr<Endpoint> endpoint)
    : Socket(af, endpoint) {
    mOpenSSL_CTX = SSL_CTX_new(SSLv23_server_method());
    mOpenSSL_SSL = SSL_new(mOpenSSL_CTX);
    SSL_set_fd(mOpenSSL_SSL, getNativeSocket());
}

TLSSocket::~TLSSocket() {
    SSL_shutdown(mOpenSSL_SSL);
    SSL_free(mOpenSSL_SSL);
    SSL_CTX_free(mOpenSSL_CTX);
}


void TLSSocket::connect(std::shared_ptr<Endpoint> endpoint) {
    Socket::connect(endpoint);
    if(SSL_connect(mOpenSSL_SSL) >= 0) {
        throw SocketException("Failed to connect to endpoint");
    }
}

std::shared_ptr<Socket> TLSSocket::accept() {
    if(getProtocol() != SocketType::TCP) {
        throw SocketException("Only TCP sockets can accept connections");
    }

    if(getEndpoint() == nullptr) {
        throw SocketException("Cannot accept without binding to an endpoint");
    }

    if(getState() != SocketState::Listening) {
        throw SocketException("Cannot accept without listening");
    }

    sockaddr_storage clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);

    int clientSocket = ::accept(getNativeSocket(), (sockaddr*)&clientAddr, &clientAddrSize);
    if(clientSocket == -1) {
        throw SocketException("Failed to accept connection");
    }

    std::shared_ptr<Endpoint> clientEndpoint = std::make_shared<Endpoint>(clientAddr, clientAddrSize);
    TLSSocket* RawConnection = new TLSSocket(clientSocket, clientEndpoint, getProtocol());

    SSL_set_fd(RawConnection->mOpenSSL_SSL, RawConnection->getNativeSocket());

    if(SSL_accept(RawConnection->mOpenSSL_SSL) <= 0) {
        throw SocketException("Failed to accept connection");
    }

    // cannot construct TLSSocket directly because of protected constructor 
    std::shared_ptr<Socket> socket = std::shared_ptr<Socket>(RawConnection);
    std::static_pointer_cast<TLSSocket>(socket)->setState(SocketState::Connected);

    return socket;
}

ssize_t TLSSocket::send(const std::vector<char>& data) const {
    ssize_t sent = SSL_write(mOpenSSL_SSL, data.data(), data.size());
    if(sent < 0) {
        throw SendError("Failed to send");
    }
    return sent;
}

ssize_t TLSSocket::send(const std::string& data) const {
    return send(std::vector<char>(data.begin(), data.end()));
}

ssize_t TLSSocket::recv(std::vector<char>& buffer, ExplicitBool autoresize) const {
    ssize_t totalReceived = 0;
    if(autoresize) {
        buffer.resize(1024);
    }

    ssize_t received = 1;
    while(received > 0) {
        received = SSL_read(mOpenSSL_SSL, buffer.data() + totalReceived, buffer.size() - totalReceived);
        if(received <= 0) {
            int error = SSL_get_error(mOpenSSL_SSL, received);
            switch(error) {
                case SSL_ERROR_ZERO_RETURN:
                    break;
                case SSL_ERROR_WANT_READ:
                case SSL_ERROR_WANT_WRITE:
                    continue;
                case SSL_ERROR_SYSCALL:
                    throw RecvError("I/O error occured");
                default:
                    throw RecvError("Failed to receive");
            }
        }

        if(received == 0) {
            break;
        }

        totalReceived += received;

        if(autoresize && buffer.size() - totalReceived < buffer.size() * 2) {
            buffer.resize(buffer.size() * 2);
        }
    }
    if(autoresize) {
        buffer.resize(totalReceived);
    }

    return totalReceived;
}

} // namespace SocketSparrow