CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
LDFLAGS = -lcrypto

SRC = src/main.cpp src/auth.cpp src/bank.cpp src/util.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = build/ventura_bank

all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build *.o src/*.o

.PHONY: all clean
