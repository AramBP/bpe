#include "Tokenizer.h"
#include "TokenPair.h"

namespace {
	/* Helper function used in bpeTrain */
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

Tokenizer::Tokenizer() {
	for (int i = 0; i < 256; ++i) {
		vocab.insert({i, i});
	}
}

void Tokenizer::bpeTrain(const std::string& text, int nMerges) {
	std::list<token_t> tokens;
	std::transform(text.begin(), text.end(), std::back_inserter(tokens), [](char c) { return static_cast<int>(c); });
	std::unordered_set<TokenPair, TokenPairHash> tokenPairs (countPairs(tokens));
	std::set<TokenPair, CompareTokenPair> h(tokenPairs.begin(), tokenPairs.end());
	
	while (nMerges != 0) {
		const auto top = h.begin();

		const token_t firstToken = top->getFirst();
		const token_t secondToken = top->getSecond();
		mergeSequence.push_back(std::make_pair(firstToken, secondToken));

		const tokenId_t nextTokenID = vocab.size() + 1;
		const token_t nextToken = vocab[firstToken] + vocab[secondToken];
		vocab.insert({nextTokenID, nextToken});

		for (const auto& [first, second] : top->getPositions()) {
			// Remove the positions from the heap
			if (first != tokens.begin()) {
				TokenPair pair (std::prev(first), first);
				h.erase(pair);
			}
			if (second != tokens.end()) {
				TokenPair pair (second, std::next(second));
				h.erase(pair);
			}

			// Update the list of tokens
			tokens.insert(first, nextTokenID);
			listPos_t tokenPos = std::prev(first);

			// Update the heap
			if (tokenPos != tokens.begin()) {
				TokenPair pair (std::prev(tokenPos), tokenPos);
				if (auto search = h.find(pair); search != h.end())
					search->addPosition(std::prev(tokenPos), tokenPos);
				else
					h.insert(pair);
			}
			if (second != tokens.end()) {
				TokenPair pair (tokenPos, std::next(second));
				if (auto search = h.find(pair); search != h.end())
					search->addPosition(tokenPos, std::next(second));
				else
					h.insert(pair);
			}
			// Delete merged tokens
			tokens.erase(first, second);
		}
		h.erase(top);
		nMerges--;
	}
}
