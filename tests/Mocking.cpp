/**
 * @file Mocking.cpp
 * @author TL044CN (tl044cn@shodan.fyi)
 * @brief Implementation of Mocking system calls for testing
 * @version 0.1
 * @date 2024-05-29
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "Mocking.hpp"

namespace MockingBird {

MockingController::MockGuard::MockGuard(const std::string& type, bool state)
    : mType(type), mInitialValue(MockingController::getMockState(type)) {
    MockingController::getMockState(type) = state;
}

MockingController::MockGuard::~MockGuard() {
    MockingController::getMockState(mType) = mInitialValue;
}


auto MockingController::getInstance() -> MockingController& {
    static MockingController instance;
    return instance;
}

auto MockingController::getMockState(const std::string& name) -> bool& {
    if ( getInstance().mMockingState.find(name) == getInstance().mMockingState.end() )
        getInstance().mMockingState[name] = false;
    return getInstance().mMockingState[name];
}

auto MockingController::createMockGuard(const std::string& type, bool state) -> MockGuard {
    return MockGuard(type, state);
}

} // namespace MockingBird