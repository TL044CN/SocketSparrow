#include "Endpoint.hpp"
#include "Util.hpp"
#include "Exceptions.hpp"

#include <cstring>
#include <assert.h>


namespace SocketSparrow {

Endpoint::Endpoint(sockaddr* addr, socklen_t size) {
    if( addr == nullptr ) {
        throw InvalidAddressException();
    }

    if(addr->sa_family == AF_INET && size != sizeof(sockaddr_in)) {
        throw InvalidAddressException();
    } else if(addr->sa_family == AF_INET6 && size != sizeof(sockaddr_in6)) {
        throw InvalidAddressException();
    } else if(addr->sa_family != AF_INET && addr->sa_family != AF_INET6) {
        throw InvalidAddressFamilyException();
    }

    memcpy(&mSockaddr.base, addr, size);
    mAddressFamily = Util::getAddressFamily(mSockaddr.base.sa_family);
}

Endpoint::Endpoint(const std::string& hostname, uint16_t port, AddressFamily af)
    : mAddressFamily(af) {

    struct addrinfo hints = {};
    hints.ai_family = Util::getNativeAddressFamily(mAddressFamily);

    switch ( mAddressFamily ) {
    case AddressFamily::IPv4:
    {

        mSockaddr.ipv4.sin_family = hints.ai_family;
        mSockaddr.ipv4.sin_port = htons(port);

        struct addrinfo* res;
        if ( getaddrinfo(hostname.c_str(), NULL, &hints, &res) != 0 ) {
            throw InvalidAddressException(hostname, "Failed to resolve hostname");
        }
        struct sockaddr_in* ipv4 = reinterpret_cast<struct sockaddr_in*>(res->ai_addr);
        mSockaddr.ipv4.sin_addr.s_addr = ipv4->sin_addr.s_addr;
        freeaddrinfo(res);
    }
    break;
    case AddressFamily::IPv6:
    {
        mSockaddr.ipv6.sin6_family = hints.ai_family;
        mSockaddr.ipv6.sin6_port = htons(port);

        struct addrinfo* res6;
        if ( getaddrinfo(hostname.c_str(), NULL, &hints, &res6) != 0 ) {
            throw InvalidAddressException(hostname, "Failed to resolve hostname");
        }
        struct sockaddr_in6* ipv6 = reinterpret_cast<struct sockaddr_in6*>(res6->ai_addr);
        mSockaddr.ipv6.sin6_addr = ipv6->sin6_addr;
        freeaddrinfo(res6);
    }
    break;
    default:
        throw InvalidAddressFamilyException(af);
    }

}

Endpoint::Endpoint(in_addr_t ip, uint16_t port) {
    mAddressFamily = AddressFamily::IPv4;
    mSockaddr.ipv4.sin_family = Util::getNativeAddressFamily(mAddressFamily);
    mSockaddr.ipv4.sin_port = htons(port);
    mSockaddr.ipv4.sin_addr.s_addr = ip;

    if( ip == INADDR_NONE || ip == 0) {
        throw InvalidAddressException("Invalid IP Address");
    }
}

Endpoint::Endpoint(AddressFamily af, uint16_t port)
    : mAddressFamily(af) {
    switch ( mAddressFamily ) {
    case AddressFamily::IPv4:
        mSockaddr.ipv4.sin_family = Util::getNativeAddressFamily(mAddressFamily);
        mSockaddr.ipv4.sin_port = htons(port);
        mSockaddr.ipv4.sin_addr.s_addr = htonl(INADDR_ANY);
        break;
    case AddressFamily::IPv6:
        mSockaddr.ipv6.sin6_family = Util::getNativeAddressFamily(mAddressFamily);
        mSockaddr.ipv6.sin6_port = htons(port);
        mSockaddr.ipv6.sin6_addr = in6addr_any;
        break;
    default:
        throw InvalidAddressFamilyException(mAddressFamily, "Invalid Address Family");
    }

}

Endpoint::Endpoint(sockaddr_storage addr, socklen_t size) {
    memcpy(&mSockaddr.base, &addr, size);
    mAddressFamily = Util::getAddressFamily(mSockaddr.base.sa_family);

    if ( mAddressFamily == AddressFamily::Unknown ) {
        throw InvalidAddressException();
    }
}

Endpoint::~Endpoint() {

}


AddressFamily Endpoint::getAddressFamily() const {
    return mAddressFamily;
}

int Endpoint::getPort() const {
    return ntohs(mSockaddr.ipv4.sin_port);
}

const sockaddr* Endpoint::c_addr() const {
    return &mSockaddr.base;
}

socklen_t Endpoint::c_size() const {
    switch ( mAddressFamily ) {
        case AddressFamily::IPv4: return sizeof(sockaddr_in);
        case AddressFamily::IPv6: return sizeof(sockaddr_in6);
        default: throw InvalidAddressFamilyException(mAddressFamily);
    }
}

}   // namespace SocketSparrow