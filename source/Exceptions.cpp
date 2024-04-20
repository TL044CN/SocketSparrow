#include "Exceptions.hpp"

namespace SocketSparrow::Exceptions {

    const char* SocketSparrowException::what() const noexcept {
        return "SocketSparrow Exception";
    }

} // namespace SocketSparrow::Exceptions