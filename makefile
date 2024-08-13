# Define the compiler
CC = g++

# Define compiler flags
CFLAGS = -std=c++17 -Wall -g

# Define the target executable
TARGET = quiz_app

# Define the source files
SRCS = main.cpp src/func/essential.cpp src/func/QuizEditing.cpp src/func/QuizCreating.cpp src/func/QuizTaking.cpp src/func/QuizDeleting.cpp

# Define the object files
OBJS = $(SRCS:.cpp=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files to .o files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -f $(TARGET) $(OBJS)
