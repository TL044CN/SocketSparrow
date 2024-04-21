#include "catch2/catch.hpp"
#include "Endpoint.hpp"
#include "Exceptions.hpp"

#include <cstring>

using namespace SocketSparrow;

TEST_CASE("Endpoint Creation", "[Endpoint]") {
    SECTION("Endpoint Creation with sockaddr and size") {
        sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8080);
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
        socklen_t size = sizeof(addr);
        REQUIRE_NOTHROW(Endpoint((sockaddr*)&addr, size));

        sockaddr_in6 addr6 = {};
        addr6.sin6_family = AF_INET6;
        addr6.sin6_port = htons(8080);
        inet_pton(AF_INET6, "::1", &addr6.sin6_addr);
        size = sizeof(addr6);
        REQUIRE_NOTHROW(Endpoint((sockaddr*)&addr6, size));

        SECTION("Endpoint Creation with invalid hostname") {
            REQUIRE_THROWS_AS(Endpoint("invalid_hostname", 8080), InvalidAddressException);
        }

        SECTION("Endpoint Creation with invalid IP address") {
            REQUIRE_THROWS_AS(Endpoint(inet_addr("invalid_ip"), 8080), InvalidAddressException);
        }

        SECTION("Endpoint Creation with invalid AddressFamily") {
            REQUIRE_THROWS_AS(Endpoint(AddressFamily::Unknown, 8080), InvalidAddressFamilyException);
        }

        SECTION("Endpoint Creation with invalid sockaddr_storage") {
            sockaddr_storage addr;
            socklen_t size = sizeof(addr);
            REQUIRE_THROWS_AS(Endpoint(addr, size), InvalidAddressException);
        }

    }

    SECTION("Endpoint Creation with hostname and port") {
        REQUIRE_NOTHROW(Endpoint("localhost", 8080));
    }

    SECTION("Endpoint Creation with ip and port") {
        REQUIRE_NOTHROW(Endpoint(in_addr_t(0x7F000001), 8080));
        REQUIRE_NOTHROW(Endpoint(inet_addr("127.0.0.1"), 8080));
    }

    SECTION("Endpoint Creation with AddressFamily and port") {
        REQUIRE_NOTHROW(Endpoint(AddressFamily::IPv4, 8080));
    }

    SECTION("Get AddressFamily") {
        Endpoint endpoint("localhost", 8080);
        REQUIRE(endpoint.getAddressFamily() == AddressFamily::IPv4);
    }

    SECTION("Get Port") {
        Endpoint endpoint("localhost", 8080);
        REQUIRE(endpoint.getPort() == 8080);
    }

    SECTION("Get sockaddr") {
        sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8080);
        inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr));
        socklen_t size = sizeof(addr);
        Endpoint endpoint((sockaddr*)&addr, size);
        REQUIRE(memcmp(endpoint.c_addr(), &addr, size) == 0);
    }

    SECTION("Get size of sockaddr") {
        sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8080);
        inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr));
        socklen_t size = sizeof(addr);
        Endpoint endpoint((sockaddr*)&addr, size);
        REQUIRE(endpoint.c_size() == size);
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