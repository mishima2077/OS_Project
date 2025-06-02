# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -Isrc/BIOS -Isrc/c312

# Source files
SRC := src/c312/c312.cpp src/sim/sim.cpp src/BIOS/bios.cpp
OBJ := $(SRC:.cpp=.o)

# Executable name
TARGET := simulate

# Default program and debug flag
PROGRAM ?= src/OS/goated_os.txt
DEBUG ?= 0

# Build rules
all: $(TARGET)
	@echo "Running: ./$(TARGET) $(PROGRAM) -D $(DEBUG)"
	./$(TARGET) $(PROGRAM) -D $(DEBUG)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

clean:
	rm -f $(TARGET) $(OBJ)

# Run with custom program and debug flag:
#   make run PROGRAM=path/to/file.txt DEBUG=2
run: $(TARGET)
	@echo "Running: ./$(TARGET) $(PROGRAM) -D $(DEBUG)"
	./$(TARGET) $(PROGRAM) -D $(DEBUG)