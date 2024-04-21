#include "catch2/catch.hpp"
#include "Exceptions.hpp"

using namespace SocketSparrow;

TEST_CASE("Exception Classes", "[Exceptions]") {
    SECTION("SocketSparrowException") {
        REQUIRE_NOTHROW(SocketSparrowException());

        REQUIRE_THROWS_WITH([] { throw SocketSparrowException(); }(), "SocketSparrow Exception");

        REQUIRE_THROWS_AS([] { throw SocketSparrowException(); }(), SocketSparrowException);
    }

    SECTION("EndpointException") {
        REQUIRE_NOTHROW(EndpointException());
        REQUIRE_NOTHROW(EndpointException("Custom Message"));

        REQUIRE_THROWS_WITH([] { throw EndpointException(); }(), "Endpoint Exception");
        REQUIRE_THROWS_WITH([] { throw EndpointException("Custom Message"); }(), "Custom Message");

        REQUIRE_THROWS_AS([] { throw EndpointException(); }(), EndpointException);
        REQUIRE_THROWS_AS([] { throw EndpointException("Custom Message"); }(), EndpointException);
        REQUIRE_THROWS_AS([] { throw EndpointException(); }(), SocketSparrowException);
        REQUIRE_THROWS_AS([] { throw EndpointException("Custom Message"); }(), SocketSparrowException);
    }

    SECTION("InvalidAddressFamilyException") {
        REQUIRE_NOTHROW(InvalidAddressFamilyException());
        REQUIRE_NOTHROW(InvalidAddressFamilyException("Custom Message"));
        REQUIRE_NOTHROW(InvalidAddressFamilyException(AddressFamily::IPv4));
        REQUIRE_NOTHROW(InvalidAddressFamilyException(AddressFamily::IPv6, "Custom Message"));

        REQUIRE_THROWS_WITH([] { throw InvalidAddressFamilyException(); }(), "Invalid Address Family");
        REQUIRE_THROWS_WITH([] { throw InvalidAddressFamilyException("Custom Message"); }(), "Custom Message");
        REQUIRE_THROWS_WITH([] { throw InvalidAddressFamilyException(AddressFamily::IPv4); }(), "Invalid Address Family: AF_INET");
        REQUIRE_THROWS_WITH([] { throw InvalidAddressFamilyException(AddressFamily::IPv6, "Custom Message"); }(), "Custom Message: AF_INET6");

        REQUIRE_THROWS_AS([] { throw InvalidAddressFamilyException(); }(), InvalidAddressFamilyException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressFamilyException("Custom Message"); }(), InvalidAddressFamilyException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressFamilyException(AddressFamily::IPv4); }(), InvalidAddressFamilyException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressFamilyException(AddressFamily::IPv6, "Custom Message"); }(), InvalidAddressFamilyException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressFamilyException(); }(), EndpointException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressFamilyException("Custom Message"); }(), EndpointException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressFamilyException(AddressFamily::IPv4); }(), EndpointException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressFamilyException(AddressFamily::IPv6, "Custom Message"); }(), EndpointException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressFamilyException(); }(), SocketSparrowException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressFamilyException("Custom Message"); }(), SocketSparrowException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressFamilyException(AddressFamily::IPv4); }(), SocketSparrowException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressFamilyException(AddressFamily::IPv6, "Custom Message"); }(), SocketSparrowException);
    }

    SECTION("InvalidPort") {
        REQUIRE_NOTHROW(InvalidPortException());
        REQUIRE_NOTHROW(InvalidPortException("Custom Message"));
        REQUIRE_NOTHROW(InvalidPortException(8080));
        REQUIRE_NOTHROW(InvalidPortException(8080, "Custom Message"));

        REQUIRE_THROWS_WITH([] { throw InvalidPortException(); }(), "Invalid Port");
        REQUIRE_THROWS_WITH([] { throw InvalidPortException("Custom Message"); }(), "Custom Message");
        REQUIRE_THROWS_WITH([] { throw InvalidPortException(8080); }(), "Invalid Port: 8080");
        REQUIRE_THROWS_WITH([] { throw InvalidPortException(8080, "Custom Message"); }(), "Custom Message: 8080");

        REQUIRE_THROWS_AS([] { throw InvalidPortException(); }(), InvalidPortException);
        REQUIRE_THROWS_AS([] { throw InvalidPortException("Custom Message"); }(), InvalidPortException);
        REQUIRE_THROWS_AS([] { throw InvalidPortException(8080); }(), InvalidPortException);
        REQUIRE_THROWS_AS([] { throw InvalidPortException(8080, "Custom Message"); }(), InvalidPortException);
        REQUIRE_THROWS_AS([] { throw InvalidPortException(); }(), EndpointException);
        REQUIRE_THROWS_AS([] { throw InvalidPortException("Custom Message"); }(), EndpointException);
        REQUIRE_THROWS_AS([] { throw InvalidPortException(8080); }(), EndpointException);
        REQUIRE_THROWS_AS([] { throw InvalidPortException(8080, "Custom Message"); }(), EndpointException);
        REQUIRE_THROWS_AS([] { throw InvalidPortException(); }(), SocketSparrowException);
        REQUIRE_THROWS_AS([] { throw InvalidPortException("Custom Message"); }(), SocketSparrowException);
        REQUIRE_THROWS_AS([] { throw InvalidPortException(8080); }(), SocketSparrowException);
        REQUIRE_THROWS_AS([] { throw InvalidPortException(8080, "Custom Message"); }(), SocketSparrowException);
    }

    SECTION("InvalidAddressException") {
        REQUIRE_NOTHROW(InvalidAddressException());
        REQUIRE_NOTHROW(InvalidAddressException("Custom Message"));
        REQUIRE_NOTHROW(InvalidAddressException("0.0.0.0.0", "Custom Message"));

        REQUIRE_THROWS_WITH([] { throw InvalidAddressException(); }(), "Invalid Address");
        REQUIRE_THROWS_WITH([] { throw InvalidAddressException("Custom Message"); }(), "Custom Message");
        REQUIRE_THROWS_WITH([] { throw InvalidAddressException("0.0.0.0.0", "Custom Message"); }(), "Custom Message: 0.0.0.0.0");

        REQUIRE_THROWS_AS([] { throw InvalidAddressException(); }(), InvalidAddressException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressException("Custom Message"); }(), InvalidAddressException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressException("0.0.0.0.0", "Custom Message"); }(), InvalidAddressException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressException(); }(), EndpointException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressException("Custom Message"); }(), EndpointException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressException("0.0.0.0.0", "Custom Message"); }(), EndpointException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressException(); }(), SocketSparrowException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressException("Custom Message"); }(), SocketSparrowException);
        REQUIRE_THROWS_AS([] { throw InvalidAddressException("0.0.0.0.0", "Custom Message"); }(), SocketSparrowException);
    }


    SECTION("SocketException") {
        REQUIRE_NOTHROW(SocketException());

        REQUIRE_THROWS_WITH([] { throw SocketException(); }(), "Socket Exception");
        REQUIRE_THROWS_WITH([] { throw SocketException("Custom Message"); }(), "Custom Message");
        REQUIRE_THROWS_WITH([] { throw SocketException(1); }(), "Socket Exception: [1] Operation not permitted");
        REQUIRE_THROWS_WITH([] { throw SocketException(1, "Custom Message"); }(), "Custom Message: [1] Operation not permitted");


        REQUIRE_THROWS_AS([] { throw SocketException(); }(), SocketException);
        REQUIRE_THROWS_AS([] { throw SocketException(1); }(), SocketException);
        REQUIRE_THROWS_AS([] { throw SocketException("Custom Message"); }(), SocketException);
        REQUIRE_THROWS_AS([] { throw SocketException(1, "Custom Message"); }(), SocketException);
        REQUIRE_THROWS_AS([] { throw SocketException(); }(), SocketSparrowException);
        REQUIRE_THROWS_AS([] { throw SocketException(1); }(), SocketSparrowException);
        REQUIRE_THROWS_AS([] { throw SocketException("Custom Message"); }(), SocketSparrowException);
        REQUIRE_THROWS_AS([] { throw SocketException(1, "Custom Message"); }(), SocketSparrowException);
    }

    SECTION("SendError") {
        REQUIRE_NOTHROW(SendError());

        REQUIRE_THROWS_WITH([] { throw SendError(); }(), "Send Error");
        REQUIRE_THROWS_WITH([] { throw SendError("Custom Message"); }(), "Custom Message");
        REQUIRE_THROWS_WITH([] { throw SendError(1); }(), "Send Error: [1] Operation not permitted");
        REQUIRE_THROWS_WITH([] { throw SendError(1, "Custom Message"); }(), "Custom Message: [1] Operation not permitted");

        REQUIRE_THROWS_AS([] { throw SendError(); }(), SendError);
        REQUIRE_THROWS_AS([] { throw SendError(1); }(), SendError);
        REQUIRE_THROWS_AS([] { throw SendError("Custom Message"); }(), SendError);
        REQUIRE_THROWS_AS([] { throw SendError(1, "Custom Message"); }(), SendError);
        REQUIRE_THROWS_AS([] { throw SendError(); }(), SocketException);
        REQUIRE_THROWS_AS([] { throw SendError(1); }(), SocketException);
        REQUIRE_THROWS_AS([] { throw SendError("Custom Message"); }(), SocketException);
        REQUIRE_THROWS_AS([] { throw SendError(1, "Custom Message"); }(), SocketException);
        REQUIRE_THROWS_AS([] { throw SendError(); }(), SocketSparrowException);
        REQUIRE_THROWS_AS([] { throw SendError(1); }(), SocketSparrowException);
        REQUIRE_THROWS_AS([] { throw SendError("Custom Message"); }(), SocketSparrowException);
        REQUIRE_THROWS_AS([] { throw SendError(1, "Custom Message"); }(), SocketSparrowException);

    }

    SECTION("RecvError") {
        REQUIRE_NOTHROW(RecvError());

        REQUIRE_THROWS_WITH([] { throw RecvError(); }(), "Receive Error");
        REQUIRE_THROWS_WITH([] { throw RecvError("Custom Message"); }(), "Custom Message");
        REQUIRE_THROWS_WITH([] { throw RecvError(1); }(), "Receive Error: [1] Operation not permitted");
        REQUIRE_THROWS_WITH([] { throw RecvError(1, "Custom Message"); }(), "Custom Message: [1] Operation not permitted");

        REQUIRE_THROWS_AS([] { throw RecvError(); }(), RecvError);
        REQUIRE_THROWS_AS([] { throw RecvError(1); }(), RecvError);
        REQUIRE_THROWS_AS([] { throw RecvError("Custom Message"); }(), RecvError);
        REQUIRE_THROWS_AS([] { throw RecvError(1, "Custom Message"); }(), RecvError);
        REQUIRE_THROWS_AS([] { throw RecvError(); }(), SocketException);
        REQUIRE_THROWS_AS([] { throw RecvError(1); }(), SocketException);
        REQUIRE_THROWS_AS([] { throw RecvError("Custom Message"); }(), SocketException);
        REQUIRE_THROWS_AS([] { throw RecvError(1, "Custom Message"); }(), SocketException);
        REQUIRE_THROWS_AS([] { throw RecvError(); }(), SocketSparrowException);
        REQUIRE_THROWS_AS([] { throw RecvError(1); }(), SocketSparrowException);
        REQUIRE_THROWS_AS([] { throw RecvError("Custom Message"); }(), SocketSparrowException);
        REQUIRE_THROWS_AS([] { throw RecvError(1, "Custom Message"); }(), SocketSparrowException);

    }
}
