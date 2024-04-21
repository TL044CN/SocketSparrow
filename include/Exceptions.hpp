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

namespace SocketSparrow {

    /**
     * @brief Base Exception Class for SocketSparrow
     */
    class SocketSparrowException : public std::exception {
    public:
        virtual const char* what() const noexcept override;
    };

    /**
     * @brief Exception for Socket Errors
     */
    class SocketException : public SocketSparrowException {
    public:
        virtual const char* what() const noexcept override;
    };

    class SendError : public SocketException {
    public:
        virtual const char* what() const noexcept override;
    };

    class RecvError : public SocketException {
    public:
        virtual const char* what() const noexcept override;
    };

} // namespace SocketSparrow::Exceptions