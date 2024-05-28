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
#pragma once

#include "Mocking.hpp"
#include <arpa/inet.h>

CREATE_MOCK_FUNC_DECLARATIONS(socket, int, int, int, int);
CREATE_MOCK_FUNC_DECLARATIONS(listen, int, int, int);
CREATE_MOCK_FUNC_DECLARATIONS(accept, int, int, struct sockaddr*, socklen_t*);
CREATE_MOCK_FUNC_DECLARATIONS(setsockopt, int, int, int, int, const void*, socklen_t);
