#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_all.hpp"
#include "catch2/generators/catch_generators_all.hpp"

#define private public
#define protected public
#include "TLSSocket.hpp"
#undef protected
#undef private

#include "SocketMocking.hpp"
#include "Exceptions.hpp"


using namespace SocketSparrow;

SCENARIO("TLSSocket::TLSSocket(AddressFamily, SocketType)") {
    GIVEN("A valid AddressFamily and SocketType") {
        AddressFamily af = AddressFamily::IPv4;
        SocketType protocol = SocketType::TCP;

        WHEN("A TLSSocket object is created") {
            TLSSocket* tlsSocket = new TLSSocket(af, protocol);

            THEN("The object is created") {
                REQUIRE(tlsSocket != nullptr);
                REQUIRE(tlsSocket->mOpenSSL_CTX != nullptr);
                REQUIRE(tlsSocket->mOpenSSL_SSL != nullptr);
            }

            delete tlsSocket;
        }
    }
}
