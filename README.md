# Hash Table Bid System

**Course:** CS300 - Data Structures & Algorithms
**Author:** Justin Guida

## Project Overview

This project is a C++ application developed for the CS300 Data Structures course. It implements a hash table to load, store, and manage bid data from a CSV file. The program provides a console-based interface for users to interact with the bid data.

## Prerequisites

Before you begin, ensure you have the following installed on your system:
*   A C++ compiler (like GCC or Clang)
*   CMake (version 3.0 or higher)
*   Make or Ninja build tools

## How to Compile and Run

This project includes a simple script to compile and run the application on macOS, Linux, and Windows (with a bash-compatible shell like Git Bash).

### Using the `run.sh` script

This is the simplest way to build and run the project on any system.

1.  Open a terminal or command prompt.
2.  Navigate to the root directory of the project.
3.  Run the following command:
    ```bash
    ./run.sh
    ```

This script will automatically:
- Create a build directory (`cmake-build-debug`).
- Run CMake to configure the project.
- Compile the source code.
- Launch the application.

## Project Structure
```
.
├── CMakeLists.txt
├── CSVparser.cpp
├── CSVparser.hpp
├── HashTable.cpp
├── License.txt
├── README.md
└── run.sh
```

## Key Features & Implementation Details

The `HashTable.cpp` file was the primary focus of this assignment. The following features and improvements were implemented:

*   **Hash Table with Chaining:** A hash table was built from the ground up to store bid information. It uses the chaining method with linked lists to resolve hash collisions, ensuring that multiple items hashing to the same bucket can be stored correctly.

*   **String-Based Hashing:** The hash function was overloaded to support modern alphanumeric string keys (`bidId`), making the system more robust and flexible compared to using only integer-based keys.

*   **Enhanced User Interface:**
    *   **ANSI Colors:** The console menu and output were improved with ANSI color codes to make the interface more readable and user-friendly.
    *   **User Pause Function:** A `pauseForUser()` function was added to prevent the menu from redisplaying instantly, giving the user time to read output from previous operations.

*   **Code Quality and Robustness:**
    *   **`const` Correctness:** Function parameters were tightened using `const` references where appropriate. This improves performance by avoiding unnecessary copies and enhances code safety by preventing accidental modification of data.
    *   **Input Validation:** The main menu loop includes input guards to validate user input, preventing crashes from non-numeric entries and gracefully guiding the user.

*   **Memory Management:** A custom destructor (`~HashTable()`) was implemented to manually deallocate all dynamically created `Node` objects, preventing memory leaks when the hash table is destroyed.
