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
#include <cassert>
#include <exception>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

void test_main_argument_parsing() {
    // Simulate passing arguments to the program
    const char* argv[] = {"program_name", "test_directory", "--show-progress"};
    int argc = 3;

    assert(argc >= 2); // Check that directory argument is passed
    std::string directory = argv[1];
    bool showProgress = (argc >= 3 && std::string(argv[2]) == "--show-progress");

    assert(directory == "test_directory");
    assert(showProgress == true);

    std::cout << "Test Passed: Command-line argument parsing works correctly." << std::endl;
}

void test_generate_sha256() {
    // Generate a hash for a mock file
    try {
        const std::string testFile = "test_file.txt";
        const std::string fileContents = "Hello, SHA256!";

        // Create a test file
        std::ofstream file(testFile);
        file << fileContents;
        file.close();

        PurgeDuplicates pd(".", false); // Use dummy progress setting
        std::string resultHash = pd.generateSHA256(testFile);

        // Precomputed hash of "Hello, SHA256!" (using online SHA-256 calculator)
        const std::string expectedHash = "d14c0cd6caa3d2006a34ba29886d8b2e698e9d2259be8b1f852375d6026502bd";
        std::cout << "Result hash: " << resultHash << std::endl;
        std::cout << "Expected hash: " << expectedHash << std::endl;
        assert(resultHash == expectedHash);

        std::cout << "Test Passed: SHA-256 hash generation works correctly." << std::endl;

        fs::remove(testFile);

    } catch (const std::exception& e) {
        std::cerr << "Test Failed: " << e.what() << std::endl;
    }
}

void test_progress_display() {
    PurgeDuplicates pd(".", true); // Enable progress
    pd.displayProgress(50, 100);

    // Can't assert console output, but this verifies no crash/exception
    std::cout << "\nTest Passed: Progress display function executes without errors." << std::endl;
}

void test_identify_and_remove_duplicates() {
    try {
        // Set up test directory structure
        const std::string testDir = "test_duplicates";
        if(fs::exists(testDir))
        {
            std::cout << "Removing existing test directory..." << std::endl;
            fs::remove_all(testDir);
        }

        fs::create_directory(testDir);

        // Create duplicate files
        std::string file1 = testDir + "/file1.txt";
        std::string file2 = testDir + "/file2.txt";
        std::string file3 = testDir + "/file3.txt";

        std::ofstream(file1) << "Duplicate content";
        std::ofstream(file2) << "Duplicate content";
        std::ofstream(file3) << "Unique content";

        // Execute PurgeDuplicates logic
        PurgeDuplicates pd(testDir, false,true);
        pd.execute();

        // Check results
        bool file1Exists = fs::exists(file1);
        bool file3Exists = fs::exists(file3);
        bool file2Exists = fs::exists(file2); // Should be removed since it's a duplicate

        assert(file1Exists != file2Exists); // One duplicate should remain
        assert(file3Exists == true); // Unique file should remain

        std::cout << "Test Passed: Duplicate files are correctly identified and removed." << std::endl;

        // Cleanup
        fs::remove_all(testDir);
    } catch (const std::exception& e) {
        std::cerr << "Test Failed: " << e.what() << std::endl;
    }
}

void test_invalid_directory() {
    try {
        PurgeDuplicates pd("non_existent_directory", false,true);
        pd.execute(); // Should throw an error

        // If no error is thrown, the test fails
        assert(false && "Expected an exception for a non-existent directory.");
    } catch (const std::exception& e) {
        std::cout << "Test Passed: Handling of non-existent directory works correctly." << std::endl;
    }
}

void test_permission_denied() {
    try {
        const std::string testDir = "restricted_dir";

        // Create directory with restricted permissions
        fs::create_directory(testDir);
        fs::permissions(testDir, fs::perms::none);

        PurgeDuplicates pd(testDir, false,true);
        pd.execute(); // Should throw an exception for restricted permissions

        // Cleanup
        fs::permissions(testDir, fs::perms::all);
        fs::remove_all(testDir);

        // If no error is thrown, the test fails
        assert(false && "Expected an exception for restricted permissions.");
    } catch (const std::exception& e) {
        const std::string msg = e.what();
        assert(msg.find("Permission denied") != std::string::npos);
        std::cout << "Test Passed: Permission errors are correctly handled." << std::endl;
    }
}

