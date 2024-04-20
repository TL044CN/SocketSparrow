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
namespace SocketSparrow {

    struct SocketData {
        int socket;
        int port;
        AddressFamily family;
        SocketType  protocol;
        SocketState state;
    };

    /**
     * @brief Abstraction for a Network Socket
     */
    class Socket {
    private:
        SocketData mData;

    public:

        /**
         * @brief Construct a new Socket object
         * 
         */
        Socket();

        /**
         * @brief Destroy the Socket object
         * 
         */
        ~Socket();

        /**
         * @brief Bind the Socket to a specific port
         * 
         * @param port Port to bind to
         */
        void bind(int port);

        /**
         * @brief Listen for incoming connections
         * 
         * @param backlog Maximum number of connections to queue
         */
        void listen(int backlog);

        /**
         * @brief Accept an incoming connection
         * 
         * @return int File Descriptor of the new connection
         */
        int accept();

        /**
         * @brief Connect to a remote endpoint
         * 
         * @param address Address of the remote endpoint
         * @param port Port of the remote endpoint
         */
        void connect(const char* address, int port);

        /**
         * @brief Send data to the connected endpoint
         * 
         * @param data Data to send
         * @param size Size of the data
         */
        void send(const char* data, int size);

        /**
         * @brief Receive data from the connected endpoint
         * 
         * @param buffer Buffer to store the received data
         * @param size Size of the buffer
         * @return int Number of bytes received
         */
        int receive(char* buffer, int size);

        /**
         * @brief Close the Socket
         * 
         */
        void close();

    };

} // namespace SocketSparrow
