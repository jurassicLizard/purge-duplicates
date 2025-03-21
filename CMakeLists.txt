# MIT License
# Copyright (c) 2024 Salem Bouraoui
#
# This project is licensed under the MIT License.
# For details, see the LICENSE.md file in the root of this repository.

cmake_minimum_required(VERSION 3.21)
project(files-deduplicator VERSION 0.4.1)

# ----------------------------------------------------------------------------
# Executable name
# ----------------------------------------------------------------------------
set(EXECUTABLE_NAME rmdup)
# ----------------------------------------------------------------------------
# C++ Standard
# ----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----------------------------------------------------------------------------
# Find OpenSSL with minimum version requirement
# ----------------------------------------------------------------------------
find_package(OpenSSL 1.1.1 REQUIRED)

# Debugging: Output OpenSSL version and library path
message(STATUS "Detected OpenSSL version: ${OPENSSL_VERSION}")
get_target_property(OPENSSL_LIB_PATH OpenSSL::SSL LOCATION)
message(STATUS "Using OpenSSL library at: ${OPENSSL_LIB_PATH}")

# Validate OpenSSL version
if (NOT OPENSSL_FOUND)
    message(FATAL_ERROR "OpenSSL not found. Please install OpenSSL 1.0.0 or newer.")
elseif (NOT OPENSSL_VERSION)
    message(FATAL_ERROR "OpenSSL version not defined. Please check your OpenSSL installation.")
elseif (OPENSSL_VERSION VERSION_LESS "1.0.0")
    message(FATAL_ERROR "OpenSSL version ${OPENSSL_VERSION} is too old. Please install version 1.0.0 or newer.")
else()
    message(STATUS "OpenSSL version ${OPENSSL_VERSION} is compatible.")
endif()

# ----------------------------------------------------------------------------
# Set Binary Output Directory
# ----------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ----------------------------------------------------------------------------
# Version Configuration
# ----------------------------------------------------------------------------
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/version.hpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/generated/version.hpp
)

# Add the generated directory to the include path
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated)

# ----------------------------------------------------------------------------
# Add the src/ directory
# ----------------------------------------------------------------------------
add_subdirectory(src)

# ----------------------------------------------------------------------------
# Optional Settings
# ----------------------------------------------------------------------------
# Static OpenSSL Option
option(PDCPP_USE_STATIC_OPENSSL "Use a statically linked OpenSSL if possible" OFF)
if (PDCPP_USE_STATIC_OPENSSL)
    set(OPENSSL_USE_STATIC_LIBS TRUE)
endif()

# ----------------------------------------------------------------------------
# Testing Options
# ----------------------------------------------------------------------------
option(PDCPP_ENABLE_TESTING "Enable testing for the ${PROJECT_NAME} project" OFF)
option(PDCPP_ENABLE_TESTING_WITH_ASAN "Enable extended testing with enabled AddressSanitizer (ASan)" OFF)
option(ENABLE_ASAN "Enable AddressSanitizer (ASan) for testing" OFF)

if (PDCPP_ENABLE_TESTING_WITH_ASAN)
    set(PDCPP_ENABLE_TESTING ON CACHE BOOL "Enable testing for the project" FORCE)
    set(ENABLE_ASAN ON CACHE BOOL "Enable AddressSanitizer (ASan) for testing" FORCE)
endif()

if (PDCPP_ENABLE_TESTING OR PDCPP_ENABLE_TESTING_WITH_ASAN)
    enable_testing()
    add_subdirectory(tests)
endif()

# ----------------------------------------------------------------------------
# Install Targets
# ----------------------------------------------------------------------------
# Install the binary (executable)
install(TARGETS ${EXECUTABLE_NAME}
        RUNTIME DESTINATION bin                         # Destination for executables
        LIBRARY DESTINATION lib                         # Destination for shared libraries
        ARCHIVE DESTINATION lib                         # Destination for static libraries
)

# If there are public headers, install them too
# (e.g., if you distribute a reusable library or API)
install(DIRECTORY src/
        DESTINATION include/${EXECUTABLE_NAME}           # Install headers into an include directory
        FILES_MATCHING PATTERN "*.hpp"                 # Only match header files
)

# ----------------------------------------------------------------------------
# Build Configuration Summary
# ----------------------------------------------------------------------------
message(STATUS "=====================================================")
message(STATUS "Configuring ${EXECUTABLE_NAME}...")
message(STATUS "  C++ Standard      : ${CMAKE_CXX_STANDARD}")
message(STATUS "  OpenSSL Found     : ${OPENSSL_FOUND}")
message(STATUS "  OpenSSL Version   : ${OPENSSL_VERSION}")
message(STATUS "  OpenSSL Library   : ${OPENSSL_LIB_PATH}")
message(STATUS "  Testing Enabled   : ${PDCPP_ENABLE_TESTING}")
message(STATUS "  ASan Enabled      : ${ENABLE_ASAN}")
message(STATUS "=====================================================")