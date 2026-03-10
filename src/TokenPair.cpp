#include "TokenPair.h"

TokenPair::TokenPair(listPos_t x, listPos_t y) : count{1}, first {*x}, second {*y} {
	positions.push_back(std::make_pair(x, y));
}

const tokenId_t TokenPair::getFirst() const { return first; }
const tokenId_t TokenPair::getSecond() const { return second; }
const int TokenPair::getCount() const { return count; }
const std::vector<std::pair<listPos_t, listPos_t>> TokenPair::getPositions() const { return positions; }

void TokenPair::addPosition(listPos_t x, listPos_t y) const {
	positions.push_back(std::make_pair(x, y));
	count++;
}

bool TokenPair::operator==(const TokenPair& other) const {
	return (first == other.getFirst()) && (second == other.getSecond());
}

std::size_t TokenPairHash::operator()(const TokenPair& bp) const {
	std::size_t h1 = std::hash<tokenId_t>()(bp.getFirst());
	std::size_t h2 = std::hash<tokenId_t>()(bp.getSecond());
	return h1^(h2 << 1);
}

bool CompareTokenPair::operator()(const TokenPair& bp1, const TokenPair& bp2) const {
	return bp1.getCount() > bp2.getCount();
}
