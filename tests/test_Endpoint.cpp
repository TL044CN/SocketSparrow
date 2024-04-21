#include "catch2/catch.hpp"
#include "Endpoint.hpp"

using namespace SocketSparrow;

TEST_CASE("Endpoint Creation", "[Endpoint]") {
    SECTION("Endpoint Creation with sockaddr and size") {
        sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8080);
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
        socklen_t size = sizeof(addr);
        REQUIRE_NOTHROW(Endpoint((sockaddr*)&addr, size));
    }
    SECTION("Endpoint Creation with hostname and port") {
        REQUIRE_NOTHROW(Endpoint("localhost", 8080));
    }
    SECTION("Endpoint Creation with ip and port") {
        REQUIRE_NOTHROW(Endpoint(in_addr_t(0x7F000001), 8080));
    }
    SECTION("Endpoint Creation with AddressFamily and port") {
        REQUIRE_NOTHROW(Endpoint(AddressFamily::IPv4, 8080));
    }
    SECTION("Endpoint Creation with sockaddr_storage and size") {
        sockaddr_storage addr;
        socklen_t size = sizeof(addr);
        REQUIRE_NOTHROW(Endpoint(addr, size));
    }

    SECTION("Endpoint Getters") {
        Endpoint endpoint("localhost", 8080);
        REQUIRE(endpoint.getAddressFamily() == AddressFamily::IPv4);
        REQUIRE(endpoint.getPort() == 8080);
        REQUIRE_NOTHROW(endpoint.c_addr());
        REQUIRE_NOTHROW(endpoint.c_size());
        REQUIRE(endpoint.c_size() == sizeof(sockaddr_in));
    }

    SECTION("Endpoint Destructor") {
        Endpoint* endpoint = new Endpoint("localhost", 8080);
        REQUIRE_NOTHROW(delete endpoint);
    }

    SECTION("Endpoint Validity") {
        Endpoint endpoint("localhost", 8080);
        REQUIRE(endpoint.getAddressFamily() == AddressFamily::IPv4);
        REQUIRE(endpoint.getPort() == 8080);

        sockaddr_in* addr = (sockaddr_in*)endpoint.c_addr();
        REQUIRE(addr->sin_addr.s_addr == htonl(INADDR_LOOPBACK));
        REQUIRE(addr->sin_port == htons(8080));
    }

}