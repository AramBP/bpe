/* This class stores a list of pointers to pairs that contain the same characters */
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <list>

using byte_t = unsigned char;
using listPos_t = std::list<byte_t>::const_iterator; 

class BytePair {
private:
	mutable std::vector<std::pair<listPos_t, listPos_t>> positions; 
	mutable int count;
	byte_t first, second;
public:
	BytePair(listPos_t x, listPos_t y);
	
	const byte_t getFirst() const;
	const byte_t getSecond() const;
	const int getCount() const;

	void addPosition(listPos_t x, listPos_t y) const;

	void print() const; 

	bool operator==(const BytePair& other) const;
};

struct BytePairHash {
	std::size_t operator()(const BytePair& bp) const;
};

struct CompareBytePair {
	bool operator()(const BytePair& bp1, const BytePair& bp2);
};
