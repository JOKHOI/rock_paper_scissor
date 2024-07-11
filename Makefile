# Step 1: Define variables for later use
CC = gcc                # Set the compiler to gcc
CFLAGS = -Wall -g       # Set compiler flags to show all warnings and include debug info
TARGET = main           # Set the name of the output executable to 'main'

# Step 2: List source files needed for the project
SRCS = main.c src/game.c # List of source files to compile

# Step 3: Define the default action (what happens when you type 'make')
all: build run          # The 'all' target depends on building the program and then running it

# Step 4: Specify how to build the program
build:                  # 'build' target does not depend on other targets
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) # Compile and link the source files to create 'main'

# Step 5: Specify how to run the program
run:                    # 'run' target does not depend on other targets
	./$(TARGET)       # Execute the 'main' program

# Step 6: Define how to clean up build files
clean:                  # 'clean' target does not depend on other targets
	rm -f $(TARGET)   # Remove the 'main' executable file

# Step 7: Declare phony targets to avoid conflicts with files of the same name
.PHONY: all build run clean # Ensure these targets are always executed when requested

