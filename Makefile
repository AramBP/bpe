CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude -O2
TARGET = bin/bpe

SRC = src/main.cpp src/Tokenizer.cpp src/ds/TokenList.cpp src/ds/TokenPairQueue.cpp
OBJS = $(SRC:.cpp = .o)

all: $(TARGET)

$(TARGET) : $(OBJS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
