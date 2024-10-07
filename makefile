# Define the compiler
CC = g++

# Define compiler flags
CFLAGS = -std=c++17 -Wall -g

# Define the target executable
TARGET = quiz_app

# Define the source files
SRCS = main.cpp src/func/essential.cpp src/func/QuizEditing.cpp src/func/QuizCreating.cpp src/func/QuizTaking.cpp src/func/QuizDeleting.cpp src/func/QuizLog.cpp

# Rule to build the target executable directly from source files
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean rule to remove the generated executable
clean:
	rm -f $(TARGET)