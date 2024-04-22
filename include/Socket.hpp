/**
 * @file Socket.hpp
 * @author TL044CN
 * @brief Socket Class for SocketSparrow
 * @version 0.1
 * @date 2024-04-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "Enums.hpp"
#include "Endpoint.hpp"
#include "UDPPacket.hpp"

#include <vector>
#include <memory>
#include <sstream>

namespace SocketSparrow {

    /**
     * @brief Abstraction for a Network Socket
     */
    class Socket {
    private:
        /**
         * @brief  Helper Class to make sure the bool is explicit
         */
        class ExplicitBool final {
        private:
            bool mValue;

        public:
            /**
             * @brief Construct a new Explicit Bool object
             * 
             * @param value the value to set
             */
            explicit ExplicitBool(bool value) : mValue(value) {}

            /**
             * @brief   Get the value of the bool
             * @note    This is explicit
             * 
             * @return bool the value of the bool
             */
            operator bool() const { return mValue; }
        };

        int mNativeSocket;
        SocketType mProtocol;
        AddressFamily mAddressFamily;
        std::shared_ptr<Endpoint> mEndpoint;
        SocketState mState = SocketState::Unknown;

    /// Private Constructors

        /**
         * @brief Construct a new Socket object
         * 
         * @param fd file descriptor of the socket
         * @param endpoint the endpoint to use (contains address and port)
         * @param protocol the protocol to use (TCP/UDP)
         * @throws SocketException if the file descriptor is invalid
         */
        Socket(int fd, std::shared_ptr<Endpoint> endpoint, SocketType protocol);

    public:

    /// Public Constructors and Destructors
        /**
         * @brief Construct a new Socket object
         * 
         * @param af Address Family of the Socket
         * @param protocol Protocol of the Socket
         * @throws SocketException if creating the Socket fails
         */
        Socket(AddressFamily af, SocketType protocol);

        /**
         * @brief Construct a new Socket object and immediately binds it to an Endpoint
         * @note  This will create a TCP Socket
         * 
         * @param af Address Family of the Socket 
         * @param endpoint the endpoint to connect to
         * @throws SocketException if creating the Socket fails
         */
        Socket(AddressFamily af, std::shared_ptr<Endpoint> endpoint);

        /**
         * @brief Cleans up after the Socket is destroyed (e.g. closes the socket)
         */
        ~Socket();

    /// Public Methods
        /**
         * @brief   bind the Socket to an Endpoint.
         *          This Socket can be client or server
         * @note    This is only used for TCP Sockets
         * 
         * @param endpoint the endpoint to connect to
         * @throws SocketException if binding fails
         * @throws SocketException if the Socket is not a TCP Socket
         */
        void bind(std::shared_ptr<Endpoint> endpoint);

        /**
         * @brief   connect the Socket to the Port
         *          Useful when Listening to a Port
         *          This Socket will most likely be the server
         * 
         * @param port the port to listen to
         * @throws SocketException if binding fails
         */
        void bindToPort(uint16_t port);

        /**
         * @brief  connect the Socket to the Endpoint
         *         This Socket will be the client
         * @note   This is only works for TCP Sockets
         * 
         * @param endpoint the endpoint to connect to
         * @throws SocketException if the connection fails
         * @throws SocketException if the Socket is not a TCP Socket
         */
        void connect(std::shared_ptr<Endpoint> endpoint);

        /**
         * @brief   listen to the Socket
         *          This Socket will be the server
         * @note    This is only used for TCP Sockets
         * 
         * @param backlog the maximum number of connections
         * @throws SocketException if the Socket is not a TCP Socket
         */
        void listen(int backlog);

        /**
         * @brief   accept a connection
         *          this Socket has to be the server
         * @note    This is only used for TCP Sockets
         * 
         * @return std::shared_ptr<Endpoint> the Endpoint of the accepted connection
         * @throws SocketException if accepting fails
         * @throws SocketException if the Socket is not a TCP Socket
         */
        std::shared_ptr<Socket> accept();

        /**
         * @brief   Configure the Socket for broadcast mode (or disable it)
         * @note    when disabling broadcast, the Address will be set to Any(0)
         * 
         * @param enable true to enable broadcast, false to disable
         * @throws SocketException if setting the Configuration fails
         */
        void enableBroadcast(bool enable = true);

        /**
         * @brief   Configure the Socket for Reuse (or disable it)
         * @note    This is useful when the Socket is closed and reopened
         * @note    UDP benefits from this
         * 
         * @param enable true to enable reuse, false to disable
         * @throws SocketException if setting the Configuration fails
         */
        void enablePortReuse(bool enable = true);

        /**
         * @brief   Configure the Socket for Reuse (or disable it)
         * @note    This is useful when the Socket is closed and reopened
         * 
         * @param enable true to enable reuse, false to disable
         * @throws SocketException if setting the Configuration fails
         */
        void enableAddressReuse(bool enable = true);

        /**
         * @brief   Configure the Socket for Blocking (or non-blocking)
         * @note    This is useful when the Socket is used in a non-blocking way
         * 
         * @param enable true to enable blocking, false to disable
         * @throws SocketException if setting the Configuration fails
         */
        void enableNonBlocking(bool enable = true);

        /**
         * @brief   Sends data to the internal Socket
         *          This is used for TCP or UDP Sockets
         * @note    for TCP this socket should be returned from accept()
         * 
         * @param data the data to send
         * @return ssize_t the number of bytes sent
         * @throws SendError if sending fails
         * @see SocketSparrow::Socket::accept()
         */
        ssize_t send(std::vector<char> data) const;

        /**
         * @brief   Sends data to the internal Socket
         *          This is used for TCP or UDP Sockets
         * @note    for TCP this socket should be returned from accept()
         * 
         * @param data the data to send
         * @return ssize_t the number of bytes sent
         * @throws SendError if sending fails
         * @see SocketSparrow::Socket::accept()
         */
        ssize_t send(const std::string& data) const;

        /**
         * @brief   Receives data from the internal Socket
         *          This is used for TCP or UDP Sockets
         * @note    for TCP this socket should be returned from accept()
         * 
         * @param buffer the buffer to store the data. The buffer will be resized to fit the data
         * @param autoresize true to automatically resize the buffer, false to use the current size
         * @return ssize_t the number of bytes received
         * @throws RecvError if receiving fails
         * @see SocketSparrow::Socket::accept()
         */
        ssize_t recv(std::vector<char>& buffer, ExplicitBool autoresize = ExplicitBool(true)) const;

        /**
         * @brief   Receives data from the internal Socket
         *          This is used for TCP or UDP Sockets
         * @note    for TCP this socket should be returned from accept()
         * 
         * @param buffer the buffer to store the data
         * @param size the maximum size of the data
         * @return ssize_t the number of bytes received
         * @throws RecvError if receiving fails
         * @see SocketSparrow::Socket::accept()
         */
        ssize_t recv(std::vector<char>& buffer, size_t size) const;

        /**
         * @brief   Receives data from the internal Socket
         *          This is used for TCP or UDP Sockets
         * @note    for TCP this socket should be returned from accept()
         * 
         * @param buffer the buffer to store the data
         * @return ssize_t the number of bytes received
         * @throws RecvError if receiving fails
         * @see SocketSparrow::Socket::accept()
         */
        ssize_t recv(std::string& buffer) const;

        /**
         * @brief   Sends a UDP Packet to the internal Socket
         * @note    this only works with UDP Sockets
         * 
         * @param data the data to send
         * @param endpoint the endpoint to send the data to
         * @return ssize_t the number of bytes sent
         * @throws SendError if sending fails
         * @see SocketSparrow::Socket::send()
         */
        ssize_t sendTo(std::vector<char> data, std::shared_ptr<Endpoint> endpoint);

        /**
         * @brief   Sends a UDP Packet to the internal Socket
         * @note    this only works with UDP Sockets
         * 
         * @param packet the packet to send
         * @return ssize_t the number of bytes sent
         * @throws SendError if sending fails
         * @see SocketSparrow::Socket::send()
         */
        ssize_t sendTo(UDPPacket packet);

        /**
         * @brief   Receives a UDP Packet from the internal Socket
         * @note    this only works with UDP Sockets
         * 
         * @return UDPPacket the received packet
         * @throws RecvError if receiving fails
         * @see SocketSparrow::Socket::recv()
         */
        UDPPacket recvFrom() const;

    /// Operators

        /**
         * @brief   Sends data to the internal Socket
         *          This is used for TCP or UDP Sockets
         * 
         * @param data the data to send
         * @return ssize_t the number of bytes sent
         * @throws SendError if sending fails
         * @see SocketSparrow::Socket::send()
         */
        ssize_t operator<<(const std::vector<char>& data) const;

        /**
         * @brief   Sends data to the internal Socket
         *          This is used for TCP or UDP Sockets
         * 
         * @param data the data to send
         * @return ssize_t the number of bytes sent
         * @throws SendError if sending fails
         * @see SocketSparrow::Socket::send()
         */
        ssize_t operator<<(const std::string& data) const;

        /**
         * @brief   Receives data from the internal Socket
         *          This is used for TCP or UDP Sockets
         * 
         * @param buffer the buffer to store the data
         * @return ssize_t the number of bytes received
         * @throws RecvError if receiving fails
         * @see SocketSparrow::Socket::recv()
         */
        ssize_t operator>>(std::vector<char>& buffer) const;

        /**
         * @brief   Receives data from the internal Socket
         *          This is used for TCP or UDP Sockets
         * 
         * @param buffer the buffer to store the data
         * @return ssize_t the number of bytes received
         * @throws RecvError if receiving fails
         * @see SocketSparrow::Socket::recv()
         */
        ssize_t operator>>(std::string& buffer) const;

        /**
         * @brief   Receives data from the internal Socket
         *          This is used for TCP or UDP Sockets
         * 
         * @param stream the stream to store the data
         * @return ssize_t the number of bytes received
         * @throws RecvError if receiving fails
         * @see SocketSparrow::Socket::recv()
         */
        ssize_t operator>>(std::stringstream& stream) const;

    };

} // namespace SocketSparrow
