# Hash Table Bid System

**Course:** CS300 - Data Structures & Algorithms

**Author:** Justin Guida

https://github.com/user-attachments/assets/537b99c2-9017-40db-82b1-be16d49a217e



## Project Overview

This project is a C++ application developed for the CS300 Data Structures course. It implements a hash table to load, store, and manage bid data from a CSV file. The program provides a console-based interface for users to interact with the bid data.

## Prerequisites

Before you begin, ensure you have the following installed on your system:
*   A C++ compiler (like GCC or Clang)
*   CMake (version 3.25 or higher)
*   Make or Ninja build tools

## How to Compile and Run

This project includes a simple script to compile and run the application on macOS, Linux, and Windows (with a bash-compatible shell like Git Bash).

### Quick Launch (Mac)

Simply double-click `scripts/launch.command` in Finder to automatically build and run the application.

### Using Make

The simplest way to build and run the project:

```bash
make        # Build the project
make run    # Build and run
make clean  # Clean build files
```

### Using the `run.sh` script

Alternatively, use the provided shell script:

```bash
./scripts/run.sh
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
├── Makefile
├── License.txt
├── README.md
├── src/
│   ├── HashTable.cpp
│   ├── CSVparser.cpp
│   └── CSVparser.hpp
├── data/
│   ├── eBid_Monthly_Sales.csv
│   └── eBid_Monthly_Sales_Dec_2016.csv
└── scripts/
    ├── run.sh            # Build and run script
    └── launch.command    # Double-click to run on Mac
```

## Key Features & Implementation Details

The `HashTable.cpp` file was the primary focus of this assignment. The following features and improvements were implemented:

*   **Hash Table with Chaining:** A hash table was built from the ground up to store bid information. It uses the chaining method with linked lists to resolve hash collisions, ensuring that multiple items hashing to the same bucket can be stored correctly.

*   **String-Based Hashing:** The hash function uses `std::hash<string>` to hash alphanumeric `bidId` keys into bucket indices, allowing flexible support for any string-based identifiers.

*   **Enhanced User Interface:**
    *   **ANSI Colors:** The console menu and output were improved with ANSI color codes to make the interface more readable and user-friendly.
    *   **User Pause Function:** A `pauseForUser()` function was added to prevent the menu from redisplaying instantly, giving the user time to read output from previous operations.

*   **Code Quality and Robustness:**
    *   **`const` Correctness:** Function parameters were tightened using `const` references where appropriate. This improves performance by avoiding unnecessary copies and enhances code safety by preventing accidental modification of data.
    *   **Input Validation:** The main menu loop includes input guards to validate user input, preventing crashes from non-numeric entries and gracefully guiding the user.

*   **Memory Management:** A custom destructor (`~HashTable()`) was implemented to manually deallocate all dynamically created `Node` objects, preventing memory leaks when the hash table is destroyed.
