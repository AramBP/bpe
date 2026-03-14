#ifndef __TOKENIZER_H
#define __TOKENIZER_H

#include "types.h"

#include <map>
#include <vector>
#include <utility>

namespace tokenizer {
    class Tokenizer {
    private:
        std::map<token_t, string_t> vocab;
        std::vector<std::pair<token_t, token_t>> mergeSequence;
    public:
        Tokenizer();
        void bpeTrain(string_t text, int nMerges);
        string_t decode(std::vector<token_t> tokens);
        std::vector<token_t> encode(string_t text);
    };
}

#endif