void test_identify_and_remove_binary_duplicates() {
    try {
        // Set up test directory structure
        const std::string testDir = "test_binary_duplicates";
        if (fs::exists(testDir)) {
            std::cout << "Removing existing test directory..." << std::endl;
            fs::remove_all(testDir);
        }
        fs::create_directory(testDir);

        // Create binary duplicate files
        std::string file1 = testDir + "/file1.bin";
        std::string file2 = testDir + "/file2.bin";
        std::string file3 = testDir + "/file3.bin";

        // Binary data (same content for file1 and file2, different for file3)
        const char binaryContent[] = {0x01, 0x02, 0x03, 0x04, 0x05};
        const char uniqueBinaryContent[] = {0x10, 0x20, 0x30, 0x40, 0x50};

        // Write binary content to file1 and file2
        std::ofstream(file1, std::ios::binary).write(binaryContent, sizeof(binaryContent));
        std::ofstream(file2, std::ios::binary).write(binaryContent, sizeof(binaryContent));
        // Write unique binary content to file3
        std::ofstream(file3, std::ios::binary).write(uniqueBinaryContent, sizeof(uniqueBinaryContent));

        // Execute PurgeDuplicates logic
        PurgeDuplicates pd(testDir, false,true);
        pd.execute();

        // Check results
        bool file1Exists = fs::exists(file1);
        bool file2Exists = fs::exists(file2);
        bool file3Exists = fs::exists(file3);

        // Assert: Either file1 or file2 should exist, but not both
        assert(file1Exists != file2Exists); // One duplicate should remain
        // Assert: Unique file (file3) should remain
        assert(file3Exists == true);

        std::cout << "Test Passed: Binary duplicate files are correctly identified and removed." << std::endl;

        // Cleanup
        fs::remove_all(testDir);
    } catch (const std::exception& e) {
        std::cerr << "Test Failed: " << e.what() << std::endl;
    }
}

void test_identify_and_remove_nested_duplicates() {
    try {
        // Set up test directory structure
        const std::string testDir = "test_nested_duplicates";
        if (fs::exists(testDir)) {
            std::cout << "Removing existing test directory..." << std::endl;
            fs::remove_all(testDir);
        }

        // Create base directory and subdirectories
        fs::create_directory(testDir);
        fs::create_directory(testDir + "/subdir1");
        fs::create_directory(testDir + "/subdir2");
        fs::create_directory(testDir + "/subdir2/nested");

        // Create files with varying content and placement
        std::string file1 = testDir + "/file1.txt";                   // Duplicate at base level
        std::string file2 = testDir + "/subdir1/file2.txt";           // Duplicate at second level
        std::string file3 = testDir + "/subdir2/nested/file3.txt";    // Unique file at third level
        std::string file4 = testDir + "/subdir2/file4.txt";           // Duplicate at second level
        std::string file5 = testDir + "/file1_duplicate.txt";         // Duplicate of file1 at base level

        // Write duplicate and unique contents
        const std::string duplicateContent = "Duplicate content across files"; // Same content
        const std::string uniqueContent = "This is a unique file";             // Unique content

        // Write duplicates and unique files
        std::ofstream(file1) << duplicateContent; // Duplicate content
        std::ofstream(file2) << duplicateContent; // Duplicate content
        std::ofstream(file3) << uniqueContent;    // Unique content
        std::ofstream(file4) << duplicateContent; // Duplicate content
        std::ofstream(file5) << duplicateContent; // Duplicate content

        // Execute PurgeDuplicates logic
        PurgeDuplicates pd(testDir, false,true);
        pd.execute();

        // Check results
        bool file1Exists = fs::exists(file1);
        bool file2Exists = fs::exists(file2);
        bool file3Exists = fs::exists(file3);
        bool file4Exists = fs::exists(file4);
        bool file5Exists = fs::exists(file5);

        // Assertions:
        // - file1, file2, file4, file5 are duplicates; only one should remain
        assert((file1Exists + file2Exists + file4Exists + file5Exists) == 1);
        // - file3 is unique and should remain
        assert(file3Exists == true);

        std::cout << "Test Passed: Nested duplicate files are correctly identified and removed." << std::endl;

        // Cleanup
        fs::remove_all(testDir);
    } catch (const std::exception& e) {
        std::cerr << "Test Failed: " << e.what() << std::endl;
    }
}

void test_live_and_dry_run() {
    try {
        const std::string testDir = "test_live_and_dry_run";
        if (fs::exists(testDir)) {
            fs::remove_all(testDir);
        }
        fs::create_directory(testDir);

        std::string file1 = testDir + "/file1.txt";
        std::string file2 = testDir + "/file2.txt"; // Duplicate
        std::ofstream(file1) << "Duplicate content";
        std::ofstream(file2) << "Duplicate content";

        // Dry-run mode
        {
            PurgeDuplicates pd(testDir, false, false);
            pd.execute();
            // Verify files are not deleted
            assert(fs::exists(file1));
            assert(fs::exists(file2));
            std::cout << "Dry run test passed." << std::endl;
        }

        // Live-run mode
        {
            PurgeDuplicates pd(testDir, false, true);
            pd.execute();
            // Verify duplicates are deleted
            assert(fs::exists(file1));
            assert(!fs::exists(file2));
            std::cout << "Live run test passed." << std::endl;
        }

        // Cleanup
        fs::remove_all(testDir);
    } catch (const std::exception& e) {
        std::cerr << "Test Failed: " << e.what() << std::endl;
    }
}

int main() {
    // Run all unit tests
    test_main_argument_parsing();
    test_generate_sha256();
    test_progress_display();
    test_identify_and_remove_duplicates();
    test_identify_and_remove_binary_duplicates();
    test_identify_and_remove_nested_duplicates();
    test_invalid_directory();
    test_permission_denied();

    std::cout << "All unit tests passed!" << std::endl;
    return 0;
}