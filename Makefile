CXX = g++
CXXFLAGS = -Wall -std=c++17 
TARGET = bin/bpe
SRC = src/main.cpp src/BytePair.cpp src/bpe.cpp

all: $(TARGET)

$(TARGET) : $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
