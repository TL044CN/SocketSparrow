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

template<typename... Args>
using MF = MockingBird::MockingController::MockFunction<Args...>;

int socket(int domain, int type, int protocol) {
    static MF<int, int, int, int> mock_socket("socket", ::socket);

    return mock_socket(domain, type, protocol);
}

int listen(int sockfd, int backlog) {
    static MF<int, int, int> mock_listen("listen", ::listen);

    return mock_listen(sockfd, backlog);
}

int accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen) {
    static MF<int, int, struct sockaddr*, socklen_t*> mock_accept("accept", ::accept);

    return mock_accept(sockfd, addr, addrlen);
}

int setsockopt(int sockfd, int level, int optname, const void* optval, socklen_t optlen) {
    static MF<int, int, int, int, const void*, socklen_t>
        mock_setsockopt("setsockopt", ::setsockopt);

    return mock_setsockopt(sockfd, level, optname, optval, optlen);
}
