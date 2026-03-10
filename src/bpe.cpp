#include "bpe.h"
#include "byte_pair.h"

#include <queue>
#include <unordered_set>
#include <list>

namespace {
	std::unordered_set<BytePair, BytePairHash> countPairs (const std::list<byte_t>& l) {
		std::unordered_set<BytePair, BytePairHash> counts;
		for (auto it; it != std::prev(l.end()); ++it) {
			BytePair pair (it, std::next(it));
			if (auto search = ret.find(pair); search != ret.end()) {
				search->addPosition(it, std::next(it));
			} else {
				ret.insert(pair);
			}
		}
		return ret;
	}
}

BPE::bpe_train(string text, int nMerges) {
	
}

BPE::BPE() {
	for (int i = 0; i < 256; ++i) {
		vocab.insert({i, static_cast<byte_t>(i)});
	}
}


