#include "Exceptions.hpp"
#include <cstring>

namespace SocketSparrow {

const char* SocketSparrowException::what() const noexcept {
    return "SocketSparrow Exception";
}

SocketException::SocketException(): mMessage("Socket Exception") {}
SocketException::SocketException(const std::string& message): mMessage(message) {}
SocketException::SocketException(int error, const std::string& message) {
    mMessage = message + ": [" + std::to_string(error) + "] " + strerror(error);
}

const char* SocketException::what() const noexcept {
    return mMessage.c_str();
}

SendError::SendError(): SocketException("Send Error") {}
SendError::SendError(const std::string& message): SocketException(message) {}
SendError::SendError(int error, const std::string& message): SocketException(error, message) {}

RecvError::RecvError(): SocketException("Receive Error") {}
RecvError::RecvError(const std::string& message): SocketException(message) {}
RecvError::RecvError(int error, const std::string& message): SocketException(error, message) {}

} // namespace SocketSparrow::Exceptions