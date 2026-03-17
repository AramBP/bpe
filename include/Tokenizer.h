#ifndef __TOKENIZER_H
#define __TOKENIZER_H

#include "types.h"

#include <map>
#include <vector>
#include <utility>
#include <regex>

namespace tokenizer {
    /** 
    * Main class for training and applying tokenization.
    * This class manages the vocabulary mapping and the learned sequence of merge operations.
    */
    class Tokenizer {
    private:
        // Maps interger token IDs to their corresponding string representations.
        std::map<token_t, string_t> vocab;

        // An ordered collection of which token pairs were merged during training.
        // Each pair in the vector represents a single BPE merge rule.
        std::vector<std::pair<token_t, token_t>> mergeSequence;

        // Regex pattern for tokenization, performs initial splitting of the input string
        std::regex regexSplitter;
        
        // Learns sub-word units by iteratively merging the most frequent token pairs.
        void bpeTrain(string_t text, int nMerges);
    public:
        // Initializes a tokenizer using a training string, starting with individual characters.
        Tokenizer(string_t text, int nMerges); 

        // Converts a sequence of token IDs back to a human readable string
        string_t decode(std::vector<token_t> tokens);

        // Converts a readable string to a sequence of learned token IDs.
        std::vector<token_t> encode(string_t text);

        // Debug utility to display the chronological order of learned merge rules.
        void printMergeSeq() const;
    };
}

#endif