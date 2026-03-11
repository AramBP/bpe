/* This class stores a list of pointers to pairs that contain the same characters */
#pragma once
#include "pch.h"

class TokenPair {
private:
	mutable std::vector<std::pair<listPos_t, listPos_t>> positions; 
	mutable int count;
	tokenId_t first, second;
public:
	TokenPair(listPos_t x, listPos_t y);
	
	tokenId_t getFirst() const;
	tokenId_t getSecond() const;
	int getCount() const;
	std::vector<std::pair<listPos_t, listPos_t>>& getPositions();

	void newPositions(listPos_t x, listPos_t y) const;
	void deletePositions(listPos_t x, listPos_t y) const;
	void print();

	bool operator==(const TokenPair& other) const;
	bool operator<(const TokenPair& rhs) const;
};

struct TokenPairHash {
	std::size_t operator()(const TokenPair& bp) const;
};

class TokenPairHeap {
private:
	std::vector<TokenPair> heap;
	void sort();
public:
	TokenPairHeap(std::list<tokenId_t>& tokens);
	TokenPair top();
	void pop();
	void addPosition(listPos_t x, listPos_t y);
	std::pair<listPos_t, listPos_t> removePosition(listPos_t x, listPos_t y);

	void print();
};