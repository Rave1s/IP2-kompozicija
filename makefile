# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Targets
TARGET_DEMO = demo
TARGET_TEST = test
MODULE = RadixTree.a

# Source files
SRC = RadixTree.cpp
DEMO_SRC = demo.cpp
TEST_SRC = test.cpp

# Object files
OBJ = $(SRC:.cpp=.o)
DEMO_OBJ = $(DEMO_SRC:.cpp=.o)
TEST_OBJ = $(TEST_SRC:.cpp=.o)

# Build the Radix Tree module
$(MODULE): $(OBJ)
	ar rcs $@ $^

# Build the demo program
$(TARGET_DEMO): $(DEMO_OBJ) $(MODULE)
	$(CXX) $(CXXFLAGS) -o $@ $^ 

# Build the test program
$(TARGET_TEST): $(TEST_OBJ) $(MODULE)
	$(CXX) $(CXXFLAGS) -o $@ $^ 

run_demo: $(TARGET_DEMO)
	./$(TARGET_DEMO)

# Run tests
run_tests: $(TARGET_TEST)
	./$(TARGET_TEST)

run_all: $(TARGET_DEMO) $(TARGET_TEST)
	./$(TARGET_DEMO)
	./$(TARGET_TEST)

# Clean build files
clean:
	rm -f $(OBJ) $(DEMO_OBJ) $(TEST_OBJ) $(MODULE) $(TARGET_DEMO).exe $(TARGET_TEST).exe 2>nul

# Rebuild everything
rebuild: clean $(MODULE) $(TARGET_DEMO) $(TARGET_TEST)