# Define the compiler and flags
CXX = g++


# Define the target executable
TARGET_DIR=build
TARGET = $(TARGET_DIR)/hello

# Define the source files
SRCS = src/hello.cpp src/request_handlers.cpp src/cache_mem.cpp

# Define the object file
OBJ_DIR = objects
OBJS = objects/hello.o objects/request_handlers.o objects/cache_mem.o

# COMMAND LINE ARGS - CACHE PARAMS
L1_SIZE = 1024
L1_ASSOC = 1
L1_BLOCKSIZE = 16 
VC_NUM_BLOCKS = 0
L2_SIZE = 8192
L2_ASSOC = 4
TRACEFILE = gcc_trace.txt

# Default target
all: $(TARGET)

# Rule to link object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) objects/hello.o

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: src/%.cpp
	$(CXX) -c $< -o $@

# Clean up the build files
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET) $(L1_SIZE) $(L1_ASSOC) $(L1_BLOCKSIZE) $(VC_NUM_BLOCKS) $(L2_SIZE) $(L2_ASSOC) $(TRACEFILE)   > logs/output.log