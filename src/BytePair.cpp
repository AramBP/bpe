#include "BytePair.h"

BytePair::BytePair(listPos_t x, listPos_t y) : count{1}, first {*x}, second {*y} {
	positions.push_back(std::make_pair(x, y));
}

const byte_t BytePair::getFirst() const { return first; }
const byte_t BytePair::getSecond() const { return second; }
const int BytePair::getCount() const { return count; }

void BytePair::addPosition(listPos_t x, listPos_t y) const {
	positions.push_back(std::make_pair(x, y));
	count++;
}

void BytePair::print() const {
	std::cout << "(" << first << ", " << second << ") : " << count << std::endl;
}

bool BytePair::operator==(const BytePair& other) const {
	return (first == other.getFirst()) && (second == other.getSecond());
}

std::size_t BytePairHash::operator()(const BytePair& bp) const {
	std::size_t h1 = std::hash<byte_t>()(bp.getFirst());
	std::size_t h2 = std::hash<byte_t>()(bp.getSecond());
	return h1^(h2 << 1);
}

bool CompareBytePair::operator()(const BytePair& bp1, const BytePair& bp2) {
	return bp1.getCount() < bp2.getCount();
}
