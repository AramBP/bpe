CXX = g++
CXXFLAGS = -Wall -std=c++17 -O2
TARGET = bin/bpe
PCH_SRC = src/pch.h
PCH_OUT = src/pch.h.gch

SRC = src/main.cpp src/TokenPair.cpp src/Tokenizer.cpp
OBJS = $(SRC:.cpp = .o)

all: $(TARGET)

$(TARGET) : $(PCH_OUT) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Rule to compile the PCH
$(PCH_OUT) : $(PCH_SRC)
	$(CXX) $(CXXFLAGS) -x c++-header $(PCH_SRC) -o $(PCH_OUT)

%.o: %.cpp $(PCH_OUT)
	$(CXX) $(CXXFLAGS) -include $(PCH_SRC) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(PCH_OUT)
