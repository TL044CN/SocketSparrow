#include "catch2/catch_test_macros.hpp"
#include "Socket.hpp"

#include <future>
#include <thread>
#include <chrono>

using namespace SocketSparrow;

TEST_CASE("Socket Creation", "[Socket]") {
    SECTION("Socket Creation with AddressFamily and Protocol") {
        REQUIRE_NOTHROW(Socket(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP));
    }
    SECTION("Socket Creation with AddressFamily and Endpoint") {
        auto endpoint = std::make_shared<Endpoint>("localhost", 7743);
        REQUIRE_NOTHROW(Socket(SocketSparrow::AddressFamily::IPv4, endpoint));
    }
}

TEST_CASE("Socket Bind", "[Socket]") {
    SECTION("Bind to Endpoint") {
        auto endpoint = std::make_shared<Endpoint>("localhost", 7744);
        Socket socket(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
        REQUIRE_NOTHROW(socket.bind(endpoint));
    }
    SECTION("Bind to Port") {
        Socket socket(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
        REQUIRE_NOTHROW(socket.bindToPort(8080));
    }
}

TEST_CASE("Socket Connect", "[Socket]") {
    auto endpoint = std::make_shared<Endpoint>("localhost", 7745);
    Socket server(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
    REQUIRE_NOTHROW(server.bind(endpoint));
    std::promise<void> serverReady;
    std::future<void> serverReadyFuture = serverReady.get_future();

    SECTION("Listen on Endpoint and accept connection") {
        auto f = std::async(std::launch::async, [&](){
            server.listen(5);
            serverReady.set_value();
            auto connection = server.accept();
            REQUIRE(connection);
        });

        serverReadyFuture.wait();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        Socket client(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
        REQUIRE_NOTHROW(client.connect(endpoint));

        f.wait();
    }
}