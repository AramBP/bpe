#include "bpe.hpp"

namespace {
	map<pair<tokenId_t, tokenId_t>, int> countPairs (string& word) {
		map<pair<tokenId_t, tokenId_t>, int> counts;
		string::iterator i, j;	

		for (i = word.begin(); i != word.end() - 1; ++i) {
			j = i + 1;
			pair<const tokenId_t, const tokenId_t> tokenPair(static_cast<const tokenId_t> (*i), static_cast<const tokenId_t> (*j)>;

			if (auto search = consts.find(tokenPair); search != counts.end())
				search->second++;
			else
				counts.insert({tokenPair, 1});
		}	
	}
}

BPE::bpe_train(string text, int nMerges) {
	
}

BPE::BPE() {
	for (int i = 0; i < 256; ++i) {
		vocab.insert({i, static_cast<byte_t>(i)});
	}
}


