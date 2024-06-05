/**
 * @file Exceptions.hpp
 * @author TL044CN
 * @brief Exception Classes for SocketSparrow
 * @version 0.1
 * @date 2024-04-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "Enums.hpp"

#include <exception>
#include <string>

namespace SocketSparrow {

/**
 * @brief Base Exception Class for SocketSparrow
 */
class SocketSparrowException : public std::exception {
protected:
    std::string mMessage;
public:
    /**
     * @brief Construct a new Socket Sparrow Exception object
     */
    explicit SocketSparrowException();

    /**
     * @brief Construct a new Socket Sparrow Exception object
     *
     * @param message the message to display
     */
    explicit SocketSparrowException(const std::string& message);

    /**
     * @brief Returns the Exception Message
     * @return const char* Exception Message
     */
    const char* what() const noexcept override;
};


/**
 * @brief Exception for Endpoint Errors
 *        This is thrown when an Endpoint operation fails
 */
class EndpointException : public SocketSparrowException {
public:

    /**
     * @brief Construct a new Endpoint Exception object
     *
     * @param message the message to display
     */
    explicit EndpointException(const std::string& message = "Endpoint Exception");

};

/**
 * @brief Exception for Invalid Address Family
 *        This is thrown when an invalid Address Family is used
 */
class InvalidAddressFamilyException : public EndpointException {
public:
    /**
     * @brief Construct a new Invalid Address Family object
     *
     * @param message the message to display
     */
    explicit InvalidAddressFamilyException(const std::string& message = "Invalid Address Family");

    /**
     * @brief Construct a new Invalid Address Family object
     *
     * @param family the Address Family
     * @param message the message to display
     */
    explicit InvalidAddressFamilyException(
        AddressFamily family,
        const std::string& message = "Invalid Address Family"
    );

};

/**
 * @brief Exception for Invalid Port
 *        This is thrown when an invalid Port is used
 */
class InvalidPortException : public EndpointException {
public:
    /**
     * @brief Construct a new Invalid Port object
     *
     * @param message the message to display
     */
    explicit InvalidPortException(const std::string& message = "Invalid Port");

    /**
     * @brief Construct a new Invalid Port object
     *
     * @param message the message to display
     */
    explicit InvalidPortException(int port, const std::string& message = "Invalid Port");

};

/**
 * @brief Exception for Invalid Address
 *        This is thrown when an invalid Address is used
 */
class InvalidAddressException : public EndpointException {
public:
    /**
     * @brief Construct a new Invalid Address object
     */
    explicit InvalidAddressException(const std::string& = "Invalid Address");

    /**
     * @brief Construct a new Invalid Address object
     *
     * @param message the message to display
     */
    explicit InvalidAddressException(
        const std::string& address,
        const std::string& message
    );
};


/**
 * @brief Exception for Socket Errors
 *        This is the base class for all Socket Exceptions
 */
class SocketException : public SocketSparrowException {
public:
    /**
     * @brief Construct a new Socket Exception object
     */
    explicit SocketException();

    /**
     * @brief Construct a new Socket Exception object
     *
     * @param message the message to display
     */
    explicit SocketException(const std::string& message);

    /**
     * @brief Construct a new Socket Exception object
     *
     * @param error the error code
     * @param message the message to display
     */
    explicit SocketException(int error, const std::string& message = "Socket Exception");

};

/**
 * @brief Exception for TLS Socket Errors
 *        This is thrown when a TLS Socket operation fails
 */
class TLSSocketException : public SocketException {
public:
    /**
     * @brief Construct a new TLS Socket Exception object
     */
    explicit TLSSocketException();

    /**
     * @brief Construct a new TLS Socket Exception object
     *
     * @param message the message to display
     */
    explicit TLSSocketException(const std::string& message);

    /**
     * @brief Construct a new TLS Socket Exception object
     *
     * @param error the error code
     * @param message the message to display
     */
    explicit TLSSocketException(int error, const std::string& message = "TLS Socket Exception");

};

/**
 * @brief Exception for Send Errors
 *        This is thrown when a Send operation fails
 */
class SendError : public SocketException {
public:
    /**
     * @brief Construct a new Send Error object
     */
    explicit SendError();

    /**
     * @brief Construct a new Send Error object
     *
     * @param message the message to display
     */
    explicit SendError(const std::string& message);

    /**
     * @brief Construct a new Send Error object
     *
     * @param error the error code
     * @param message the message to display
     */
    explicit SendError(int error, const std::string& message = "Send Error");


};

/**
 * @brief Exception for Receive Errors
 *        This is thrown when a Receive operation fails
 */
class RecvError : public SocketException {
public:
    /**
     * @brief Construct a new Receive Error object
     */
    explicit RecvError();

    /**
     * @brief Construct a new Receive Error object
     *
     * @param message the message to display
     */
    explicit RecvError(const std::string& message);

    /**
     * @brief Construct a new Receive Error object
     *
     * @param error the error code
     * @param message the message to display
     */
    explicit RecvError(int error, const std::string& message = "Receive Error");
};

} // namespace SocketSparrow::Exceptions