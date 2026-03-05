#include <vector>
#include <string>
#include <pair>
#include <unordered_map>

class BPE {
private:
	std::unordered_map<std::string, int> vocab; 
	std::vector<std::pair<std::string, std::string>> mergeSequence;
public:
	BPE();
	void bpe_train(std::string text, int nMerges);
   	std::string decode(std::vector<int> tokenIds);
	std::vector<int> encode(std::string text); 	
}
