# Makefile for Minesweeper
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra
TARGET = minesweeper
SOURCES = main.cpp board.cpp game.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Build the target executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean compiled files
clean:
	del /Q *.o $(TARGET).exe 2>nul || true

# Build and run the program
run: $(TARGET)
	./$(TARGET)

# Mark these as "phony targets" (not files)
.PHONY: clean run