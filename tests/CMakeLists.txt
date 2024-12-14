# MIT License
# Copyright (c) 2024 Salem Bouraoui
#
# This project is licensed under the MIT License.
# For details, see the LICENSE.md file in the root of this repository.

# ----------------------------------------------------------------------------
# List of test sources
# ----------------------------------------------------------------------------
set(TEST_TARGETS_SOURCES
        ../src/PurgeDuplicates.cpp
)

set(UNIT_TEST_SOURCES
        tests_unit.cpp
)

set(INTEGRATION_TEST_SOURCES
        tests_integration.cpp
)

# ----------------------------------------------------------------------------
# Add unit test executable
# ----------------------------------------------------------------------------
add_executable(tests_unit ${UNIT_TEST_SOURCES} ${TEST_TARGETS_SOURCES})

# Include the src folder for headers
target_include_directories(tests_unit PUBLIC ../src)

# Link OpenSSL and source files for unit testing (if needed)
find_package(OpenSSL REQUIRED)
target_link_libraries(tests_unit PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Register the unit test executable in CTest
add_test(NAME UnitTests COMMAND tests_unit)

# ----------------------------------------------------------------------------
# Add integration test executable
# ----------------------------------------------------------------------------
add_executable(tests_integration ${INTEGRATION_TEST_SOURCES} ${TEST_TARGETS_SOURCES})

# Include the src folder for headers
target_include_directories(tests_integration PUBLIC ../src)

# Link OpenSSL and source files for integration testing (if needed)
find_package(OpenSSL REQUIRED)
target_link_libraries(tests_integration PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Register the integration test executable in CTest
add_test(NAME IntegrationTests COMMAND tests_integration)

# ----------------------------------------------------------------------------
# Debugging Information
# ----------------------------------------------------------------------------
message(STATUS "=====================================================")
message(STATUS "Configuring tests...")
message(STATUS "  Unit Test Sources        : ${UNIT_TEST_SOURCES}")
message(STATUS "  Integration Test Sources : ${INTEGRATION_TEST_SOURCES}")
message(STATUS "=====================================================")