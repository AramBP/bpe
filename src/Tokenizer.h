#pragma once
#include "pch.h"

using token_t = int;
using tokenId_t = int;

class Tokenizer {
private:
	std::map<tokenId_t, token_t> vocab; // Maps a token id to a utf-8 byte
	std::vector<std::pair<tokenId_t, tokenId_t>> mergeSequence;
	void bpeTrain(const std::string& text, int nMerges);
public:
	Tokenizer();
   	std::string decode(std::vector<tokenId_t> tokens);
	std::vector<tokenId_t> encode(std::string text);
};
