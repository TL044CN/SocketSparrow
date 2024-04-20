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
        IPv4,
        IPv6
    };

    /**
     * @brief Protocol of a Network Socket
     */
    enum class SocketType {
        STREAM,
        DATAGRAM,
        TCP = STREAM,
        UDP = DATAGRAM
    };

    /**
     * @brief State of a Network Socket
     */
    enum class SocketState {
        Closed,
        Open,
        Listening,
        Connected
    };

} // namespace SocketSparrow