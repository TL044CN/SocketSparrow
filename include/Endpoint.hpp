/**
 * @file Endpoint.hpp
 * @author TL044CN
 * @brief Endpoint Class for SocketSparrow
 * @version 0.1
 * @date 2024-04-20
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "Enums.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <string>

namespace SocketSparrow {

/**
 * @brief Abstraction for a Network Endpoint
 */
class Endpoint {
private:
    union Sockaddr {
        sockaddr        base;
        sockaddr_in     ipv4;
        sockaddr_in6    ipv6;
    } mSockaddr;

    AddressFamily mAddressFamily;    

public:
    /**
     * @brief Construct a new Endpoint object
     * 
     * @param addr sockaddr to copy
     * @param size size of sockaddr
     */
    Endpoint(sockaddr* addr, socklen_t size);

    /**
     * @brief Construct a new Endpoint object
     * 
     * @param hostname the Hostname of the Endpoint
     * @param port the Port of the Endpoint
     */
    explicit Endpoint(std::string hostname = "localhost", uint16_t port = 80, AddressFamily af = AddressFamily::IPv4);

    /**
     * @brief Construct a new Endpoint object
     * 
     * @param ip the IP of the Endpoint
     * @param port the Port of the Endpoint
     */
    explicit Endpoint(in_addr_t ip, uint16_t port = 80);

    /**
     * @brief Construct a new Endpoint object
     * 
     * @param addr address to copy
     * @param size size of address
     */
    explicit Endpoint(sockaddr_storage addr, socklen_t size);

    /**
     * @brief Construct a new Endpoint object
     * 
     * @param af the AddressFamily of the Endpoint
     * @param port the Port of the Endpoint
     */
    explicit Endpoint(AddressFamily, uint16_t port = 80);

    /**
     * @brief Destroy the Endpoint
     */
    ~Endpoint();

    /**
     * @brief Get the AddressFamily of the Endpoint
     * 
     * @return AddressFamily the AddressFamily of the Endpoint
     */
    AddressFamily getAddressFamily() const;

    /**
     * @brief Get the Port of the Endpoint
     * 
     * @return uint16_t the Port of the Endpoint
     */
    int getPort() const;

    /**
     * @brief get the sockaddr* of the Endpoint
     * 
     * @return sockaddr* raw sockaddr struct of the Endpoint
     */
    const sockaddr* c_addr() const;

    /**
     * @brief get the size of the sockaddr struct
     * 
     * @return socklen_t size of the sockaddr struct
     */
    socklen_t c_size() const;

};

} // namespace SocketSparrow