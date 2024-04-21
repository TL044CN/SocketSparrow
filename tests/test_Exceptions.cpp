#include "catch2/catch.hpp"
#include "Exceptions.hpp"

using namespace SocketSparrow;

TEST_CASE("Exception Classes", "[Exceptions]") {
    SECTION("SocketSparrowException") {
        REQUIRE_NOTHROW(SocketSparrowException());

        REQUIRE_THROWS_WITH([] { throw SocketSparrowException(); }(), "SocketSparrow Exception");

        REQUIRE_THROWS_AS([] { throw SocketSparrowException(); }(), SocketSparrowException);
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
