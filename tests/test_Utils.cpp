#include "catch2/catch.hpp"
#include "Util.hpp"

using namespace SocketSparrow;
using namespace SocketSparrow::Util;

TEST_CASE("Test Utils", "[Utils]") {
    SECTION("Test AddressFamily") {
        REQUIRE(getAddressFamily("AF_INET") == AddressFamily::IPv4);
        REQUIRE(getAddressFamily("AF_INET6") == AddressFamily::IPv6);
        REQUIRE(getAddressFamily("AF_UNSPEC") == AddressFamily::Unknown);
        REQUIRE(getAddressFamily("AF_UNIX") == AddressFamily::Unknown);
        REQUIRE(getAddressFamily("AF_APPLETALK") == AddressFamily::Unknown);
        REQUIRE(getAddressFamily("AF_PACKET") == AddressFamily::Unknown);
        REQUIRE(getNativeAddressFamily(AddressFamily::IPv4) == AF_INET);
        REQUIRE(getNativeAddressFamily(AddressFamily::IPv6) == AF_INET6);
        REQUIRE(getNativeAddressFamily(AddressFamily::Unknown) == AF_UNSPEC);
        REQUIRE(getAddressFamily(AF_INET) == AddressFamily::IPv4);
        REQUIRE(getAddressFamily(AF_INET6) == AddressFamily::IPv6);
        REQUIRE(getAddressFamily(AF_UNSPEC) == AddressFamily::Unknown);

        REQUIRE(getAddressFamilyString(AddressFamily::IPv4) == "AF_INET");
        REQUIRE(getAddressFamilyString(AddressFamily::IPv6) == "AF_INET6");
        REQUIRE(getAddressFamilyString(AddressFamily::Unknown) == "AF_UNSPEC");
        REQUIRE(getAddressFamilyString(static_cast<AddressFamily>(331)) == "AF_UNSPEC");

        REQUIRE(getAddressFamily("SomeRandomString") == AddressFamily::Unknown);
        REQUIRE(getAddressFamily(331) == AddressFamily::Unknown);
    }

    SECTION("Test SocketType") {
        REQUIRE(getSocketType("TCP") == SocketType::TCP);
        REQUIRE(getSocketType("SOCK_STREAM") == SocketType::TCP);
        REQUIRE(getSocketType("UDP") == SocketType::UDP);
        REQUIRE(getSocketType("SOCK_DGRAM") == SocketType::UDP);
        REQUIRE(getSocketType("SOCK_RAW") == SocketType::Unknown);
        REQUIRE(getSocketType("Unknown") == SocketType::Unknown);
        REQUIRE(getSocketType(SOCK_STREAM) == SocketType::TCP);
        REQUIRE(getSocketType(SOCK_DGRAM) == SocketType::UDP);
        REQUIRE(getSocketType(SOCK_RAW) == SocketType::Unknown);
        REQUIRE(getNativeSocketType(SocketType::TCP) == SOCK_STREAM);
        REQUIRE(getNativeSocketType(SocketType::Stream) == SOCK_STREAM);
        REQUIRE(getNativeSocketType(SocketType::UDP) == SOCK_DGRAM);
        REQUIRE(getNativeSocketType(SocketType::Datagram) == SOCK_DGRAM);
        REQUIRE(getNativeSocketType(SocketType::Unknown) == SOCK_RAW);

        REQUIRE(getSocketTypeString(SocketType::TCP) == "TCP");
        REQUIRE(getSocketTypeString(SocketType::Stream) == "TCP");
        REQUIRE(getSocketTypeString(SocketType::UDP) == "UDP");
        REQUIRE(getSocketTypeString(SocketType::Datagram) == "UDP");
        REQUIRE(getSocketTypeString(SocketType::Unknown) == "SOCK_RAW");
        REQUIRE(getSocketTypeString(static_cast<SocketType>(331)) == "SOCK_RAW");

        REQUIRE(getSocketType("SomeRandomString") == SocketType::Unknown);
        REQUIRE(getSocketType(331) == SocketType::Unknown);
    }

    SECTION("Test SocketState") {
        REQUIRE(getSocketState("Closed") == SocketState::Closed);
        REQUIRE(getSocketState("Open") == SocketState::Open);
        REQUIRE(getSocketState("Listening") == SocketState::Listening);
        REQUIRE(getSocketState("Connected") == SocketState::Connected);
        REQUIRE(getSocketState("Disconnected") == SocketState::Disconnected);
        REQUIRE(getSocketState("Unknown") == SocketState::Unknown);
        REQUIRE(getSocketState("SomeRandomString") == SocketState::Unknown);

        REQUIRE(getSocketStateString(SocketState::Closed) == "Closed");
        REQUIRE(getSocketStateString(SocketState::Open) == "Open");
        REQUIRE(getSocketStateString(SocketState::Listening) == "Listening");
        REQUIRE(getSocketStateString(SocketState::Connected) == "Connected");
        REQUIRE(getSocketStateString(SocketState::Disconnected) == "Disconnected");
        REQUIRE(getSocketStateString(SocketState::Unknown) == "Unknown");
        REQUIRE(getSocketStateString(static_cast<SocketState>(331)) == "Unknown");
    }

}