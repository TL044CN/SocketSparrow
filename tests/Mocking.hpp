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

#include <cstdint>
#include <dlfcn.h>
#include <cerrno>
#include <functional>
#include <cassert>

 /**
  * @brief MockingController is a singleton class that controls the mocking of
  *        system calls
  */
class MockingController {
public:
    /**
     * @brief MockType is an enum class that represents the different types of
     *        system calls that can be mocked
     */
    enum class MockType {
        SOCKET,     ///< socket()
        LISTEN,     ///< listen()
        ACCEPT,     ///< accept()
        SETSOCKOPT  ///< setsockopt()
    };
    static constexpr inline uint32_t ceMockTypeCount = 4;

    static constexpr inline uint32_t mockTypeToInt(const MockType type) {
        return static_cast<uint32_t>(type);
    }

private:
    /**
     * @brief MockGuard is a helper class that resets a boolean flag to its
     *        initial value when it goes out of scope
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

    bool mockFlag[ceMockTypeCount] = { false };

    MockingController() {}

    MockingController(MockingController const&) = delete;
    void operator=(MockingController const&) = delete;

public:

    /**
     * @brief getInstance is a static function that returns the singleton
     *        instance of the MockingController
     * @return MockingController& The singleton instance of the MockingController
     */
    static MockingController& getInstance() {
        static MockingController instance;
        return instance;
    };

    /**
     * @brief getMockState is a helper function that returns the mocking state
     *        of a system call
     * @param type The type of the system call
     * @return bool The mocking state of the system call
     */
    static bool& getMockState(const MockType type) {
        assert(mockTypeToInt(type) < ceMockTypeCount && mockTypeToInt(type) >= 0);
        uint32_t index = mockTypeToInt(type);
        return getInstance().mockFlag[index];
    }

    /**
     * @brief createMockGuard is a helper function that creates a MockGuard
     *        The MockGuard object resets the mocking state to its initial
     *        value when it goes out of scope
     * @param type The type of the system call to mock
     * @param state The state of the mocking
     * @return MockGuard The MockGuard object
     */
    static MockGuard createMockGuard(MockType type, bool state = true) {
        return MockGuard(type, state);
    }

    /**
     * @brief call is a helper function that calls the real or mock function
     *        based on the mocking state
     * @tparam Func The type of the function
     * @tparam Args The type of the arguments
     * @param type The type of the system call
     * @param realFunc The real function
     * @param mockFunc The mock function
     * @param args The arguments
     * @return auto The return value of the function
     */
    template<typename Func, typename... Args>
    static auto call(MockType type, Func realFunc, Func mockFunc, Args... args)
        -> decltype(realFunc()(args...)) {
        if ( getMockState(type) )
            return mockFunc()(args...);
        return realFunc()(args...);
    }
};

/**
 * @brief CREATE_MOCK_FUNC_DECLARATIONS is a macro that creates the necessary
 *        declarations for mocking a system call
 * @param func_name The name of the function
 * @param return_type The return type of the function
 * @param ... The arguments of the function
 */
#define CREATE_MOCK_FUNC_DECLARATIONS(func_name, return_type, ...) \
    std::function<return_type(__VA_ARGS__)>& real_##func_name();\
    std::function<return_type(__VA_ARGS__)>& mock_##func_name();

/**
 * @brief CREATE_MOCK_FUNC_DEFINITIONS is a macro that creates the necessary
 *        definitions for mocking a system call
 * @param func_name The name of the function
 * @param return_type The return type of the function
 * @param ... The arguments of the function
 */
#define CREATE_MOCK_FUNC_DEFINITIONS(func_name, return_type, ...) \
    std::function<return_type(__VA_ARGS__)>& real_##func_name(){\
        static std::function<return_type(__VA_ARGS__)> real = (return_type(*)(__VA_ARGS__))(dlsym(RTLD_NEXT, #func_name));\
        return real;\
    };\
    std::function<return_type(__VA_ARGS__)>& mock_##func_name(){\
        static std::function<return_type(__VA_ARGS__)> mock = [](__VA_ARGS__) -> return_type { \
            errno = EOPNOTSUPP; \
            return -1; \
        };\
        return mock;\
    };
