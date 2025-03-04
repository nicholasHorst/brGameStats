# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -I/opt/homebrew/include

# Linker flags
LDFLAGS = -L/opt/homebrew/lib -ljsoncpp

# Target executable
TARGET = leaderboard_parser

# Source files
SRCS = leaderboard.cpp

# Object files (replace .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

# Default rule to build the executable
all: $(TARGET)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(TARGET)

# Clean compiled files
clean:
	rm -f $(OBJS) $(TARGET)
