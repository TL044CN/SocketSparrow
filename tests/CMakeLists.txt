# Fetch Catch2
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)

# Setup Catch2
FetchContent_GetProperties(Catch2)
if(NOT catch2_POPULATED)
  FetchContent_Populate(Catch2)
  add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR})
endif()

FetchContent_Declare(
  MockingBird
  GIT_REPOSITORY https://github.com/TL044CN/Mockingbird.git
  GIT_TAG        1.0.5
)

# Setup Catch2
FetchContent_GetProperties(MockingBird)
if(NOT mockingbird_POPULATED)
  FetchContent_Populate(MockingBird)
  add_subdirectory(${mockingbird_SOURCE_DIR} ${mockingbird_BINARY_DIR})
endif()

# Add tests
add_executable(${PROJECT_NAME}_tests
  SocketMocking.cpp
  test_main.cpp
  test_Utils.cpp
  test_Endpoint.cpp
  test_Socket.cpp
  test_TLSSocket.cpp
  test_Exceptions.cpp
)

# Link required libraries
target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE
        Catch2::Catch2
        MockingBird
        ${PROJECT_NAME}
)

# Include directories
target_include_directories(${PROJECT_NAME}_tests
    PRIVATE
#        ${catch2_SOURCE_DIR}/include
        ${${PROJECT_NAME}_INCLUDE_DIR}
)

# Set test properties
set_target_properties(${PROJECT_NAME}_tests PROPERTIES
    TEST_PREFIX "test_"
    TEST_SUFFIX ""
)

# Discover tests
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
include(Catch)
catch_discover_tests(${PROJECT_NAME}_tests)

# Create coverage target
if(NOT ${PROJECT_NAME}_IS_SUBMODULE)

  if(GCOV AND LCOV AND GENHTML)
    message("Compiler: ${CMAKE_CXX_COMPILER_ID}")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
        target_compile_options(${PROJECT_NAME}_tests PRIVATE -fprofile-arcs -ftest-coverage)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
        target_compile_options(${PROJECT_NAME}_tests PRIVATE -fprofile-instr-generate -fcoverage-mapping)
    endif()
    message("Coverage flags: ${CMAKE_CXX_FLAGS}")


    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        message("Test executable: ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_tests")
        add_custom_target(coverage
        COMMAND ${CMAKE_MAKE_PROGRAM} test
        COMMAND ${GCOV} -r -pb tests/CMakeFiles/${PROJECT_NAME}_tests.dir/*.gcno CMakeFiles/${PROJECT_NAME}.dir/source/*.gcno
        COMMAND ${LCOV} --capture --directory . --output-file coverage.info
        COMMAND ${LCOV} --remove coverage.info '*catch2*' '*test_*' '/usr/*' --output-file coverage.info
        COMMAND ${GENHTML} coverage.info --output-directory out
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running GCOV and generating report"
      )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      set(PROFRAW_FILE "${CMAKE_BINARY_DIR}/tests/${PROJECT_NAME}-%p.profraw")
      set(PROFDATA_FILE "${CMAKE_BINARY_DIR}/tests/coverage.profdata")
      set(DEFAULT_PROFRAW_FILE "${CMAKE_BINARY_DIR}/tests/default.profraw")

      add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE="${PROFRAW_FILE}" ${CMAKE_MAKE_PROGRAM} test
        COMMAND llvm-profdata merge -sparse ${CMAKE_BINARY_DIR}/tests/${PROJECT_NAME}-*.profraw ${DEFAULT_PROFRAW_FILE} -o ${PROFDATA_FILE}
        COMMAND ${GCOV} export -format=lcov ${CMAKE_BINARY_DIR}/tests/${PROJECT_NAME}_tests -instr-profile=${PROFDATA_FILE} > coverage.lcov
        COMMAND ${LCOV} --remove coverage.lcov '*catch2*' '*test_*' '/usr/*' --output-file coverage.lcov
        COMMAND ${LCOV} --summary coverage.lcov
        COMMAND ${GENHTML} coverage.lcov --output-directory out
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running llvm-cov and generating report"
    )  
    endif()
  endif()

endif()
