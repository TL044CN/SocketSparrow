#include "Endpoint.hpp"
#include "Util.hpp"

#include <cstring>

namespace SocketSparrow {

Endpoint::Endpoint(sockaddr* addr, socklen_t size) {
    memcpy(&mSockaddr.base, addr, size);
    mAddressFamily = Util::getAddressFamily(mSockaddr.base.sa_family);
}

Endpoint::Endpoint(std::string hostname, uint16_t port) {
    mAddressFamily = AddressFamily::IPv4;
    mSockaddr.ipv4.sin_family = Util::getNativeAddressFamily(mAddressFamily);
    mSockaddr.ipv4.sin_port = htons(port);
    mSockaddr.ipv4.sin_addr.s_addr = inet_addr(hostname.c_str());
}

Endpoint::Endpoint(in_addr_t ip, uint16_t port) {
    mAddressFamily = AddressFamily::IPv4;
    mSockaddr.ipv4.sin_family = Util::getNativeAddressFamily(mAddressFamily);
    mSockaddr.ipv4.sin_port = htons(port);
    mSockaddr.ipv4.sin_addr.s_addr = ip;
}

Endpoint::Endpoint(AddressFamily af, uint16_t port) {
    mAddressFamily = af;
    mSockaddr.ipv4.sin_family = Util::getNativeAddressFamily(mAddressFamily);
    mSockaddr.ipv4.sin_port = htons(port);
    mSockaddr.ipv4.sin_addr.s_addr = htonl(INADDR_ANY);
}

Endpoint::Endpoint(sockaddr_storage addr, socklen_t size) {
    memcpy(&mSockaddr.base, &addr, size);
    mAddressFamily = Util::getAddressFamily(mSockaddr.base.sa_family);
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
    switch (mAddressFamily) {
        case AddressFamily::IPv4: return sizeof(sockaddr_in);
        case AddressFamily::IPv6: return sizeof(sockaddr_in6);
        case AddressFamily::Unknown: return sizeof(sockaddr);
    }
    return 0;
}

}   // namespace SocketSparrow