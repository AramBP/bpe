/* This class stores a list of pointers to pairs that contain the same characters */
#pragma once
#include "pch.h"

using token_t = int;
using listPos_t = std::list<token_t>::const_iterator;

class TokenPair {
private:
	mutable std::vector<std::pair<listPos_t, listPos_t>> positions; 
	mutable int count;
	token_t first, second;
public:
	TokenPair(listPos_t x, listPos_t y);
	
	const token_t getFirst() const;
	const token_t getSecond() const;
	const int getCount() const;
	const std::vector<std::pair<listPos_t, listPos_t>> getPositions() const;

	void addPosition(listPos_t x, listPos_t y) const;

	void print() const; 

	bool operator==(const TokenPair& other) const;
};

struct TokenPairHash {
	std::size_t operator()(const TokenPair& bp) const;
};

struct CompareTokenPair {
	bool operator()(const TokenPair& bp1, const TokenPair& bp2);
};
