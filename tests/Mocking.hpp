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
#include <map>
#include <string>
#include <stdexcept>

 /**
  * @brief MockingController is a singleton class that controls the mocking of
  *        system calls
  */
class MockingController {
public:
    /**
     * @brief MockFunction is a class that mocks a function
     * @tparam Ret The return type of the function
     * @tparam Args The arguments of the function
     * @note This class is used to mock system calls. It copies the real function
     */
    template<typename Ret, typename... Args>
    class MockFunction {
    private:
        std::string mName;
        std::function<Ret(Args...)> mRealFunc;
        std::function<Ret(Args...)> mMockFunc;
    public:
        /**
         * @brief MockFunction is the constructor of the MockFunction class
         * @param name The name of the function
         * @param realFunc The real function
         * @param mockFunc The mock function
         */
        MockFunction(const std::string& name, Ret(*realFunc)(Args... args), Ret(*mockFunc)(Args...))
        : mName(name), mMockFunc(mockFunc) {
            mRealFunc = reinterpret_cast<Ret(*)(Args...)>(dlsym(RTLD_NEXT, name.c_str()));
            if(!mRealFunc) throw std::runtime_error("Failed to load real function");
        }

        /**
         * @brief MockFunction is the constructor of the MockFunction class
         * @param name The name of the function
         * @param realFunc The real function
         * @param mockFunc The mock function
         */
        MockFunction(const std::string& name, Ret(*realFunc)(Args...))
            : MockFunction(name, realFunc, [](Args... args) -> Ret {
            errno = EOPNOTSUPP;
            return -1;
            }) {}

        /**
         * @brief operator() is the function call operator of the MockFunction class
         * @param args The arguments
         * @return Ret The return value of the function
         */
        Ret operator()(Args... args) {
            if ( MockingController::getMockState(mName) )
                return mMockFunc(args...);
            return mRealFunc(args...);
        }

        /**
         * @brief real is a helper function that returns the real function
         * @return std::function<Ret(Args...)> The real function
         */
        std::function<Ret(Args...)> real() {
            return mRealFunc;
        }

        /**
         * @brief mock is a helper function that returns the mock function
         * @return std::function<Ret(Args...)> The mock function
         */
        std::function<Ret(Args...)> mock() {
            return mMockFunc;
        }
    };

private:
    /**
     * @brief MockGuard is a helper class that resets a boolean flag to its
     *        initial value when it goes out of scope
     */
    class MockGuard {
        bool mInitialValue;
        std::string mType;
    public:
        /**
         * @brief MockGuard is the constructor of the MockGuard class
         * @param type The type of the system call to mock
         * @param state The state of the mocking
         */
        MockGuard(const std::string& type, bool state = true)
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


    std::map<std::string, bool> mMockingState;

    MockingController() = default;
    MockingController(MockingController const&) = delete;
    void operator=(MockingController const&) = delete;

public:

    /**
     * @brief getInstance is a static function that returns the singleton
     *        instance of the MockingController
     * @return MockingController& The singleton instance of the MockingController
     */
    static auto getInstance()
        -> MockingController& {
        static MockingController instance;
        return instance;
    };

    /**
     * @brief getMockState is a helper function that returns the mocking state
     *        of a system call
     * @param name The name of the system call
     * @return bool The mocking state of the system call
     */
    static auto getMockState(const std::string& name)
        -> bool& {
        if ( getInstance().mMockingState.find(name) == getInstance().mMockingState.end() )
            getInstance().mMockingState[name] = false;
        return getInstance().mMockingState[name];
    }

    /**
     * @brief createMockGuard is a helper function that creates a MockGuard
     *        The MockGuard object resets the mocking state to its initial
     *        value when it goes out of scope
     * @param type The type of the system call to mock
     * @param state The state of the mocking
     * @return MockGuard The MockGuard object
     */
    static auto createMockGuard(const std::string& type, bool state = true)
        -> MockGuard {
        return MockGuard(type, state);
    }

    /**
     * @brief call is a helper function that calls the real or mock function
     *        based on the mocking state
     * @tparam Func The type of the function
     * @tparam Args The type of the arguments
     * @param name The name of the system call
     * @param realFunc The real function
     * @param mockFunc The mock function
     * @param args The arguments
     * @return auto The return value of the function
     */
    template<typename Func, typename... Args>
    static auto call(const std::string& name, Func realFunc, Func mockFunc, Args... args)
        -> decltype(realFunc()(args...)) {
        if ( getMockState(name) )
            return mockFunc()(args...);
        return realFunc()(args...);
    }
};
