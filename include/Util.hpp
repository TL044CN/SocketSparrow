/**
 * @file Util.hpp
 * @author TL044CN
 * @brief SocketSparrow Utility Functions
 * @version 0.1
 * @date 2024-04-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#include "Enums.hpp"

#include <string>
#include <arpa/inet.h>

/**
 * @brief SocketSparrow Utility Functions
 */
namespace SocketSparrow::Util {

    /**
     * @brief Get the Address Family from a string
     * 
     * @param family Address Family as a string
     * @return AddressFamily Address Family as an enum
     */
    AddressFamily getAddressFamily(const std::string& family);

    /**
     * @brief Get the Address Family from an integer (AF_INET, AF_INET6, etc.)
     * 
     * @param family Address Family as an integer (AF_INET, AF_INET6, etc.)
     * @return AddressFamily Address Family as an enum
     */
    AddressFamily getAddressFamily(int nativeFamily);

    /**
     * @brief Get the integer representation of an Address Family
     * 
     * @param family Address Family as an enum
     * @return int Address Family as an integer
     */
    int getNativeAddressFamily(AddressFamily family);

    /**
     * @brief Get the Socket Type from a string
     * 
     * @param type Socket Type as a string
     * @return SocketType Socket Type as an enum
     */
    SocketType getSocketType(const std::string& type);

    /**
     * @brief Get the Socket Type from an integer (SOCK_STREAM, SOCK_DGRAM, etc.)
     * 
     * @param type Socket Type as an integer (SOCK_STREAM, SOCK_DGRAM, etc.)
     * @return SocketType Socket Type as an enum
     */
    SocketType getSocketType(int nativeType);

    /**
     * @brief Get the integer representation of a Socket Type
     * 
     * @param type Socket Type as an enum
     * @return int Socket Type as an integer
     */
    int getNativeSocketType(SocketType type);

    /**
     * @brief Get the Socket State from a string
     * 
     * @param state Socket State as a string
     * @return SocketState Socket State as an enum
     */
    SocketState getSocketState(const std::string& state);

    /**
     * @brief Get the string representation of an Address Family
     * 
     * @param family Address Family as an enum
     * @return std::string Address Family as a string
     */
    std::string getAddressFamilyString(AddressFamily family);

    /**
     * @brief Get the string representation of a Socket Type
     * 
     * @param type Socket Type as an enum
     * @return std::string Socket Type as a string
     */
    std::string getSocketTypeString(SocketType type);

    /**
     * @brief Get the string representation of a Socket State
     * 
     * @param state Socket State as an enum
     * @return std::string Socket State as a string
     */
    std::string getSocketStateString(SocketState state);

} // namespace SocketSparrow::Util
