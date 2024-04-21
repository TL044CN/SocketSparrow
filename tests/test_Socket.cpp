#include "catch2/catch.hpp"
#include "Socket.hpp"

using namespace SocketSparrow;

TEST_CASE("Socket Creation", "[Socket]") {
    SECTION("Socket Creation with AddressFamily and Protocol") {
        REQUIRE_NOTHROW(Socket(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP));
    }
    SECTION("Socket Creation with AddressFamily and Endpoint") {
        auto endpoint = std::make_shared<Endpoint>("localhost", 8080);
        REQUIRE_NOTHROW(Socket(SocketSparrow::AddressFamily::IPv4, endpoint));
    }
}

TEST_CASE("Socket Bind", "[Socket]") {
    SECTION("Bind to Endpoint") {
        auto endpoint = std::make_shared<Endpoint>("localhost", 8080);
        Socket socket(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
        REQUIRE_NOTHROW(socket.bind(endpoint));
    }
    SECTION("Bind to Port") {
        Socket socket(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
        REQUIRE_NOTHROW(socket.bindToPort(8080));
    }
}
