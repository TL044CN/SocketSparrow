#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_all.hpp"
#include "Exceptions.hpp"

using namespace SocketSparrow;

TEST_CASE("Exception Classes", "[Exceptions]") {
    SECTION("SocketSparrowException") {
        REQUIRE_NOTHROW(SocketSparrowException());

        CHECK_THROWS_WITH([] { throw SocketSparrowException(); }(), "SocketSparrow Exception");

        CHECK_THROWS_AS([] { throw SocketSparrowException(); }(), SocketSparrowException);
    }

    SECTION("EndpointException") {
        REQUIRE_NOTHROW(EndpointException());
        REQUIRE_NOTHROW(EndpointException("Custom Message"));

        CHECK_THROWS_WITH([] { throw EndpointException(); }(), "Endpoint Exception");
        CHECK_THROWS_WITH([] { throw EndpointException("Custom Message"); }(), "Custom Message");

        CHECK_THROWS_AS([] { throw EndpointException(); }(), EndpointException);
        CHECK_THROWS_AS([] { throw EndpointException("Custom Message"); }(), EndpointException);
        CHECK_THROWS_AS([] { throw EndpointException(); }(), SocketSparrowException);
        CHECK_THROWS_AS([] { throw EndpointException("Custom Message"); }(), SocketSparrowException);
    }

    SECTION("InvalidAddressFamilyException") {
        REQUIRE_NOTHROW(InvalidAddressFamilyException());
        REQUIRE_NOTHROW(InvalidAddressFamilyException("Custom Message"));
        REQUIRE_NOTHROW(InvalidAddressFamilyException(AddressFamily::IPv4));
        REQUIRE_NOTHROW(InvalidAddressFamilyException(AddressFamily::IPv6, "Custom Message"));

        CHECK_THROWS_WITH([] { throw InvalidAddressFamilyException(); }(), "Invalid Address Family");
        CHECK_THROWS_WITH([] { throw InvalidAddressFamilyException("Custom Message"); }(), "Custom Message");
        CHECK_THROWS_WITH([] { throw InvalidAddressFamilyException(AddressFamily::IPv4); }(), "Invalid Address Family: AF_INET");
        CHECK_THROWS_WITH([] { throw InvalidAddressFamilyException(AddressFamily::IPv6, "Custom Message"); }(), "Custom Message: AF_INET6");

        CHECK_THROWS_AS([] { throw InvalidAddressFamilyException(); }(), InvalidAddressFamilyException);
        CHECK_THROWS_AS([] { throw InvalidAddressFamilyException("Custom Message"); }(), InvalidAddressFamilyException);
        CHECK_THROWS_AS([] { throw InvalidAddressFamilyException(AddressFamily::IPv4); }(), InvalidAddressFamilyException);
        CHECK_THROWS_AS([] { throw InvalidAddressFamilyException(AddressFamily::IPv6, "Custom Message"); }(), InvalidAddressFamilyException);
        CHECK_THROWS_AS([] { throw InvalidAddressFamilyException(); }(), EndpointException);
        CHECK_THROWS_AS([] { throw InvalidAddressFamilyException("Custom Message"); }(), EndpointException);
        CHECK_THROWS_AS([] { throw InvalidAddressFamilyException(AddressFamily::IPv4); }(), EndpointException);
        CHECK_THROWS_AS([] { throw InvalidAddressFamilyException(AddressFamily::IPv6, "Custom Message"); }(), EndpointException);
        CHECK_THROWS_AS([] { throw InvalidAddressFamilyException(); }(), SocketSparrowException);
        CHECK_THROWS_AS([] { throw InvalidAddressFamilyException("Custom Message"); }(), SocketSparrowException);
        CHECK_THROWS_AS([] { throw InvalidAddressFamilyException(AddressFamily::IPv4); }(), SocketSparrowException);
        CHECK_THROWS_AS([] { throw InvalidAddressFamilyException(AddressFamily::IPv6, "Custom Message"); }(), SocketSparrowException);
    }

    SECTION("InvalidPort") {
        REQUIRE_NOTHROW(InvalidPortException());
        REQUIRE_NOTHROW(InvalidPortException("Custom Message"));
        REQUIRE_NOTHROW(InvalidPortException(8080));
        REQUIRE_NOTHROW(InvalidPortException(8080, "Custom Message"));

        CHECK_THROWS_WITH([] { throw InvalidPortException(); }(), "Invalid Port");
        CHECK_THROWS_WITH([] { throw InvalidPortException("Custom Message"); }(), "Custom Message");
        CHECK_THROWS_WITH([] { throw InvalidPortException(8080); }(), "Invalid Port: 8080");
        CHECK_THROWS_WITH([] { throw InvalidPortException(8080, "Custom Message"); }(), "Custom Message: 8080");

        CHECK_THROWS_AS([] { throw InvalidPortException(); }(), InvalidPortException);
        CHECK_THROWS_AS([] { throw InvalidPortException("Custom Message"); }(), InvalidPortException);
        CHECK_THROWS_AS([] { throw InvalidPortException(8080); }(), InvalidPortException);
        CHECK_THROWS_AS([] { throw InvalidPortException(8080, "Custom Message"); }(), InvalidPortException);
        CHECK_THROWS_AS([] { throw InvalidPortException(); }(), EndpointException);
        CHECK_THROWS_AS([] { throw InvalidPortException("Custom Message"); }(), EndpointException);
        CHECK_THROWS_AS([] { throw InvalidPortException(8080); }(), EndpointException);
        CHECK_THROWS_AS([] { throw InvalidPortException(8080, "Custom Message"); }(), EndpointException);
        CHECK_THROWS_AS([] { throw InvalidPortException(); }(), SocketSparrowException);
        CHECK_THROWS_AS([] { throw InvalidPortException("Custom Message"); }(), SocketSparrowException);
        CHECK_THROWS_AS([] { throw InvalidPortException(8080); }(), SocketSparrowException);
        CHECK_THROWS_AS([] { throw InvalidPortException(8080, "Custom Message"); }(), SocketSparrowException);
    }

    SECTION("InvalidAddressException") {
        REQUIRE_NOTHROW(InvalidAddressException());
        REQUIRE_NOTHROW(InvalidAddressException("Custom Message"));
        REQUIRE_NOTHROW(InvalidAddressException("0.0.0.0.0", "Custom Message"));

        CHECK_THROWS_WITH([] { throw InvalidAddressException(); }(), "Invalid Address");
        CHECK_THROWS_WITH([] { throw InvalidAddressException("Custom Message"); }(), "Custom Message");
        CHECK_THROWS_WITH([] { throw InvalidAddressException("0.0.0.0.0", "Custom Message"); }(), "Custom Message: 0.0.0.0.0");

        CHECK_THROWS_AS([] { throw InvalidAddressException(); }(), InvalidAddressException);
        CHECK_THROWS_AS([] { throw InvalidAddressException("Custom Message"); }(), InvalidAddressException);
        CHECK_THROWS_AS([] { throw InvalidAddressException("0.0.0.0.0", "Custom Message"); }(), InvalidAddressException);
        CHECK_THROWS_AS([] { throw InvalidAddressException(); }(), EndpointException);
        CHECK_THROWS_AS([] { throw InvalidAddressException("Custom Message"); }(), EndpointException);
        CHECK_THROWS_AS([] { throw InvalidAddressException("0.0.0.0.0", "Custom Message"); }(), EndpointException);
        CHECK_THROWS_AS([] { throw InvalidAddressException(); }(), SocketSparrowException);
        CHECK_THROWS_AS([] { throw InvalidAddressException("Custom Message"); }(), SocketSparrowException);
        CHECK_THROWS_AS([] { throw InvalidAddressException("0.0.0.0.0", "Custom Message"); }(), SocketSparrowException);
    }


    SECTION("SocketException") {
        REQUIRE_NOTHROW(SocketException());

        CHECK_THROWS_WITH([] { throw SocketException(); }(), "Socket Exception");
        CHECK_THROWS_WITH([] { throw SocketException("Custom Message"); }(), "Custom Message");
        CHECK_THROWS_WITH([] { throw SocketException(1); }(), "Socket Exception: [1] Operation not permitted");
        CHECK_THROWS_WITH([] { throw SocketException(1, "Custom Message"); }(), "Custom Message: [1] Operation not permitted");

        CHECK_THROWS_AS([] { throw SocketException(); }(), SocketException);
        CHECK_THROWS_AS([] { throw SocketException(1); }(), SocketException);
        CHECK_THROWS_AS([] { throw SocketException("Custom Message"); }(), SocketException);
        CHECK_THROWS_AS([] { throw SocketException(1, "Custom Message"); }(), SocketException);
        CHECK_THROWS_AS([] { throw SocketException(); }(), SocketSparrowException);
        CHECK_THROWS_AS([] { throw SocketException(1); }(), SocketSparrowException);
        CHECK_THROWS_AS([] { throw SocketException("Custom Message"); }(), SocketSparrowException);
        CHECK_THROWS_AS([] { throw SocketException(1, "Custom Message"); }(), SocketSparrowException);
    }

    SECTION("SendError") {
        REQUIRE_NOTHROW(SendError());

        CHECK_THROWS_WITH([] { throw SendError(); }(), "Send Error");
        CHECK_THROWS_WITH([] { throw SendError("Custom Message"); }(), "Custom Message");
        CHECK_THROWS_WITH([] { throw SendError(1); }(), "Send Error: [1] Operation not permitted");
        CHECK_THROWS_WITH([] { throw SendError(1, "Custom Message"); }(), "Custom Message: [1] Operation not permitted");

        CHECK_THROWS_AS([] { throw SendError(); }(), SendError);
        CHECK_THROWS_AS([] { throw SendError(1); }(), SendError);
        CHECK_THROWS_AS([] { throw SendError("Custom Message"); }(), SendError);
        CHECK_THROWS_AS([] { throw SendError(1, "Custom Message"); }(), SendError);
        CHECK_THROWS_AS([] { throw SendError(); }(), SocketException);
        CHECK_THROWS_AS([] { throw SendError(1); }(), SocketException);
        CHECK_THROWS_AS([] { throw SendError("Custom Message"); }(), SocketException);
        CHECK_THROWS_AS([] { throw SendError(1, "Custom Message"); }(), SocketException);
        CHECK_THROWS_AS([] { throw SendError(); }(), SocketSparrowException);
        CHECK_THROWS_AS([] { throw SendError(1); }(), SocketSparrowException);
        CHECK_THROWS_AS([] { throw SendError("Custom Message"); }(), SocketSparrowException);
        CHECK_THROWS_AS([] { throw SendError(1, "Custom Message"); }(), SocketSparrowException);

    }

    SECTION("RecvError") {
        REQUIRE_NOTHROW(RecvError());

        CHECK_THROWS_WITH([] { throw RecvError(); }(), "Receive Error");
        CHECK_THROWS_WITH([] { throw RecvError("Custom Message"); }(), "Custom Message");
        CHECK_THROWS_WITH([] { throw RecvError(1); }(), "Receive Error: [1] Operation not permitted");
        CHECK_THROWS_WITH([] { throw RecvError(1, "Custom Message"); }(), "Custom Message: [1] Operation not permitted");

        CHECK_THROWS_AS([] { throw RecvError(); }(), RecvError);
        CHECK_THROWS_AS([] { throw RecvError(1); }(), RecvError);
        CHECK_THROWS_AS([] { throw RecvError("Custom Message"); }(), RecvError);
        CHECK_THROWS_AS([] { throw RecvError(1, "Custom Message"); }(), RecvError);
        CHECK_THROWS_AS([] { throw RecvError(); }(), SocketException);
        CHECK_THROWS_AS([] { throw RecvError(1); }(), SocketException);
        CHECK_THROWS_AS([] { throw RecvError("Custom Message"); }(), SocketException);
        CHECK_THROWS_AS([] { throw RecvError(1, "Custom Message"); }(), SocketException);
        CHECK_THROWS_AS([] { throw RecvError(); }(), SocketSparrowException);
        CHECK_THROWS_AS([] { throw RecvError(1); }(), SocketSparrowException);
        CHECK_THROWS_AS([] { throw RecvError("Custom Message"); }(), SocketSparrowException);
        CHECK_THROWS_AS([] { throw RecvError(1, "Custom Message"); }(), SocketSparrowException);

    }
}
