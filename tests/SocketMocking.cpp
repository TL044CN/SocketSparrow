/**
 * @file SocketMocking.cpp
 * @author TL044CN  (tl044cn@shodan.fyi)
 * @brief Definition of Mocking system calls for testing
 * @version 0.1
 * @date 2024-05-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "Mocking.hpp"
#include <arpa/inet.h>

CREATE_MOCK_FUNC_DEFINITIONS(socket, int, int, int, int)
CREATE_MOCK_FUNC_DEFINITIONS(listen, int, int, int);
CREATE_MOCK_FUNC_DEFINITIONS(accept, int, int, struct sockaddr*, socklen_t*);
CREATE_MOCK_FUNC_DEFINITIONS(setsockopt, int, int, int, int, const void*, socklen_t);

int socket(int fd, int domain, int type) {
    return MockingController::call(
        MockingController::MockType::SOCKET,
        real_socket, mock_socket,
        fd, domain, type
    );
};

int listen(int fd, int backlog) {
    return MockingController::call(
        MockingController::MockType::LISTEN,
        real_listen, mock_listen,
        fd, backlog
    );
};

int accept(int fd, struct sockaddr* addr, socklen_t* addrlen) {
    return MockingController::call(
        MockingController::MockType::ACCEPT,
        real_accept, mock_accept,
        fd, addr, addrlen
    );
};

int setsockopt(int fd, int level, int optname, const void* optval, socklen_t optlen) {
    return MockingController::call(
        MockingController::MockType::SETSOCKOPT,
        real_setsockopt, mock_setsockopt,
        fd, level, optname, optval, optlen
    );
};
