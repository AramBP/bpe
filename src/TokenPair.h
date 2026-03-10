/* This class stores a list of pointers to pairs that contain the same characters */
#pragma once
#include "pch.h"
#include "types.h"

class TokenPair {
private:
	mutable std::vector<std::pair<listPos_t, listPos_t>> positions; 
	mutable int count;
	tokenId_t first, second;
public:
	TokenPair(listPos_t x, listPos_t y);
	
	const tokenId_t getFirst() const;
	const tokenId_t getSecond() const;
	const int getCount() const;
	const std::vector<std::pair<listPos_t, listPos_t>> getPositions() const;

	void addPosition(listPos_t x, listPos_t y) const;

	bool operator==(const TokenPair& other) const;
};

struct TokenPairHash {
	std::size_t operator()(const TokenPair& bp) const;
};

struct CompareTokenPair {
	bool operator()(const TokenPair& bp1, const TokenPair& bp2) const;
};
