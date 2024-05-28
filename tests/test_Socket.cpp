#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_all.hpp"
#include "catch2/generators/catch_generators_all.hpp"


#define private public
#define protected public
#include "Socket.hpp"
#undef protected
#undef private

#include "Exceptions.hpp"
#include "SocketMocking.hpp"

#include <future>
#include <thread>
#include <chrono>
#include <atomic>

using namespace SocketSparrow;
using MockingBird::MockingController;

TEST_CASE("Socket Creation", "[Socket]") {
    SECTION("Socket Creation with invalid file descriptor") {
        CHECK_THROWS_MATCHES(
            Socket(-1, std::make_shared<Endpoint>(), SocketType::TCP),
            SocketException,
            Catch::Matchers::Message("Failed to create Socket")
        );
    }

    SECTION("Socket Creation with AddressFamily and Protocol") {
        REQUIRE_NOTHROW(Socket(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP));

        auto mockGuard = MockingController::createMockGuard("socket");
        CHECK_THROWS_MATCHES(
            Socket(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP),
            SocketException,
            Catch::Matchers::Message("Failed to create Socket: [95] Operation not supported")
        );
    }

    SECTION("Socket Creation with AddressFamily and Endpoint") {
        auto endpoint = std::make_shared<Endpoint>("localhost", 7743);
        REQUIRE_NOTHROW(Socket(SocketSparrow::AddressFamily::IPv4, endpoint));

        // deliberately make the socket call fail
        auto mockGuard = MockingController::createMockGuard("socket");
        CHECK_THROWS_MATCHES(
            Socket(SocketSparrow::AddressFamily::IPv4, endpoint),
            SocketException,
            Catch::Matchers::Message("Failed to create Socket: [95] Operation not supported")
        );
    }
}

