#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_all.hpp"
#include "catch2/generators/catch_generators_all.hpp"

#define private public
#include "Socket.hpp"
#undef private

#include "Exceptions.hpp"

#include <future>
#include <thread>
#include <chrono>
#include <atomic>

#include <dlfcn.h>
#include <errno.h>

// === System Call Mocking ===
typedef int(*socket_func_t)(int, int, int);
typedef int(*listen_func_t)(int, int);
typedef int(*accept_func_t)(int, struct sockaddr*, socklen_t*);
typedef int(*setsockopt_func_t)(int, int, int, const void*, socklen_t);
typedef int(*fnctl_func_t)(int, int, ...);

socket_func_t real_socket = (socket_func_t)dlsym(RTLD_NEXT, "socket");
socket_func_t mock_socket = [](int domain, int type, int protocol) -> int {
    errno = EPROTONOSUPPORT;
    return -1;
    };

listen_func_t real_listen = (listen_func_t)dlsym(RTLD_NEXT, "listen");
listen_func_t mock_listen = [](int sockfd, int backlog) -> int {
    errno = EOPNOTSUPP;
    return -1;
    };

accept_func_t real_accept = (accept_func_t)dlsym(RTLD_NEXT, "accept");
accept_func_t mock_accept = [](int sockfd, struct sockaddr* addr, socklen_t* addrlen) -> int {
    errno = EOPNOTSUPP;
    return -1;
    };

setsockopt_func_t real_setsockopt = (setsockopt_func_t)dlsym(RTLD_NEXT, "setsockopt");
setsockopt_func_t mock_setsockopt = [](int sockfd, int level, int optname, const void* optval, socklen_t optlen) -> int {
    errno = EOPNOTSUPP;
    return -1;
    };


bool useMockSocket = false;
bool useMockListen = false;
bool useMockAccept = false;
bool useMockSetsockopt = false;


int socket(int domain, int type, int protocol) {
    if ( useMockSocket ) {
        return mock_socket(domain, type, protocol);
    }
    return real_socket(domain, type, protocol);
}

int listen(int sockfd, int backlog) {
    if ( useMockListen ) {
        return mock_listen(sockfd, backlog);
    }
    return real_listen(sockfd, backlog);
}

int accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen) {
    if ( useMockAccept ) {
        return mock_accept(sockfd, addr, addrlen);
    }
    return real_accept(sockfd, addr, addrlen);
}

int setsockopt(int sockfd, int level, int optname, const void* optval, socklen_t optlen) {
    if ( useMockSetsockopt ) {
        return mock_setsockopt(sockfd, level, optname, optval, optlen);
    }
    return real_setsockopt(sockfd, level, optname, optval, optlen);
}

// === End System Call Mocking ===

using namespace SocketSparrow;

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

        useMockSocket = true;
        CHECK_THROWS_MATCHES(
            Socket(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP),
            SocketException,
            Catch::Matchers::Message("Failed to create Socket: [93] Protocol not supported")
        );
        useMockSocket = false;
    }

    SECTION("Socket Creation with AddressFamily and Endpoint") {
        auto endpoint = std::make_shared<Endpoint>("localhost", 7743);
        REQUIRE_NOTHROW(Socket(SocketSparrow::AddressFamily::IPv4, endpoint));

        // deliberately make the socket call fail
        useMockSocket = true;
        CHECK_THROWS_MATCHES(
            Socket(SocketSparrow::AddressFamily::IPv4, endpoint),
            SocketException,
            Catch::Matchers::Message("Failed to create Socket: [93] Protocol not supported")
        );
        useMockSocket = false;
    }
}

TEST_CASE("Socket Bind", "[Socket]") {
    SECTION("Bind to Endpoint") {
        auto endpoint = std::make_shared<Endpoint>("localhost", 7744);
        Socket socket(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::TCP);
        REQUIRE_NOTHROW(socket.bind(endpoint));

        auto endpoint2 = std::make_shared<Endpoint>("localhost", 7745);
        Socket socket2(SocketSparrow::AddressFamily::IPv4, SocketSparrow::SocketType::UDP);
        CHECK_THROWS_MATCHES(
            socket2.bind(endpoint2),
            SocketException,
            Catch::Matchers::Message("Cannot bind a UDP socket to an endpoint")
        );
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

    useMockSetsockopt = true;
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
    useMockSetsockopt = false;

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
        useMockListen = true;
        CHECK_THROWS_MATCHES(
            socket4.listen(5),
            SocketException,
            Catch::Matchers::Message("Failed to listen: [95] Operation not supported")
        );
        useMockListen = false;

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
        useMockAccept = true;
        CHECK_THROWS_MATCHES(
            socket3.accept(),
            SocketException,
            Catch::Matchers::Message("Failed to accept: [95] Operation not supported")
        );
        useMockAccept = false;
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
}