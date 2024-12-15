# Purge-Duplicates

**purge-duplicates** is a command-line tool that identifies and removes duplicate files recursively from a specified folder. It uses cryptographic **SHA-256** hashes for reliable file comparisons to ensure precise duplicate detection. Whether managing large datasets or cleaning up redundant files, this tool provides an efficient and lightweight solution.

---

## Features

- **Recursive File Scanning**: Analyzes all files within a folder, including its subdirectories.
- **Cryptographic Precision**: Utilizes SHA-256 to guarantee accurate duplicate detection.
- **Progress Display**: Optionally display progress during execution using a progress bar.
- **Cross-Platform**: Designed to work on **Linux**, **Windows**, and **macOS**.
- **Efficient and Lightweight**: Capable of processing large datasets effectively.

> **DISCLAIMER**: This software is provided **"as is"** under the MIT License. Use the software **at your own risk**. The authors are not liable for any damage, loss, or issues that may arise as a result of using this software. Always ensure you have a backup of your data before running the tool.

---

## Table of Contents

1. [Getting Started](#getting-started)
2. [Usage](#usage)
    - [Command-Line Arguments](#command-line-arguments)
3. [Building from Source](#building-from-source)
    - [Linux](#linux)
    - [Windows](#windows)
    - [Installing and Uninstalling the Software](#installing-and-uninstalling-the-software)
4. [Testing](#testing)
5. [Contributing](#contributing)
6. [License](#license)

---

## Getting Started

Download the latest release or build the software from source to get started. Refer to the [Usage](#usage) section for detailed instructions on how to use the tool.

---

## Usage

To use **Purge-Duplicates**, you can execute the compiled binary with the required arguments directly from the command line:

```bash
purge_duplicates <directory_path> [--show-progress] [--live-run]
```

### Command-Line Arguments

- `<directory_path>` (required):
  The directory path to be scanned for duplicate files.

- `--show-progress` (optional):
  Displays a progress bar in the terminal to indicate file processing progress. Useful for large datasets.

- `--live-run` (optional):
  Performs the actual deletion of duplicate files. When this flag is **not** provided, the tool will execute in **dry-run mode** and only list the duplicate files that would be deleted without making any changes.

### Example 1: Dry-Run (Default)

**Scenario**: Find duplicate files in `/home/user/documents` and only list them (dry-run mode by default).

```bash
purge_duplicates /home/user/documents
```

Output:
```plaintext
Dry Run: The following files would be deleted:
  /home/user/documents/file1.txt
  /home/user/documents/subdir/file2.txt
Dry run complete. No files were deleted.
To perform the actual deletion, re-run the command with the --live-run flag.
```

---

### Example 2: Live-Run (Deletes Duplicates)

**Scenario**: Remove duplicate files in `/home/user/documents` and show progress.

```bash
purge_duplicates /home/user/documents --show-progress --live-run
```

Output:
```plaintext
[=========================>                ] 50% 
Removed duplicate: /home/user/documents/file1.txt
Removed duplicate: /home/user/documents/subdir/file2.txt
Duplicate removal complete. Processed 500 unique files.
```

> **NOTE**: Use the `--live-run` flag cautiously, as it will permanently delete files identified as duplicates. Always run the tool in **dry-run mode** first to verify the files to be deleted.

---

### Example 3: Large Dataset (Dry-Run)

**Scenario**: Scan a folder containing a large dataset while displaying progress, but do not delete any files.

```bash
purge_duplicates /large/dataset --show-progress
```

Output:
```plaintext
[=========================>                ] 75%
Dry Run: The following files would be deleted:
  /large/dataset/sample1.txt
  /large/dataset/subfolder/sample2.txt
  ...
Dry run complete. No files were deleted.
```

By default, the program ensures no files are damaged or unexpectedly removed during testing by executing in **safe dry-run mode** unless explicitly instructed otherwise using the `--live-run` flag.

---

## Building from Source

This project uses **CMake** for cross-platform builds. Ensure you have the prerequisites installed and follow the instructions for your platform.

### Linux

1. Install prerequisites:
    - **GCC** or **Clang** compiler supporting C++17.
    - CMake version >= 3.15.
    - OpenSSL library version >= 1.0.0.

2. Clone and build the project:
   ```bash
   git clone https://github.com/jurassicLizard/purge-duplicates.git
   cd purge-duplicates
   mkdir build
   cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   make
   ```

    - The `-DCMAKE_BUILD_TYPE=Release` flag ensures the project is built in optimized **Release mode**.

3. The compiled binary will be located in the `build/bin/` folder.

4. (Optional) To install the binary system-wide:
   ```bash
   sudo cmake --install . --prefix /usr/local
   ```
    - By default, the binary will be installed to `/usr/local/bin`.

---

### Windows

The project is compatible with **Windows**, but preparation is required due to external dependencies (e.g., OpenSSL). These instructions use Visual Studio and a prebuilt OpenSSL binary.

1. Install prerequisites:
    - Visual Studio 2017 or newer with C++ tools.
    - Prebuilt [OpenSSL for Windows](https://slproweb.com/products/Win32OpenSSL.html) or compile OpenSSL manually.
    - CMake version >= 3.15.

2. Add OpenSSL to the system environment:
    - Set the `CMAKE_PREFIX_PATH` environment variable:
      ```powershell
      $env:CMAKE_PREFIX_PATH = "C:/path/to/openssl"
      ```

3. Clone and build the project:
   ```powershell
   git clone https://github.com/yourusername/purge-duplicates.git
   cd purge-duplicates
   mkdir build
   cd build
   cmake .. -G "Visual Studio 17 2022" -DCMAKE_PREFIX_PATH="C:/path/to/openssl"
   cmake --build . --config Release
   ```

4. The compiled binary can be found in the `build/bin/` folder.

---

### Installing and Uninstalling the Software

#### Installing the Software

The project includes an **install target** to simplify deployment. The `cmake --install` command ensures that the binary and associated files are installed in system-appropriate directories (e.g., `/usr/local/bin` on Linux).

1. Install the binary:
   ```bash
   cmake --install . --prefix /custom/install/path
   ```

2. Run the installed binary from anywhere:
   ```bash
   purge_duplicates /path/to/folder --show-progress
   ```

#### Uninstalling the Software

To uninstall, delete the installed files manually, e.g.:
```bash
sudo rm -f /usr/local/bin/purge_duplicates
sudo rm -rf /usr/local/include/purge-duplicates/
```

---

## Testing

The project includes built-in testing functionality.

1. Enable testing:
   ```bash
   cmake .. -DPDCPP_ENABLE_TESTING=ON
   ```

2. Run tests:
   ```bash
   ctest --output-on-failure
   ```

---

## Contributing

Contributions are welcome!

1. Fork the repository.
2. Create a new branch:
   ```bash
   git checkout -b my-feature
   ```
3. Submit a pull request.

---

## License

This project is licensed under the MIT License. See [LICENSE.md](LICENSE.md) for more details.