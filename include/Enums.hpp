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

namespace SocketSparrow {

    /**
     * @brief Address Family of a Network Endpoint
     */
    enum class AddressFamily {
        IPv4,   ///< Internet Protocol Version 4
        IPv6    ///< Internet Protocol Version 6
    };

    /**
     * @brief Protocol of a Network Socket
     */
    enum class SocketType {
        STREAM,         ///< TCP (Transmission Control Protocol)
        DATAGRAM,       ///< UDP (User Datagram Protocol)
        TCP = STREAM,   ///< TCP (Transmission Control Protocol)
        UDP = DATAGRAM  ///< UDP (User Datagram Protocol)
    };

    /**
     * @brief State of a Network Socket
     */
    enum class SocketState {
        Closed,     ///< Not Open
        Open,       ///< Open but not Connected
        Listening,  ///< Listening for Incoming Connections
        Connected   ///< Connected to a Remote Endpoint
    };

} // namespace SocketSparrow