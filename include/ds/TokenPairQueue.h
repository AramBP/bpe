#ifndef __TOKENIZER__DS__TOKENPAIRQUEUE_H
#define __TOKENIZER__DS__TOKENPAIRQUEUE_H

#include "TokenList.h"

#include <vector>

using listPos_t = tokenizer::ds::TokenList::listPos_t;


namespace tokenizer {
    namespace ds {
        struct TokenPair {
            mutable std::vector<listPos_t> positions; // Holds pointers to the positions of this pair in the TokenList
            mutable int count; // The number of occurences of this pair in the TokenList
            token_t first, second; 

            bool operator<(const TokenPair& rhs) const;
            bool operator==(const TokenPair& other) const;
        };

        void initTokenPair(TokenPair& pair, listPos_t position);

        struct TokenPairHash {
            bool operator()(const TokenPair& tokenPair) const;
        };

        class TokenPairQueue {
        private:
            std::vector<TokenPair> queue;
        public:
            void sort();
            void pop();
            TokenPair top() const;

            void pairs(TokenList& tokens); // Empty the queue and fill it with TokenPairs from the TokenList
            void addPosition(listPos_t position); 
            void removePosition(listPos_t position);
            void printQueue() const;
        };
    }
}

#endif