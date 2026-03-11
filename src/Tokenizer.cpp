#include "Tokenizer.h"
#include "TokenPair.h"
#include "pch.h"
#include <iterator>

Tokenizer::Tokenizer() {
	for (int i = 0; i < 256; ++i) {
		vocab.insert({i, std::string(1, static_cast<unsigned char>(i))});
	}
}

void Tokenizer::bpeTrain(const std::string& text, int nMerges) {
	std::list<tokenId_t> tokens = tokenize(text);
	TokenPairHeap heap (tokens);
	while (nMerges != 0) {
		TokenPair top = heap.top();
		tokenId_t firstTokenId = top.getFirst();
		tokenId_t secondTokenId = top.getSecond();
		mergeSequence.push_back(std::make_pair(firstTokenId, secondTokenId));

		tokenId_t nextTokenId = vocab.size();
		token_t nextToken = vocab.at(firstTokenId) + vocab.at(secondTokenId);
		vocab.insert({nextTokenId, nextToken});
		auto& pos = top.getPositions();
		std::pair<listPos_t, listPos_t> deletedPrev;
		std::pair<listPos_t, listPos_t> deletedNext;	

		for (std::pair<listPos_t, listPos_t> p : pos) {
			if (p == deletedPrev || p == deletedNext)
				continue;
			auto [first, second] = p;
			// Remove the positions from the heap
			if (first != tokens.begin())
				deletedPrev = heap.removePosition(std::prev(first), first);	
			if (second != std::prev(tokens.end())) 
				deletedNext = heap.removePosition(second, std::next(second));
			// Update the list of tokens
			*first = nextTokenId;
			std::next(first) = std::next(second);
			std::prev(std::next(second)) = first;
			tokens.erase(second);
			// Update the heap
			if (first != tokens.begin()) 
				heap.addPosition(std::prev(first), first);
			if (first != std::prev(tokens.end()))
				heap.addPosition(first, std::next(first));
		}
		heap.pop();
		nMerges--;
	}
}

std::list<tokenId_t> Tokenizer::tokenize(const std::string& text) const {
	std::list<tokenId_t> tokens;
	for (unsigned char c : text) {
		tokens.push_back(static_cast<int>(c));
	}
	return tokens;
}

std::string Tokenizer::decode(const std::vector<tokenId_t>& tokens) const {
	std::string res = "";
	for (const tokenId_t& token : tokens) {
		res += vocab.at(token);
	}
	return res;
}

std::vector<tokenId_t> Tokenizer::encode(const std::string& text) const {
	std::list<tokenId_t> tokens = tokenize(text);
	for (listPos_t it = tokens.begin(); it != std::prev(tokens.end());) {
		listPos_t first = it;
		listPos_t second = std::next(it);
		if (auto search = std::find(mergeSequence.begin(), mergeSequence.end(), std::make_pair(*first,*second)); search != mergeSequence.end()) {
			tokens.insert(first, 256 + std::distance(mergeSequence.begin(), search));
			tokens.erase(first);
			tokens.erase(second);
			it = tokens.begin();
		} else {
			++it;
		}
 	}
	return std::vector<tokenId_t>(tokens.begin(), tokens.end());
}

void Tokenizer::printMergeSequence() const {
	for (const auto& [first, second] : mergeSequence) {
		std::cout << vocab.at(first) << " " << vocab.at(second) << std::endl;
	}
}
