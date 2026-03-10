#include "bpe.h"
#include "BytePair.h"

#include <queue>
#include <unordered_set>
#include <list>

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

void BPE::bpe_train(const std::string& text, int nMerges) {
	
}

BPE::BPE() {
	for (int i = 0; i < 256; ++i) {
		vocab.insert({i, static_cast<byte_t>(i)});
	}
}


