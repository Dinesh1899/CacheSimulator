# Define the compiler
CXX = g++

# Define the target executable
TARGET_DIR=build
TARGET = $(TARGET_DIR)/hello

# Define the source file
SRC = src/hello.cpp

# Define the object file
OBJ_DIR = objects
OBJ = $(OBJ_DIR)/hello.o

# Default target
all: $(TARGET)
	./$(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJ)
	$(CXX) -o $(TARGET) $(OBJ)

# Rule to build the object file
$(OBJ): $(SRC)
	$(CXX) -c $(SRC) -o $(OBJ)

# Clean up generated files
clean:
	rm -rf $(TARGET_DIR)/* $(OBJ_DIR)/*
