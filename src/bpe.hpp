#include <vector>
#include <string>
#include <pair>
#include <map>

using std::map;
using std::string;
using std::vector;
using std::pair;

using byte_t = unsigned char;
using tokenId_t = int;

class BPE {
private:
	map<tokenId_t, byte_t> vocab; // Maps a token id to a utf-8 byte
	vector<pair<tokenId_t, tokenId_t>> mergeSequence;
	void bpe_train(const string& text, int nMerges);
public:
	BPE();
   	string decode(vector<tokenId_t> tokens);
	vector<tokenId_t> encode(string text);
};
