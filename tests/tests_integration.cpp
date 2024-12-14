/*
 * MIT License
 *
 * Copyright (c) 2024 Salem Bouraoui
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * ---
 *
 * Portions of this software were generated using AI-based tools (e.g., OpenAI's GPT).
 * The generated code has been reviewed, modified, and integrated into this project
 * by the author, Salem Bouraoui.
 */
#include "../src/PurgeDuplicates.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <cassert>
#include <random>
#include <chrono>

namespace fs = std::filesystem;

// Helper function to write binary data to a file
void write_binary_file(const std::string& path, const std::vector<char>& content) {
    std::ofstream file(path, std::ios::binary);
    file.write(content.data(), content.size());
}

// Helper function to generate random binary data
std::vector<char> generate_random_binary_data(size_t size) {
    std::vector<char> data(size);
    std::default_random_engine rng(
            std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(0, 255);

    for (auto& byte : data) {
        byte = static_cast<char>(dist(rng));
    }
    return data;
}

// Test 1: Large Number of Files with Mixed Binary and ASCII Content
void test_large_number_of_mixed_files() {
    const std::string testDir = fs::temp_directory_path() / "test_large_mixed_files";
    if (fs::exists(testDir)) fs::remove_all(testDir);

    fs::create_directory(testDir);

    // Create 1000 files: 500 duplicates (ASCII and binary), 500 unique (ASCII and binary)
    for (int i = 0; i < 1000; ++i) {
        std::string filePath = testDir + "/file" + std::to_string(i);
        if (i < 250) {
            // ASCII duplicates
            std::ofstream(filePath + ".txt") << "Duplicate ASCII content";
        } else if (i < 500) {
            // Binary duplicates
            write_binary_file(filePath + ".bin", {0x01, 0x02, 0x03, 0x04});
        } else if (i < 750) {
            // Unique ASCII files
            std::ofstream(filePath + ".txt") << "Unique ASCII content " + std::to_string(i);
        } else {
            // Unique binary files
            write_binary_file(filePath + ".bin", generate_random_binary_data(64));
        }
    }

    // Run PurgeDuplicates
    PurgeDuplicates pd(testDir, false);
    pd.execute();

    // Verify results: 250 ASCII duplicates and 250 binary duplicates should reduce to 2 unique files
    size_t remainingFiles = 0;
    for (const auto& entry : fs::directory_iterator(testDir)) {
        if (entry.is_regular_file()) {
            remainingFiles++;
        }
    }

    assert(remainingFiles == 502); // 2 single copies of duplicates + 500 unique files

    std::cout << "Test Passed: Large number of mixed files handled correctly." << std::endl;

    // Cleanup
    fs::remove_all(testDir);
}

// Test 2: Deeply Nested Directories with Binary Files
void test_deeply_nested_directories_with_binary_files() {
    const std::string testDir = fs::temp_directory_path() / "test_nested_binary_dirs";
    if (fs::exists(testDir)) fs::remove_all(testDir);

    fs::create_directory(testDir);

    // Create a nested directory structure
    std::string nestedDir = testDir;
    for (int i = 0; i < 10; ++i) {
        nestedDir += "/level" + std::to_string(i);
        fs::create_directory(nestedDir);

        // Add duplicate binary files and unique binary files at each level
        write_binary_file(nestedDir + "/duplicate.bin", {0x01, 0x02, 0x03});
        write_binary_file(nestedDir + "/unique" + std::to_string(i) + ".bin", generate_random_binary_data(32));
    }

    // Run PurgeDuplicates
    PurgeDuplicates pd(testDir, false);
    pd.execute();

    // Verify results
    size_t remainingFiles = 0;
    for (const auto& entry : fs::recursive_directory_iterator(testDir)) {
        if (entry.is_regular_file()) {
            remainingFiles++;
        }
    }

    assert(remainingFiles == 11); // 1 duplicate + 10 unique

    std::cout << "Test Passed: Deeply nested directories with binary files handled correctly." << std::endl;

    // Cleanup
    fs::remove_all(testDir);
}

// Test 3: Mixed Binary and ASCII Edge Cases
void test_binary_and_ascii_edge_cases() {
    const std::string testDir = fs::temp_directory_path() / "test_binary_ascii_edge_cases";
    if (fs::exists(testDir)) fs::remove_all(testDir);

    fs::create_directory(testDir);

    // Create ASCII and binary files with unusual names
    write_binary_file(testDir + "/binary_long_" + std::string(200, 'a') + ".bin", {0x01, 0x02, 0x03, 0x04});
    write_binary_file(testDir + "/binary_space .bin", {0x01, 0x02, 0x03, 0x04});
    write_binary_file(testDir + "/binary_unicode_✓.bin", {0x01, 0x02, 0x03, 0x04});
    std::ofstream(testDir + "/ascii_long_" + std::string(200, 'b') + ".txt") << "ASCII content";
    std::ofstream(testDir + "/ascii_unicode_✓.txt") << "Unicode ASCII content";

    // Run PurgeDuplicates
    PurgeDuplicates pd(testDir, false);
    pd.execute();

    // Verify results
    bool binaryFileExists = fs::exists(testDir + "/binary_long_" + std::string(200, 'a') + ".bin");
    bool asciiFileExists = fs::exists(testDir + "/ascii_long_" + std::string(200, 'b') + ".txt");

    assert(binaryFileExists == true); // Unique files should remain
    assert(asciiFileExists == true);
    std::cout << "Test Passed: Binary and ASCII edge cases handled correctly." << std::endl;

    // Cleanup
    fs::remove_all(testDir);
}

int main() {
    // Run all tests
    test_large_number_of_mixed_files(); // Mixed ASCII and binary files
    test_deeply_nested_directories_with_binary_files(); // Deeply nested with binary
    test_binary_and_ascii_edge_cases(); // Binary and ASCII edge cases

    std::cout << "All integration tests passed!" << std::endl;
    return 0;
}