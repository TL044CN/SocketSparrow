#include "Exceptions.hpp"

namespace SocketSparrow {

    const char* SocketSparrowException::what() const noexcept {
        return "SocketSparrow Exception";
    }

    const char* SocketException::what() const noexcept {
        return "Socket Exception";
    }

    const char* SendError::what() const noexcept {
        return "Send Error";
    }

    const char* RecvError::what() const noexcept {
        return "Receive Error";
    }

} // namespace SocketSparrow::Exceptions