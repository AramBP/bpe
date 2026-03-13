#ifndef __TOKENIZER__DS__TOKENPAIRQUEUE_H
#define __TOKENIZER__DS__TOKENPAIRQUEUE_H

#include <vector>

#include "TokenList.h"

namespace tokenizer {
    namespace ds {
        struct TokenPair {
            mutable std::vector<TokenList::listPos_t> positions; // Holds pointers to the positions of this pair in the TokenList
            mutable int count; // The number of occurences of this pair in the TokenList
            const token_t first, second; 
        };

        struct TokenPairHash {
            bool operator()(const TokenPair& tokenPair1, const TokenPair& tokenPair2);
        };

        class TokenPairQueue {
        private:
            std::vector<TokenPair> queue;
            void sort();
        public:
            TokenPairQueue();
            void pop();
            TokenPair top() const;

            void pairs(const TokenList& tokens);
            void addPosition(TokenList::listPos_t position);
            void removePosition(TokenList::listPos_t position);
        };
    }
}

#endif