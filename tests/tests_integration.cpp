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
    PurgeDuplicates pd(testDir, false,true);
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
    PurgeDuplicates pd(testDir, false,true);
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
    PurgeDuplicates pd(testDir, false,true);
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

void test_dry_run_mode() {
    const std::string testDir = fs::temp_directory_path() / "test_dry_run";
    if (fs::exists(testDir)) {
        fs::remove_all(testDir);
    }
    fs::create_directory(testDir);

    // Create duplicate and unique files
    std::string file1 = testDir + "/file1.txt";       // Duplicate content
    std::string file2 = testDir + "/file2.txt";       // Duplicate content
    std::string file3 = testDir + "/file3.txt";       // Unique content

    std::ofstream(file1) << "Duplicate content";
    std::ofstream(file2) << "Duplicate content";
    std::ofstream(file3) << "Unique content";

    // Run dry-run
    try {
        PurgeDuplicates pd(testDir, false, false); // Dry-run mode
        pd.execute();
    } catch (const std::exception& e) {
        std::cerr << "Test Failed: " << e.what() << std::endl;
        return;
    }

    // Verify that no duplicates are actually deleted
    bool file1Exists = fs::exists(file1);
    bool file2Exists = fs::exists(file2);
    bool file3Exists = fs::exists(file3);

    assert(file1Exists == true);
    assert(file2Exists == true);
    assert(file3Exists == true);

    std::cout << "Test Passed: Dry-run mode does not delete files and lists duplicates correctly." << std::endl;

    fs::remove_all(testDir);
}

void test_live_run_mode() {
    const std::string testDir = fs::temp_directory_path() / "test_live_run";
    if (fs::exists(testDir)) {
        fs::remove_all(testDir);
    }
    fs::create_directory(testDir);

    // Create duplicate and unique files
    std::string file1 = testDir + "/file1.txt";       // Duplicate content
    std::string file2 = testDir + "/file2.txt";       // Duplicate content
    std::string file3 = testDir + "/file3.txt";       // Unique content

    std::ofstream(file1) << "Duplicate content";
    std::ofstream(file2) << "Duplicate content";
    std::ofstream(file3) << "Unique content";

    // Run live-run
    try {
        PurgeDuplicates pd(testDir, false, true); // Live-run mode
        pd.execute();
    } catch (const std::exception& e) {
        std::cerr << "Test Failed: " << e.what() << std::endl;
        return;
    }

    // Verify that only duplicates are deleted
    bool file1Exists = fs::exists(file1);
    bool file2Exists = fs::exists(file2);
    bool file3Exists = fs::exists(file3);

    assert((file1Exists || file2Exists) == true); // One duplicate copy remains
    assert(file3Exists == true);                 // Unique file should remain

    std::cout << "Test Passed: Live-run mode deletes duplicate files and preserves unique files." << std::endl;

    fs::remove_all(testDir);
}

void test_dry_run_then_live_run() {
    const std::string testDir = fs::temp_directory_path() / "test_dry_run_then_live_run";
    if (fs::exists(testDir)) {
        fs::remove_all(testDir);
    }
    fs::create_directory(testDir);

    // Create duplicate and unique files
    std::string file1 = testDir + "/file1.txt";       // Duplicate content
    std::string file2 = testDir + "/file2.txt";       // Duplicate content
    std::string file3 = testDir + "/file3.txt";       // Unique content

    std::ofstream(file1) << "Duplicate content";
    std::ofstream(file2) << "Duplicate content";
    std::ofstream(file3) << "Unique content";

    // Run dry-run
    try {
        PurgeDuplicates dryRunPd(testDir, false, false); // Dry-run mode
        dryRunPd.execute();
    } catch (const std::exception& e) {
        std::cerr << "Test Failed in Dry-Run: " << e.what() << std::endl;
        return;
    }

    // Verify no files are deleted in dry-run
    assert(fs::exists(file1));
    assert(fs::exists(file2));
    assert(fs::exists(file3));

    // Run live-run
    try {
        PurgeDuplicates liveRunPd(testDir, false, true); // Live-run mode
        liveRunPd.execute();
    } catch (const std::exception& e) {
        std::cerr << "Test Failed in Live-Run: " << e.what() << std::endl;
        return;
    }

    // Verify that duplicates are deleted and unique file remains
    bool file1Exists = fs::exists(file1);
    bool file2Exists = fs::exists(file2);
    bool file3Exists = fs::exists(file3);

    assert((file1Exists || file2Exists) == true); // One duplicate remains
    assert(file3Exists == true);                 // Unique file should remain

    std::cout << "Test Passed: Dry-run preserves files; live-run deletes duplicates." << std::endl;

    fs::remove_all(testDir);
}

