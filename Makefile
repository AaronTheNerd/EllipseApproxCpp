# Written by Aaron Barge
# Copyright 2022


# File Structure
SRC_DIR := src
INC_DIR := include
BLD_DIR := build
INC_H := $(wildcard $(INC_DIR)/*.h)
INC_O := $(patsubst $(INC_DIR)/%.h,$(BLD_DIR)/%.o, $(INC_H))

TEST_DIR := test

# Build Flags
CXX := g++
LXXFLAGS := -std=c++1z -I $(INC_DIR) -pthread
CXXFLAGS := -Wall -O3 -std=c++1z -I $(INC_DIR)
TEST_INC := -I /usr/local/include/gtest/
GTEST = /usr/local/lib/libgtest.a
TARGET := main

# Rules
all: $(TARGET)

$(TARGET): $(INC_O) $(BLD_DIR)/$(TARGET).o
	$(CXX) $(LXXFLAGS) -o $@ $^

$(BLD_DIR)/$(TARGET).o: $(SRC_DIR)/$(TARGET).cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BLD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

test.exe: $(INC_O) $(BLD_DIR)/test.o
	$(CXX) $(LXXFLAGS) -o $@ $^ $(GTEST)

$(BLD_DIR)/test.o: $(TEST_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) $(TEST_INC) -c $< -o $@

clean:
	$(RM) $(BLD_DIR)/* test.exe main