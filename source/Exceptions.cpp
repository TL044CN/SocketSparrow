#include "Exceptions.hpp"
#include "Util.hpp"

#include <format>
#include <cstring>
#include <openssl/err.h>

namespace SocketSparrow {

using namespace Util;

SocketSparrowException::SocketSparrowException():mMessage("SocketSparrow Exception") {}
SocketSparrowException::SocketSparrowException(const std::string& message): mMessage(message) {}

const char* SocketSparrowException::what() const noexcept {
    return mMessage.c_str();
}


EndpointException::EndpointException(const std::string& message): SocketSparrowException(message) {}

InvalidAddressFamilyException::InvalidAddressFamilyException(const std::string& message)
    : EndpointException(message) {}
InvalidAddressFamilyException::InvalidAddressFamilyException(AddressFamily family, const std::string& message)
    :EndpointException(message + ": " + getAddressFamilyString(family)) {}


InvalidPortException::InvalidPortException(const std::string& message): EndpointException(message) {}
InvalidPortException::InvalidPortException(int port, const std::string& message)
    : EndpointException(std::format("{}: {}", message, port)) {}

InvalidAddressException::InvalidAddressException(const std::string& message)
    : EndpointException(message) {}
InvalidAddressException::InvalidAddressException(const std::string& address, const std::string& message)
    : EndpointException(message + ": " + address) {}


SocketException::SocketException(): SocketSparrowException("Socket Exception") {}
SocketException::SocketException(const std::string& message): SocketSparrowException(message) {}
SocketException::SocketException(int error, const std::string& message)
    : SocketSparrowException(std::format("{}: [{}] {}", message, error, strerror(error))) {}

TLSSocketException::TLSSocketException(): SocketException("TLS Socket Exception") {}
TLSSocketException::TLSSocketException(const std::string& message): SocketException(message) {}
TLSSocketException::TLSSocketException(int error, const std::string& message) {
    mMessage = std::format(
        "{}: [{}] {}",
        message, error, ERR_error_string(error, nullptr)
    );
}

SendError::SendError(): SocketException("Send Error") {}
SendError::SendError(const std::string& message): SocketException(message) {}
SendError::SendError(int error, const std::string& message): SocketException(error, message) {}

RecvError::RecvError(): SocketException("Receive Error") {}
RecvError::RecvError(const std::string& message): SocketException(message) {}
RecvError::RecvError(int error, const std::string& message): SocketException(error, message) {}

} // namespace SocketSparrow::Exceptions