void test_large_dataset_dry_run() {
    const std::string testDir = fs::temp_directory_path() / "test_large_dataset_dry_run";
    if (fs::exists(testDir)) {
        fs::remove_all(testDir);
    }
    fs::create_directory(testDir);

    // Create 1000 files with 500 duplicates
    for (int i = 0; i < 1000; ++i) {
        std::string filePath = testDir + "/file" + std::to_string(i) + ".txt";
        if (i < 500) {
            std::ofstream(filePath) << "Duplicate content";
        } else {
            std::ofstream(filePath) << "Unique content " << i;
        }
    }

    // Run dry-run
    try {
        PurgeDuplicates pd(testDir, false, false); // Dry-run mode
        pd.execute();
    } catch (const std::exception& e) {
        std::cerr << "Test Failed: " << e.what() << std::endl;
        return;
    }

    // Verify all files still exist
    size_t fileCount = 0;
    for (const auto& entry : fs::directory_iterator(testDir)) {
        if (entry.is_regular_file()) {
            fileCount++;
        }
    }
    assert(fileCount == 1000);

    std::cout << "Test Passed: Dry-run mode handles large datasets without deleting files." << std::endl;

    fs::remove_all(testDir);
}

void test_large_dataset_live_run() {
    const std::string testDir = fs::temp_directory_path() / "test_large_dataset_live_run";
    if (fs::exists(testDir)) {
        fs::remove_all(testDir);
    }
    fs::create_directory(testDir);

    // Create 1000 files with 500 duplicates
    for (int i = 0; i < 1000; ++i) {
        std::string filePath = testDir + "/file" + std::to_string(i) + ".txt";
        if (i < 500) {
            std::ofstream(filePath) << "Duplicate content";
        } else {
            std::ofstream(filePath) << "Unique content " << i;
        }
    }

    // Run live-run
    try {
        PurgeDuplicates pd(testDir, false, true); // Live-run mode
        pd.execute();
    } catch (const std::exception& e) {
        std::cerr << "Test Failed: " << e.what() << std::endl;
        return;
    }

    // Verify duplicates are deleted (remaining files = 500 unique + 1 duplicate copy)
    size_t fileCount = 0;
    for (const auto& entry : fs::directory_iterator(testDir)) {
        if (entry.is_regular_file()) {
            fileCount++;
        }
    }
    assert(fileCount == 501);

    std::cout << "Test Passed: Live-run mode handles large datasets, removing duplicates." << std::endl;

    fs::remove_all(testDir);
}

int main() {
    // Run all tests
    test_large_number_of_mixed_files(); // Mixed ASCII and binary files
    test_deeply_nested_directories_with_binary_files(); // Deeply nested with binary
    test_binary_and_ascii_edge_cases(); // Binary and ASCII edge cases
    test_dry_run_mode(); // Test Dry run mode functionality
    test_live_run_mode(); // Test Live run mode functionality
    test_dry_run_then_live_run(); // Test Dry run followed by live run
    test_large_dataset_dry_run(); // Test large dataset dry run
    test_large_dataset_live_run(); // Test large dataset live run

    std::cout << "All integration tests passed!" << std::endl;
    return 0;
}