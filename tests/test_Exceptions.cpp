#include "catch2/catch.hpp"
#include "Exceptions.hpp"

using namespace SocketSparrow;

TEST_CASE("Exception Classes", "[Exceptions]") {
    SECTION("SocketSparrowException") {        
        REQUIRE_NOTHROW(SocketSparrowException());
        try {
            throw SocketSparrowException();
        } catch(const SocketSparrowException& e){
            REQUIRE(std::string(e.what()) == "SocketSparrow Exception");
        } catch(...) {
            FAIL("Unexpected exception");
        }
    }
    SECTION("SocketException") {
        REQUIRE_NOTHROW(SocketException());
        try {
            throw SocketException();
        } catch(const SocketException& e){
            REQUIRE(std::string(e.what()) == "Socket Exception");
        } catch(...) {
            FAIL("Unexpected exception");
        }
    }
    SECTION("SendError") {
        REQUIRE_NOTHROW(SendError());
        try {
            throw SendError();
        } catch(const SendError& e){
            REQUIRE(std::string(e.what()) == "Send Error");
        } catch(...) {
            FAIL("Unexpected exception");
        }
    }
    SECTION("RecvError") {
        REQUIRE_NOTHROW(RecvError());
        try {
            throw RecvError();
        } catch(const RecvError& e){
            REQUIRE(std::string(e.what()) == "Receive Error");
        } catch(...) {
            FAIL("Unexpected exception");
        }
    }
}
