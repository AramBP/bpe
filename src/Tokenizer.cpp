#include "Tokenizer.h"
#include "BytePair.h"

namespace {
	std::unordered_set<BytePair, BytePairHash> countPairs (const std::list<byte_t>& l) {
		std::unordered_set<BytePair, BytePairHash> counts;
		for (auto it = l.begin(); it != std::prev(l.end()); ++it) {
			BytePair pair (it, std::next(it));
			if (auto search = counts.find(pair); search != counts.end()) {
				search->addPosition(it, std::next(it));
			} else {
				counts.insert(pair);
			}
		}
		return counts;
	}
}

void Tokenizer::bpeTrain(const std::string& text, int nMerges) {
	std::list<byte_t> bytes(text.begin(), text.end());
	std::unordered_set<BytePair, BytePairHash> bytePairs (countPairs(bytes));
	std::priority_queue<BytePair, std::vector<BytePair>, CompareBytePair> q(bytePairs.begin(), bytePairs.end());
	
	for (int i = 0; i != nMerges; ++i) {

	}
}

Tokenizer::Tokenizer() {
	for (int i = 0; i < 256; ++i) {
		vocab.insert({i, static_cast<byte_t>(i)});
	}
}
