#include "Util.hpp"
#include <tuple>
#include <assert.h>

namespace SocketSparrow::Util {

/**
 * @brief maps String to AddressFamily
 ^ @details This is a mapping of AddressFamily enum values to their string representations.
 */
constexpr std::pair<AddressFamily, const char*> addressFamilyMapping[] = {
    {AddressFamily::IPv4, "AF_INET"},
    {AddressFamily::IPv6, "AF_INET6"},
    {AddressFamily::Unknown, "AF_UNSPEC"}
};

/**
 * @brief maps String to SocketType
 * @details This is a mapping of SocketType enum values to their string representations.
 */
constexpr std::pair<SocketType, const char*> socketTypeMapping[] = {
    {SocketType::TCP, "TCP"},
    {SocketType::TCP, "SOCK_STREAM"},
    {SocketType::UDP, "UDP"},
    {SocketType::UDP, "SOCK_DGRAM"},
    {SocketType::Unknown, "SOCK_RAW"},
    {SocketType::Unknown, "Unknown"}
};

/**
 * @brief maps String to SocketState
 * @details This is a mapping of SocketState enum values to their string representations.
 */
constexpr std::pair<SocketState, const char*> socketStateStrings[] = {
    {SocketState::Closed, "Closed"},
    {SocketState::Open, "Open"},
    {SocketState::Listening, "Listening"},
    {SocketState::Connected, "Connected"},
    {SocketState::Disconnected, "Disconnected"},
    {SocketState::Unknown, "Unknown"}
};

AddressFamily getAddressFamily(const std::string& family) {
    for ( const auto& [f, __] : addressFamilyMapping ) {
        if ( family == __ ) {
            return f;
        }
    }
    return AddressFamily::Unknown;
}

AddressFamily getAddressFamily(int nativeFamily) {
    switch (nativeFamily){
        case AF_INET:
        case AF_INET6:
        case AF_UNSPEC:
            return static_cast<AddressFamily>(nativeFamily);
        default:
            break;
    }
    return AddressFamily::Unknown;
}

int getNativeAddressFamily(AddressFamily family) {
    return static_cast<int>(family);
}

SocketType getSocketType(const std::string& type) {
    for ( const auto& [t, __] : socketTypeMapping ) {
        if ( type == __ ) {
            return t;
        }
    }
    return SocketType::Unknown;
}

SocketType getSocketType(int type) {
    switch (type){
        case SOCK_STREAM:
        case SOCK_DGRAM:
        case SOCK_RAW:
            return static_cast<SocketType>(type);
        default:
            break;
    }
    return SocketType::Unknown;
}

int getNativeSocketType(SocketType type) {
    return static_cast<int>(type);
}

SocketState getSocketState(const std::string& state) {
    for ( const auto& [s, __] : socketStateStrings ) {
        if ( state == __ ) {
            return s;
        }
    }
    return SocketState::Unknown;
}

std::string getAddressFamilyString(AddressFamily family) {
    for ( const auto& [f, __] : addressFamilyMapping ) {
        if ( family == f ) {
            return __;
        }
    }
    return "AF_UNSPEC";
}

std::string getSocketTypeString(SocketType type) {
    for ( const auto& [t, __] : socketTypeMapping ) {
        if ( type == t ) {
            return __;
        }
    }
    return "SOCK_RAW";
}

std::string getSocketStateString(SocketState state) {
    for ( const auto& [s, __] : socketStateStrings ) {
        if ( state == s ) {
            return __;
        }
    }
    return "Unknown";
}

}   // namespace SocketSparrow::Util