/**
 * @file mocking.hpp
 * @author TL044CN  (tl044cn@shodan.fyi)
 * @brief Mocking system calls for testing
 * @version 0.1
 * @date 2024-05-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <arpa/inet.h>
#include <dlfcn.h>
#include <errno.h>

 /**
  * @brief MockingController is a singleton class that controls the mocking of system calls
  */
class MockingController {
public:
    /**
     * @brief MockType is an enum class that represents the different types of system calls that can be mocked
     */
    enum class MockType {
        Socket,     ///< socket()
        Listen,     ///< listen()
        Accept,     ///< accept()
        Setsockopt  ///< setsockopt()
    };
private:
    /**
     * @brief MockGuard is a helper class that resets a boolean flag to its initial value when it goes out of scope
     */
    class MockGuard {
        bool mInitialValue;
        MockType mType;
    public:
        /**
         * @brief MockGuard is the constructor of the MockGuard class
         * @param type The type of the system call to mock
         * @param state The state of the mocking
         */
        MockGuard(MockType type, bool state = true)
            : mType(type), mInitialValue(MockingController::getMockState(type)) {
            MockingController::getMockState(type) = state;
        }

        /**
         * @brief ~MockGuard is the destructor of the MockGuard class
         */
        ~MockGuard() {
            MockingController::getMockState(mType) = mInitialValue;
        }
    };

    bool mockSocket = false;
    bool mockListen = false;
    bool mockAccept = false;
    bool mockSetsockopt = false;

    MockingController() {}

    MockingController(MockingController const&) = delete;
    void operator=(MockingController const&) = delete;

public:

    /**
     * @brief getInstance is a static function that returns the singleton instance of the MockingController
     * @return MockingController& The singleton instance of the MockingController
     */
    static MockingController& getInstance() {
        static MockingController instance;
        return instance;
    };

    /**
     * @brief getMockState is a helper function that returns the mocking state of a system call
     * @param type The type of the system call
     * @return bool The mocking state of the system call
     */
    static bool& getMockState(MockType type) {
        switch ( type ) {
        case MockType::Socket:
            return getInstance().mockSocket;
        case MockType::Listen:
            return getInstance().mockListen;
        case MockType::Accept:
            return getInstance().mockAccept;
        case MockType::Setsockopt:
            return getInstance().mockSetsockopt;
        }
    }

    /**
     * @brief createMockGuard is a helper function that creates a MockGuard object
     *        The MockGuard object resets the mocking state to its initial value when it goes out of scope
     * @param type The type of the system call to mock
     * @param state The state of the mocking
     * @return MockGuard The MockGuard object
     */
    static MockGuard createMockGuard(MockType type, bool state = true) {
        return MockGuard(type, state);
    }

};

// === System Call Mocking ===

/**
 * @brief Macro to create a mock function for a system call
 * @param func_name The name of the function to mock
 * @param return_type The return type of the function
 * @param ... The arguments of the function
 * @details The macro creates a mock function that sets errno to EOPNOTSUPP and returns -1
 *          The macro also creates a real function pointer that points to the original function
 *          The macro also creates a mock function pointer that points to the mock function
 * @example CREATE_MOCK_FUNC(socket, int, int, int, int);
 */
#define CREATE_MOCK_FUNC(func_name, return_type, ...) \
    typedef return_type(*func_name##_func_t)(__VA_ARGS__); \
    func_name##_func_t real_##func_name = (func_name##_func_t)dlsym(RTLD_NEXT, #func_name); \
    func_name##_func_t mock_##func_name = []( __VA_ARGS__ ) -> return_type { \
        errno = EOPNOTSUPP; \
        return -1; \
    }

CREATE_MOCK_FUNC(socket, int, int, int, int);
CREATE_MOCK_FUNC(listen, int, int, int);
CREATE_MOCK_FUNC(accept, int, int, struct sockaddr*, socklen_t*);
CREATE_MOCK_FUNC(setsockopt, int, int, int, int, const void*, socklen_t);

using MT = MockingController::MockType;

/**
 * @brief Mocking system call socket
 * @param domain The domain of the socket
 * @param type The type of the socket
 * @param protocol The protocol of the socket
 * @return int The file descriptor of the socket
 */
int socket(int domain, int type, int protocol) {
    if ( MockingController::getMockState(MT::Socket) ) {
        return mock_socket(domain, type, protocol);
    }
    return real_socket(domain, type, protocol);
}

/**
 * @brief Mocking system call listen
 * @param sockfd The file descriptor of the socket
 * @param backlog The maximum length of the queue of pending connections
 * @return int The return value of the system call
 */
int listen(int sockfd, int backlog) {
    if ( MockingController::getMockState(MT::Listen) ) {
        return mock_listen(sockfd, backlog);
    }
    return real_listen(sockfd, backlog);
}

/**
 * @brief Mocking system call accept
 * @param sockfd The file descriptor of the socket
 * @param addr The address of the client
 * @param addrlen The length of the address
 * @return int The file descriptor of the client socket
 */
int accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen) {
    if ( MockingController::getMockState(MT::Accept) ) {
        return mock_accept(sockfd, addr, addrlen);
    }
    return real_accept(sockfd, addr, addrlen);
}

/**
 * @brief Mocking system call setsockopt
 * @param sockfd The file descriptor of the socket
 * @param level The level of the option
 * @param optname The name of the option
 * @param optval The value of the option
 * @param optlen The length of the option
 * @return int The return value of the system call
 */
int setsockopt(int sockfd, int level, int optname, const void* optval, socklen_t optlen) {
    if ( MockingController::getMockState(MT::Setsockopt) ) {
        return mock_setsockopt(sockfd, level, optname, optval, optlen);
    }
    return real_setsockopt(sockfd, level, optname, optval, optlen);
}

// === End System Call Mocking ===