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
CFLAGS := -Wall -O3 -std=c++1z -pthread
INCFLAG := -I $(INC_DIR)
TARGET := main

# Rules
all: $(TARGET)

$(TARGET): $(INC_O) $(BLD_DIR)/$(TARGET).o
	$(CXX) $(CFLAGS) -o $@ $^

$(BLD_DIR)/$(TARGET).o: $(SRC_DIR)/$(TARGET).cpp
	$(CXX) $(CFLAGS) $(INCFLAG) -c $< -o $@

$(BLD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.h
	$(CXX) $(CFLAGS) $(INCFLAG) -c $< -o $@

test.exe: $(INC_O) $(BLD_DIR)/test.o
	$(CXX) $(CFLAGS) -o $@ $^

$(BLD_DIR)/test.o: $(TEST_DIR)/main.cpp
	$(CXX) $(CFLAGS) $(INCFLAG) -c $< -o $@

clean:
	$(RM) $(BLD_DIR)/* test.exe main