/**
 * @file UDPPacket.hpp
 * @author TL044CN
 * @brief UDP Packet Class for SocketSparrow
 * @version 0.1
 * @date 2024-04-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "Endpoint.hpp"

#include <vector>
#include <memory>

namespace SocketSparrow {

    constexpr size_t MAX_UDP_PACKET_SIZE = 65507;

    /**
     * @brief Abstraction for a UDP Packet
     */
    struct UDPPacket {
        std::vector<char> data;
        std::shared_ptr<Endpoint> endpoint;

        UDPPacket(size_t size = MAX_UDP_PACKET_SIZE) : data(size), endpoint(nullptr) {}
        UDPPacket(const std::vector<char>& _data, std::shared_ptr<Endpoint> _endpoint = nullptr)
        : data(_data), endpoint(_endpoint) {}
    };

} // namespace SocketSparrow