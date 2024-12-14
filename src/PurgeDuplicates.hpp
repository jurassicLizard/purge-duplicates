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
#ifndef PURGE_DUPLICATES_HPP
#define PURGE_DUPLICATES_HPP

#include <string>

class PurgeDuplicates {
public:
    /**
     * @brief Constructor to initialize the PurgeDuplicates object.
     * @param directory Path to the directory that will be processed.
     * @param showProgress Whether to display a progress bar or not.
     */
    PurgeDuplicates(const std::string& directory, bool showProgress);

    /**
     * @brief Executes the logic for identifying and removing duplicates.
     */
    void execute();

/**
 * @brief Generates the SHA-256 hash of a file's contents.
 * @param filePath The file to generate the hash for.
 * @return The SHA-256 hash as a hexadecimal string.
 * @throws std::runtime_error If the hash generation fails.
 * @throws std::ios_base::failure If the file cannot be opened.
 */
std::string generateSHA256(const std::string& filePath);

/**
 * @brief Displays a progress bar in the console.
 * @param current The current progress count.
 * @param total The total number of files to process.
 */
void displayProgress(size_t current, size_t total);

private:
    std::string directoryPath; // The path to the target directory
    bool showProgress;         // Flag to indicate if a progress bar is displayed

    /**
     * @brief Identifies and removes duplicate files in a directory.
     * This is the main logic for processing the directory.
     */
    void identifyAndRemoveDuplicates();
};

#endif // PURGE_DUPLICATES_HPP