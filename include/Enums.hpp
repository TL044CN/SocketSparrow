/**
 * @file Enums.hpp
 * @author TL044CN
 * @brief Enum Collection for SocketSparrow
 * @version 0.1
 * @date 2024-04-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include <arpa/inet.h>

namespace SocketSparrow {

    /**
     * @brief Address Family of a Network Endpoint
     */
    enum class AddressFamily {
        Unknown = AF_UNSPEC, ///< Unknown Address Family
        IPv4    = AF_INET,   ///< Internet Protocol Version 4
        IPv6    = AF_INET6   ///< Internet Protocol Version 6
    };

    /**
     * @brief Protocol of a Network Socket
     */
    enum class SocketType {
        TCP         = SOCK_STREAM,///< TCP (Transmission Control Protocol)
        UDP         = SOCK_DGRAM, ///< UDP (User Datagram Protocol)
        Unknown     = SOCK_RAW,   ///< Unknown Protocol
        Stream      = TCP,        ///< TCP (Transmission Control Protocol)
        Datagram    = UDP         ///< UDP (User Datagram Protocol)
    };

    /**
     * @brief State of a Network Socket
     */
    enum class SocketState {
        Closed,      ///< Not Open
        Open,        ///< Open but not Connected
        Listening,   ///< Listening for Incoming Connections
        Connected,   ///< Connected to a Remote Endpoint
        Disconnected,///< Disconnected from a Remote Endpoint
        Unknown      ///< Unknown State
    };

} // namespace SocketSparrow