TEST_CASE("Socket Getters", "[Socket]") {
    SECTION("Get File Descriptor") {
        Socket socket(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
        CHECK(socket.getNativeSocket() > 0);
    }

    SECTION("Get Endpoint") {
        auto endpoint = std::make_shared<Endpoint>("localhost", 7743);
        Socket socket(SocketSparrow::AddressFamily::IPv4, endpoint);
        CHECK(socket.getEndpoint() == endpoint);
    }

    SECTION("Get Protocol") {
        Socket socket(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
        CHECK(socket.getProtocol() == SocketSparrow::SocketType::TCP);
    }

    SECTION("Get Address Family") {
        Socket socket(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
        CHECK(socket.getAddressFamily() == SocketSparrow::AddressFamily::IPv4);
    }

    SECTION("Get State") {
        Socket socket(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
        CHECK(socket.getState() == SocketState::Open);
    }

}

TEST_CASE("Socket Setters", "[Socket]") {
    SECTION("Set State") {
        Socket socket(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
        socket.setState(SocketState::Closed);
        CHECK(socket.getState() == SocketState::Closed);
    }
}

TEST_CASE("Socket Bind", "[Socket]") {
    SECTION("Bind to Endpoint") {
        auto endpoint = std::make_shared<Endpoint>("localhost", 7744);
        Socket socket(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
        REQUIRE_NOTHROW(socket.bind(endpoint));

        auto endpoint2 = std::make_shared<Endpoint>("localhost", 7745);
        Socket socket2(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::UDP);
        REQUIRE_NOTHROW(socket2.bind(endpoint2));
    }

    SECTION("Bind to Port") {
        Socket socket(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
        REQUIRE_NOTHROW(socket.bindToPort(8080));
        Socket socket2(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
        REQUIRE_THROWS_MATCHES(
            socket2.bindToPort(8080),
            SocketException,
            Catch::Matchers::Message("Failed to bind to endpoint: [98] Address already in use")
        );

    }
}

TEST_CASE("Socket Options", "[Socket]") {
    Socket socket(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);

    CHECK_NOTHROW(socket.enableAddressReuse(true));
    CHECK_NOTHROW(socket.enableAddressReuse(false));
    CHECK_NOTHROW(socket.enablePortReuse(true));
    CHECK_NOTHROW(socket.enablePortReuse(false));
    CHECK_NOTHROW(socket.enableBroadcast(true));
    CHECK_NOTHROW(socket.enableBroadcast(false));
    CHECK_NOTHROW(socket.enableNonBlocking(true));
    CHECK_NOTHROW(socket.enableNonBlocking(false));

    auto mockGuard = MockingController::createMockGuard("setsockopt");
    CHECK_THROWS_MATCHES(
        socket.enableAddressReuse(true),
        SocketException,
        Catch::Matchers::Message("Failed to set socket option: [95] Operation not supported")
    );
    CHECK_THROWS_MATCHES(
        socket.enablePortReuse(true),
        SocketException,
        Catch::Matchers::Message("Failed to set socket option: [95] Operation not supported")
    );
    CHECK_THROWS_MATCHES(
        socket.enableBroadcast(true),
        SocketException,
        Catch::Matchers::Message("Failed to set socket option: [95] Operation not supported")
    );
    CHECK_THROWS_MATCHES(
        socket.enableAddressReuse(false),
        SocketException,
        Catch::Matchers::Message("Failed to set socket option: [95] Operation not supported")
    );
    CHECK_THROWS_MATCHES(
        socket.enablePortReuse(false),
        SocketException,
        Catch::Matchers::Message("Failed to set socket option: [95] Operation not supported")
    );
    CHECK_THROWS_MATCHES(
        socket.enableBroadcast(false),
        SocketException,
        Catch::Matchers::Message("Failed to set socket option: [95] Operation not supported")
    );

}

TEST_CASE("Socket Connect", "[Socket]") {
    auto endpoint = std::make_shared<Endpoint>("localhost", 7746);
    Socket server(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
    REQUIRE_NOTHROW(server.enableAddressReuse(true));
    REQUIRE_NOTHROW(server.bind(endpoint));
    std::promise<void> serverReady;
    std::future<void> serverReadyFuture = serverReady.get_future();
    SECTION("Listen on Endpoint and accept connection") {
        auto f = std::async(std::launch::async, [&]() {
            server.listen(5);
            CHECK(server.getState() == SocketState::Listening);
            serverReady.set_value();
            auto connection = server.accept();
            REQUIRE(connection);
        });

        serverReadyFuture.wait();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        Socket client(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
        REQUIRE_NOTHROW(client.connect(endpoint));
        CHECK(client.getState() == SocketState::Connected);
        f.wait();
    }
    SECTION("Check Connection Failures", "[Socket]") {
        Socket client(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::UDP);
        CHECK_THROWS_MATCHES(
            client.connect(endpoint),
            SocketException,
            Catch::Matchers::Message("Cannot connect a UDP socket")
        );

        auto endpoint2 = std::make_shared<Endpoint>("localhost", 7747);
        Socket client2(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
        CHECK_THROWS_MATCHES(
            client2.connect(endpoint2),
            SocketException,
            Catch::Matchers::Message("Failed to connect: [111] Connection refused")
        );
    }
}
TEST_CASE("Socket Listen and Accept", "[Socket]") {
    Socket socket2(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::UDP);
    Socket socket3(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
    Socket socket4(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
    auto endpoint = std::make_shared<Endpoint>("localhost", 7748);
    SECTION("Check Listen", "[Socket]") {
        CHECK_THROWS_MATCHES(
            socket2.listen(5),
            SocketException,
            Catch::Matchers::Message("Cannot listen on a UDP socket")
        );

        socket4.bind(endpoint);
        {
            auto mockGuard = MockingController::createMockGuard("listen");
            CHECK_THROWS_MATCHES(
                socket4.listen(5),
                SocketException,
                Catch::Matchers::Message("Failed to listen: [95] Operation not supported")
            );
        }

        REQUIRE_NOTHROW(socket4.listen(5));
    }

    SECTION("Check Accept", "[Socket]") {
        CHECK_THROWS_MATCHES(
            socket2.accept(),
            SocketException,
            Catch::Matchers::Message("Cannot accept on a UDP socket")
        );

        CHECK_THROWS_MATCHES(
            socket3.accept(),
            SocketException,
            Catch::Matchers::Message("Cannot accept without binding to an endpoint")
        );

        CHECK_THROWS_MATCHES(
            socket3.accept(),
            SocketException,
            Catch::Matchers::Message("Cannot accept without binding to an endpoint")
        );

        socket3.bind(endpoint);
        CHECK_THROWS_MATCHES(
            socket3.accept(),
            SocketException,
            Catch::Matchers::Message("Cannot accept without listening")
        );
    }

    SECTION("Check Accept Failures", "[Socket]") {
        socket3.bind(endpoint);
        socket3.listen(5);
        auto mockGuard = MockingController::createMockGuard("accept");
        CHECK_THROWS_MATCHES(
            socket3.accept(),
            SocketException,
            Catch::Matchers::Message("Failed to accept: [95] Operation not supported")
        );
    }
}
TEST_CASE("Socket Send and Recv", "[Socket]") {
    SECTION("Check Send and Recv", "[Socket]") {
        auto endpoint2 = std::make_shared<Endpoint>("localhost", 7749);
        Socket server(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
        server.enableAddressReuse(true);
        REQUIRE_NOTHROW(server.bind(endpoint2));
        REQUIRE_NOTHROW(server.listen(5));

        std::atomic_flag flag = ATOMIC_FLAG_INIT;

        auto serverFuture = std::async(std::launch::async, [&]() {
            auto connection = server.accept();
            REQUIRE(connection);
            flag.test_and_set();
            flag.notify_all();

            connection->enableNonBlocking(false);

            std::string recvMessage;
            REQUIRE_NOTHROW(connection->recv(recvMessage));
            CHECK(recvMessage == "Hello World!");

            std::vector<char> sendMessageVec(recvMessage.begin(), recvMessage.end());
            REQUIRE_NOTHROW(connection->send(std::string(sendMessageVec.begin(), sendMessageVec.end())));

            std::vector<char> recvMessageVec;
            REQUIRE_NOTHROW(connection->recv(recvMessageVec, 5));
            bool equal = std::string(recvMessageVec.begin(), recvMessageVec.end()) == "Hello";
            CHECK(equal);

            std::string largeMessage(1028, 'A');

            REQUIRE_NOTHROW((*connection) << largeMessage);
            std::stringstream ss;
            REQUIRE_NOTHROW((*connection) >> ss);
            REQUIRE_NOTHROW((*connection) << ss.str());
        });

        auto clientFuture = std::async(std::launch::async, [&]() {

            Socket client(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
            REQUIRE_NOTHROW(client.connect(endpoint2));
            flag.wait(true);
            REQUIRE_NOTHROW(client.send("Hello World!"));
            std::vector<char> recvMessageVec;
            REQUIRE_NOTHROW(client.recv(recvMessageVec));
            REQUIRE_NOTHROW(client.send(recvMessageVec));
            REQUIRE_NOTHROW(client >> recvMessageVec);
            REQUIRE_NOTHROW(client << recvMessageVec);
            std::string recvMessage;
            REQUIRE_NOTHROW(client >> recvMessage);
        });

        serverFuture.wait();
        clientFuture.wait();
    }

    SECTION("send_to and revc_from", "[Socket]") {
        auto endpoint2 = std::make_shared<Endpoint>("localhost", 7750);
        Socket server(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::UDP);
        Socket client(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::UDP);

        server.enableAddressReuse(true);
        server.enablePortReuse(true);
        client.enableAddressReuse(true);
        client.enablePortReuse(true);

        REQUIRE_NOTHROW(server.bind(endpoint2));

        std::atomic_flag flag = ATOMIC_FLAG_INIT;

        auto serverFuture = std::async(std::launch::async, [&]() {
            while (flag.test_and_set(std::memory_order_acquire)) {
                std::this_thread::yield();
            }

            UDPPacket packet;
            REQUIRE_NOTHROW(packet = server.recv_from());

            std::string message(packet.data.begin(), packet.data.end());
            REQUIRE( message == "Hello World!");
            REQUIRE_NOTHROW(packet = server.recv_from());

        });

        auto clientFuture = std::async(std::launch::async, [&]() {

            REQUIRE_NOTHROW(client.send_to("Hello World!", endpoint2));
            UDPPacket packet(
                std::vector<char>{ 'H', 'E', 'L', 'L', 'O' },
                endpoint2
            );

            client.send_to(packet);
            flag.clear(std::memory_order_release);
        });

        serverFuture.wait();
        clientFuture.wait();
    }

}