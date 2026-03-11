#pragma once
#include "pch.h"

class Tokenizer {
private:
	std::map<tokenId_t, token_t> vocab; // Maps a token id to a token
	std::vector<std::pair<tokenId_t, tokenId_t>> mergeSequence;
	std::list<tokenId_t> tokenize(const std::string& text) const;
public:
	Tokenizer();
	void bpeTrain(const std::string& text, int nMerges);
   	std::string decode(const std::vector<tokenId_t>& tokens) const;
	std::vector<tokenId_t> encode(const std::string& text) const;

	void printMergeSequence() const;
};
