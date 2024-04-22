#include "catch2/catch_all.hpp"
#include "Endpoint.hpp"
#include "Exceptions.hpp"
#include "Util.hpp"
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
    }

    SECTION("Endpoint Creation with invalid sockaddr and size") {
        REQUIRE_THROWS_AS(Endpoint(nullptr, 0), InvalidAddressException);

        sockaddr_in addr = {};
        addr.sin_family = Util::getNativeAddressFamily(AddressFamily::Unknown);
        socklen_t size = sizeof(sockaddr_in);
        REQUIRE_THROWS_AS(Endpoint((sockaddr*)&addr, size), InvalidAddressFamilyException);

        addr.sin_family = AF_INET;
        size = sizeof(sockaddr_in6);
        REQUIRE_THROWS_AS(Endpoint((sockaddr*)&addr, size), InvalidAddressException);

        addr.sin_family = AF_INET6;
        size = sizeof(sockaddr_in);
        REQUIRE_THROWS_AS(Endpoint((sockaddr*)&addr, size), InvalidAddressException);
    }

    SECTION("Endpoint Creation with hostname and port") {
        REQUIRE_NOTHROW(Endpoint("localhost", 8080));
        REQUIRE_NOTHROW(Endpoint("localhost", 8080, AddressFamily::IPv4));
        REQUIRE_NOTHROW(Endpoint("localhost", 8080, AddressFamily::IPv6));
    }

    SECTION("Endpoint Creation with invalid hostname, port and AddressFamily") {
        CHECK_THROWS_AS(Endpoint("invalid_hostname", 8080), InvalidAddressException);
        CHECK_THROWS_AS(Endpoint("invalid_hostname", 8080, AddressFamily::IPv4), InvalidAddressException);
        CHECK_THROWS_AS(Endpoint("invalid_hostname", 8080, AddressFamily::IPv6), InvalidAddressException);
        CHECK_THROWS_AS(Endpoint("localhost", 8080, AddressFamily::Unknown), InvalidAddressFamilyException);
        // all int values for port are valid
    }

    SECTION("Endpoint Creation with invalid IP address") {
        CHECK_THROWS_AS(Endpoint(inet_addr("invalid_ip"), 8080), InvalidAddressException);
    }

    SECTION("Endpoint Creation with invalid AddressFamily") {
        REQUIRE_THROWS_AS(Endpoint(AddressFamily::Unknown, 8080), InvalidAddressFamilyException);
    }

    SECTION("Endpoint Creation with invalid sockaddr_storage") {
        sockaddr_storage addr = {};
        socklen_t size = sizeof(addr);
        CHECK_THROWS_AS(Endpoint(addr, size), InvalidAddressException);
    }

    SECTION("Endpoint Creation with ip and port") {
        REQUIRE_NOTHROW(Endpoint(in_addr_t(0x7F000001), 8080));
        REQUIRE_NOTHROW(Endpoint(inet_addr("127.0.0.1"), 8080));
    }

    SECTION("Endpoint Creation with invalid ip") {
        CHECK_THROWS_AS(Endpoint(in_addr_t(0), 8080), InvalidAddressException);
    }

    SECTION("Endpoint Creation with AddressFamily and port") {
        REQUIRE_NOTHROW(Endpoint(AddressFamily::IPv4, 8080));
        REQUIRE_NOTHROW(Endpoint(AddressFamily::IPv6, 8080));
    }

    SECTION("Endpoint Creation with invalid AddressFamily") {
        CHECK_THROWS_AS(Endpoint(AddressFamily::Unknown, 8080), InvalidAddressFamilyException);
    }

    SECTION("Get AddressFamily") {
        Endpoint endpoint("localhost", 8080);
        CHECK(endpoint.getAddressFamily() == AddressFamily::IPv4);
    }

    SECTION("Get Port") {
        Endpoint endpoint("localhost", 8080);
        CHECK(endpoint.getPort() == 8080);
    }

    SECTION("Get sockaddr") {
        sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8080);
        inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr));
        socklen_t size = sizeof(addr);
        Endpoint endpoint((sockaddr*)&addr, size);
        CHECK(memcmp(endpoint.c_addr(), &addr, size) == 0);

        sockaddr_in6 addr6 = {};
        addr6.sin6_family = AF_INET6;
        addr6.sin6_port = htons(8080);
        inet_pton(AF_INET6, "::1", &(addr6.sin6_addr));
        size = sizeof(addr6);
        Endpoint endpoint6((sockaddr*)&addr6, size);
        CHECK(memcmp(endpoint6.c_addr(), &addr6, size) == 0);
    }

    SECTION("Get size of sockaddr") {
        sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8080);
        inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr));
        socklen_t size = sizeof(addr);
        Endpoint endpoint((sockaddr*)&addr, size);
        CHECK(endpoint.c_size() == size);

        sockaddr_in6 addr6 = {};
        addr6.sin6_family = AF_INET6;
        addr6.sin6_port = htons(8080);
        inet_pton(AF_INET6, "::1", &(addr6.sin6_addr));
        size = sizeof(addr6);
        Endpoint endpoint6((sockaddr*)&addr6, size);
        CHECK(endpoint6.c_size() == size);
    }

    SECTION("Endpoint Destructor") {
        Endpoint* endpoint = new Endpoint("localhost", 8080);
        REQUIRE_NOTHROW(delete endpoint);
    }

    SECTION("Endpoint Validity") {
        Endpoint endpoint("localhost", 8080);
        CHECK(endpoint.getAddressFamily() == AddressFamily::IPv4);
        CHECK(endpoint.getPort() == 8080);

        sockaddr_in* addr = (sockaddr_in*)endpoint.c_addr();
        CHECK(addr->sin_addr.s_addr == htonl(INADDR_LOOPBACK));
        CHECK(addr->sin_port == htons(8080));
    }

}