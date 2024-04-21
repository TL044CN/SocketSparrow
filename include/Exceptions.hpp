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
#include <exception>
#include <string>

namespace SocketSparrow {

    /**
     * @brief Base Exception Class for SocketSparrow
     */
    class SocketSparrowException : public std::exception {
    public:
        explicit SocketSparrowException() = default;

        /**
         * @brief Returns the Exception Message
         * @return const char* Exception Message
         */
        virtual const char* what() const noexcept override;
    };

    /**
     * @brief Exception for Socket Errors
     *        This is the base class for all Socket Exceptions
     */
    class SocketException : public SocketSparrowException {
    protected:
        std::string mMessage;
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

        /**
         * @brief Returns the Exception Message
         * @return const char* Exception Message
         */
        virtual const char* what() const noexcept override;
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