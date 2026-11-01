# Author: WAM2021
# Date: 05/10/2025

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Executable name
TARGET = P4

# Source files
SRC = P4.cpp parser.cpp scanner.cpp testTree.cpp semantics.cpp codeGen.cpp
HDR = parser.h scanner.h tree.h testTree.h semantics.h codeGen.h
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)


%.o: %.cpp $(HDR)
	$(CXX) $(CXXFLAGS) -c $<

# Clean object and executable files
clean:
	del -f $(OBJ) $(TARGET)
	del -f *.exe