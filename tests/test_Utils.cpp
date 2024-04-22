#include "catch2/catch_test_macros.hpp"
#include "Util.hpp"

using namespace SocketSparrow;
using namespace SocketSparrow::Util;

TEST_CASE("Test Utils", "[Utils]") {
    SECTION("Test AddressFamily") {
        CHECK(getAddressFamily("AF_INET") == AddressFamily::IPv4);
        CHECK(getAddressFamily("AF_INET6") == AddressFamily::IPv6);
        CHECK(getAddressFamily("AF_UNSPEC") == AddressFamily::Unknown);
        CHECK(getAddressFamily("AF_UNIX") == AddressFamily::Unknown);
        CHECK(getAddressFamily("AF_APPLETALK") == AddressFamily::Unknown);
        CHECK(getAddressFamily("AF_PACKET") == AddressFamily::Unknown);
        CHECK(getNativeAddressFamily(AddressFamily::IPv4) == AF_INET);
        CHECK(getNativeAddressFamily(AddressFamily::IPv6) == AF_INET6);
        CHECK(getNativeAddressFamily(AddressFamily::Unknown) == AF_UNSPEC);
        CHECK(getAddressFamily(AF_INET) == AddressFamily::IPv4);
        CHECK(getAddressFamily(AF_INET6) == AddressFamily::IPv6);
        CHECK(getAddressFamily(AF_UNSPEC) == AddressFamily::Unknown);

        CHECK(getAddressFamilyString(AddressFamily::IPv4) == "AF_INET");
        CHECK(getAddressFamilyString(AddressFamily::IPv6) == "AF_INET6");
        CHECK(getAddressFamilyString(AddressFamily::Unknown) == "AF_UNSPEC");
        CHECK(getAddressFamilyString(static_cast<AddressFamily>(331)) == "AF_UNSPEC");

        CHECK(getAddressFamily("SomeRandomString") == AddressFamily::Unknown);
        CHECK(getAddressFamily(331) == AddressFamily::Unknown);
    }

    SECTION("Test SocketType") {
        CHECK(getSocketType("TCP") == SocketType::TCP);
        CHECK(getSocketType("SOCK_STREAM") == SocketType::TCP);
        CHECK(getSocketType("UDP") == SocketType::UDP);
        CHECK(getSocketType("SOCK_DGRAM") == SocketType::UDP);
        CHECK(getSocketType("SOCK_RAW") == SocketType::Unknown);
        CHECK(getSocketType("Unknown") == SocketType::Unknown);
        CHECK(getSocketType(SOCK_STREAM) == SocketType::TCP);
        CHECK(getSocketType(SOCK_DGRAM) == SocketType::UDP);
        CHECK(getSocketType(SOCK_RAW) == SocketType::Unknown);
        CHECK(getNativeSocketType(SocketType::TCP) == SOCK_STREAM);
        CHECK(getNativeSocketType(SocketType::Stream) == SOCK_STREAM);
        CHECK(getNativeSocketType(SocketType::UDP) == SOCK_DGRAM);
        CHECK(getNativeSocketType(SocketType::Datagram) == SOCK_DGRAM);
        CHECK(getNativeSocketType(SocketType::Unknown) == SOCK_RAW);

        CHECK(getSocketTypeString(SocketType::TCP) == "TCP");
        CHECK(getSocketTypeString(SocketType::Stream) == "TCP");
        CHECK(getSocketTypeString(SocketType::UDP) == "UDP");
        CHECK(getSocketTypeString(SocketType::Datagram) == "UDP");
        CHECK(getSocketTypeString(SocketType::Unknown) == "SOCK_RAW");
        CHECK(getSocketTypeString(static_cast<SocketType>(331)) == "SOCK_RAW");

        CHECK(getSocketType("SomeRandomString") == SocketType::Unknown);
        CHECK(getSocketType(331) == SocketType::Unknown);
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