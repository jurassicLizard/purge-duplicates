# MIT License
# Copyright (c) 2024 Salem Bouraoui
#
# This project is licensed under the MIT License.
# For details, see the LICENSE.md file in the root of this repository.



# ----------------------------------------------------------------------------
# Specify sources and headers
# ----------------------------------------------------------------------------
set(SOURCES
        main.cpp
        PurgeDuplicates.cpp
)

set(HEADERS
        PurgeDuplicates.hpp
)

# ----------------------------------------------------------------------------
# Add the main executable
# ----------------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${SOURCES})

# Link OpenSSL to the main executable
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Include current directory for headers
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ----------------------------------------------------------------------------
# Debugging Information
# ----------------------------------------------------------------------------
message(STATUS "=====================================================")
message(STATUS "Building purge-duplicates executable...")
message(STATUS "  Source Files       : ${SOURCES}")
message(STATUS "  Header Files       : ${HEADERS}")
message(STATUS "=====================================================")