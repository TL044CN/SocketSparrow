#include "TLSSocket.hpp"
#include "Exceptions.hpp"

#include <openssl/err.h>

namespace SocketSparrow {

TLSSocket::TLSSocket(
    int fd,
    std::shared_ptr<Endpoint> endpoint,
    SocketType protocol,
    TLSSocketMode mode
) : Socket(fd, endpoint, protocol), mMode(mode) {
    if(mode == TLSSocketMode::Client) {
        mOpenSSL_CTX = SSL_CTX_new(TLS_client_method());
    } else {
        mOpenSSL_CTX = SSL_CTX_new(TLS_server_method());
    }
    mOpenSSL_SSL = SSL_new(mOpenSSL_CTX);
    SSL_set_fd(mOpenSSL_SSL, getNativeSocket());
}

TLSSocket::TLSSocket(AddressFamily af, SocketType protocol, TLSSocketMode mode)
    : Socket(af, protocol), mMode(mode) {
    if(mode == TLSSocketMode::Client) {
        mOpenSSL_CTX = SSL_CTX_new(TLS_client_method());
    } else {
        mOpenSSL_CTX = SSL_CTX_new(TLS_server_method());
    }
    mOpenSSL_SSL = SSL_new(mOpenSSL_CTX);
    SSL_set_fd(mOpenSSL_SSL, getNativeSocket());
}

TLSSocket::TLSSocket(
    AddressFamily af,
    std::shared_ptr<Endpoint> endpoint,
    TLSSocketMode mode
) : Socket(af, endpoint), mMode(mode) {
    if(mode == TLSSocketMode::Client) {
        mOpenSSL_CTX = SSL_CTX_new(TLS_client_method());
    } else {
        mOpenSSL_CTX = SSL_CTX_new(TLS_server_method());
    }
    mOpenSSL_SSL = SSL_new(mOpenSSL_CTX);
    SSL_set_fd(mOpenSSL_SSL, getNativeSocket());
}

TLSSocket::~TLSSocket() {
    SSL_shutdown(mOpenSSL_SSL);
    SSL_free(mOpenSSL_SSL);
    SSL_CTX_free(mOpenSSL_CTX);
}


TLSSocketMode TLSSocket::getMode() const {
    return mMode;
}

void TLSSocket::connect(std::shared_ptr<Endpoint> endpoint) {
    Socket::connect(endpoint);
    auto err = SSL_connect(mOpenSSL_SSL);
    if(err < 0) {
        if(FILE* log = fopen("error.txt", "w")) {
            ERR_print_errors_fp(log);
            fclose(log);
        }
        throw TLSSocketException(SSL_get_error(mOpenSSL_SSL, err),"Failed to connect to endpoint");
    } else if (err == 0) {
        throw TLSSocketException("Connection closed by peer");
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

    auto clientEndpoint = std::make_shared<Endpoint>(clientAddr, clientAddrSize);

    // Because the Constructor that is being called is Private, it cannot be called by std::make_shared
    auto Connection = std::shared_ptr<TLSSocket>(new TLSSocket(clientSocket, clientEndpoint, getProtocol()));

    SSL_set_fd(Connection->mOpenSSL_SSL, Connection->getNativeSocket());
    if(SSL_accept(Connection->mOpenSSL_SSL) <= 0) {
        throw SocketException("Failed to accept connection");
    }

    Connection->setState(SocketState::Connected);
    return Connection;
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