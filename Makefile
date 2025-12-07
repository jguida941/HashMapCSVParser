# Makefile for Hash Table Bid System
# CS300 - Data Structures & Algorithms

CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g
SRC_DIR = src
BUILD_DIR = build
TARGET = HashMap

SRCS = $(SRC_DIR)/HashTable.cpp $(SRC_DIR)/CSVparser.cpp
OBJS = $(BUILD_DIR)/HashTable.o $(BUILD_DIR)/CSVparser.o

.PHONY: all clean run

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(BUILD_DIR)/HashTable.o: $(SRC_DIR)/HashTable.cpp $(SRC_DIR)/CSVparser.hpp
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -c $< -o $@

$(BUILD_DIR)/CSVparser.o: $(SRC_DIR)/CSVparser.cpp $(SRC_DIR)/CSVparser.hpp
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)