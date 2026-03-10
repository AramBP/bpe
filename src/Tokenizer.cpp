#include "Tokenizer.h"
#include "TokenPair.h"

namespace {
	std::unordered_set<TokenPair, TokenPairHash> countPairs (const std::list<token_t>& l) {
		std::unordered_set<TokenPair, TokenPairHash> counts;
		for (auto it = l.begin(); it != std::prev(l.end()); ++it) {
			TokenPair pair (it, std::next(it));
			if (auto search = counts.find(pair); search != counts.end()) {
				search->addPosition(it, std::next(it));
			} else {
				counts.insert(pair);
			}
		}
		return counts;
	}
}
/*
void Tokenizer::bpeTrain(const std::string& text, int nMerges) {
	std::list<token_t> bytes(text.begin(), text.end());
	std::unordered_set<TokenPair, TokenPairHash> bytePairs (countPairs(bytes));
	std::set<TokenPair, CompareTokenPair> h(bytePairs.begin(), bytePairs.end());
	
	for (int i = 0; i != nMerges; ++i) {
		const auto& pos = *(h.begin());

		tokenId_t nextToken = 256 + i;
		mergeSequence.push_back(std::make_pair())

		for (const auto& [first, second] : pos) {
			// Remove the positions from the heap
			if (first != bytes.begin()) {
				TokenPair pair (std::prev(first), first);
				h.erase(pair);
			}
			if (second != bytes.end()) {
				TokenPair pair (second, std::next(second));
				h.erase(pair);
			}

			// Perform the merge
			*(first) = *(first) + *(second);
			std::next(first) = std::next(second);
			std::prev(std::next(second)) = first;

			// Add the new tokens to the string
			if (first != bytes.begin()) {
				TokenPair pair (std::prev(first), first);
				if (auto search = h.find(pair); search != h.end())
					search->addPosition(std::prev(first), first);
				else
					h.insert(pair);
			}
			if (second != bytes.end()) {
				TokenPair pair (second, std::next(second));
				if (auto search = h.find(pair); search != h.end())
					search->addPosition(second, std::next(second));
				else
					h.insert(pair);
			}
		}
		h.erase(pos);
	}
}
*/
Tokenizer::Tokenizer() {
	for (int i = 0; i < 256; ++i) {
		vocab.insert({i, i});
	}
}
