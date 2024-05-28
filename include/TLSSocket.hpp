/**
 * @file TLSSocket.hpp
 * @author TL044CN (tl044cn@shodan.fyi)
 * @brief TLS Socket Class
 * @version 0.1
 * @date 2024-05-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include "Socket.hpp"
#include <openssl/ssl.h>

namespace SocketSparrow {

    /**
     * @brief Abstract for TLS Socket
     * 
     */
    class TLSSocket : public Socket {
    private:
        SSL_CTX* mOpenSSL_CTX;
        SSL* mOpenSSL_SSL;

    protected:
            /**
         * @brief Construct a new Socket object
         * 
         * @param fd file descriptor of the socket
         * @param endpoint the endpoint to use (contains address and port)
         * @param protocol the protocol to use (TCP/UDP)
         * @throws SocketException if the file descriptor is invalid
         */
        TLSSocket(int fd, std::shared_ptr<Endpoint> endpoint, SocketType protocol);

    public:
    /// Constructors
        /**
         * @brief Construct a new TLSSocket object
         * 
         * @param af Address Family
         * @param protocol Socket Type
         */
        TLSSocket(AddressFamily af, SocketType protocol);

        /**
         * @brief Construct a new TLSSocket object
         * 
         * @param af Address Family
         * @param endpoint Endpoint
         */
        TLSSocket(AddressFamily af, std::shared_ptr<Endpoint> endpoint);

    /// Destructor
        /**
         * @brief Destroy the TLSSocket object
         */
        ~TLSSocket();

    /// Public Methods
        /**
         * @brief Connect to an endpoint
         * 
         * @param endpoint Endpoint
         */
        void connect(std::shared_ptr<Endpoint> endpoint) override;

        /**
         * @brief accept a connection
         *        this Socket has to be the server
         * @note  This is only used for TCP Sockets
         * 
         * @return std::shared_ptr<Socket> the Socket of the accepted connection
         * @throws SocketException if the accept fails
         * @throws SocketException if the Socket is not a TCP Socket
         * @see SocketSparrow::Socket::accept()
         */
        std::shared_ptr<Socket> accept() override;

        /**
         * @brief Sends data to the internal Socket
         *        this is used for TCP or UDP Sockets
         * @note  for TCP Servers this Socket should be returned from accept()
         * 
         * @param data the data to send
         * @return ssize_t the number of bytes sent
         * @throws SendError if sending fails
         * @see SocketSparrow::Socket::accept()
         * @see SocketSparrow::Socket::send()
         */
        ssize_t send(const std::vector<char>& data) const override;

        /**
         * @brief Sends data to the internal Socket
         *        this is used for TCP or UDP Sockets
         * @note  for TCP Servers this Socket should be returned from accept()
         * 
         * @param data the data to send
         * @return ssize_t the number of bytes sent
         * @throws SendError if sending fails
         * @see SocketSparrow::Socket::accept()
         * @see SocketSparrow::Socket::send()
         */
        ssize_t send(const std::string& data) const override;

        /**
         * @brief Receives data from the internal Socket
         *        this is used for TCP or UDP Sockets
         * 
         * @param buffer the buffer to store the data
         * @param autoresize if the buffer should be resized
         * @return ssize_t the number of bytes received
         * @throws RecvError if receiving fails
         * @see SocketSparrow::Socket::recv()
         */
        ssize_t recv(std::vector<char>& buffer, ExplicitBool autoresize = ExplicitBool(true)) const override;

        /**
         * @brief Receives data from the internal Socket
         *        this is used for TCP or UDP Sockets
         * 
         * @param buffer the buffer to store the data
         * @param size the size of the buffer
         * @return ssize_t the number of bytes received
         * @throws RecvError if receiving fails
         * @see SocketSparrow::Socket::recv()
         */
        ssize_t recv(std::vector<char>& buffer, size_t size) const override;

        /**
         * @brief Receives data from the internal Socket
         *        this is used for TCP or UDP Sockets
         * 
         * @param buffer the buffer to store the data
         * @return ssize_t the number of bytes received
         * @throws RecvError if receiving fails
         * @see SocketSparrow::Socket::recv()
         */
        ssize_t recv(std::string& buffer) const override;

    };
} // namespace SocketSparrow