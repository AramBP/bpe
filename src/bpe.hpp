#include <vector>
#include <string>
#include <pair>
#include <map>

using byte_t = unsigned char;
using tokenId_t = int;

class BPE {
private:
	std::map<tokenId_t, byte_t> vocab; // Maps a token id to a utf-8 byte
	std::vector<std::pair<tokenId_t, tokenId_t>> mergeSequence;
	void bpe_train(const std::string& text, int nMerges);
public:
	BPE();
   	std::string decode(std::vector<tokenId_t> tokens);
	std::vector<tokenId_t> encode(std::string text);
};
