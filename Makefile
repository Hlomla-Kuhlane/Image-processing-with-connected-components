# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -g -std=c++17

# Target executables
MAIN_TARGET = findcomp
TEST_TARGET = tests

# Source files
MAIN_SRCS = findcomp.cpp PGMimageProcessor.cpp PGMimage.cpp ConnectedComponent.cpp
TEST_SRCS = tests.cpp PGMimageProcessor.cpp PGMimage.cpp ConnectedComponent.cpp

# Object files
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

# Default target (builds main program)
all: $(MAIN_TARGET)

# Main application
$(MAIN_TARGET): $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Test executable needs catch.hpp implementation
$(TEST_TARGET): $(TEST_OBJS) catch.hpp
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_OBJS)
	@echo "Running tests..."
	@./$@

# Test target (build and run tests)
test: $(TEST_TARGET)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f *.o $(MAIN_TARGET) $(TEST_TARGET) *.pgm

# Phony targets
.PHONY: all clean test