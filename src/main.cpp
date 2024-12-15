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
#include "PurgeDuplicates.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // Argument usage check
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <directory_path> [--show-progress] [--live-run]" << std::endl;
        return 1;
    }

    const std::string directory = argv[1];
    bool showProgress = false;
    bool liveRun = false;

    // Parse additional arguments
    for (int i = 2; i < argc; ++i) {
        std::string argument = argv[i];
        if (argument == "--show-progress") {
            showProgress = true;
        } else if (argument == "--live-run") {
            liveRun = true;
        }
    }

    try {
        // Pass the new flag to PurgeDuplicates
        PurgeDuplicates purgeDuplicates(directory, showProgress, liveRun);
        purgeDuplicates.execute(); // Begin execution
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}