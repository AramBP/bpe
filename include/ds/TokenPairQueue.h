#ifndef __TOKENIZER__DS__TOKENPAIRQUEUE_H
#define __TOKENIZER__DS__TOKENPAIRQUEUE_H

#include "TokenList.h"

#include <vector>

using listPos_t = tokenizer::ds::TokenList::listPos_t;


namespace tokenizer {
    namespace ds {
        /**
        * Represents a unique pair of adjacent tokens and tracks their occurences and positions in the TokenList.
        * The 'mutable' keywords allow these fields to be modified even when the object is stores in a const-sensitive container.
        */
        struct TokenPair {
            // List of pointers to the first token of this pair within the TokenList.
            mutable std::vector<listPos_t> positions;

            // Total number of times this specific pair appears.
            mutable int count; 
            
            token_t first, second; // The two tokens forming the pair
            
            // Comparison operator for sorting. 
            // Prioritzes higher count and distinguishes pairs with the same count but different tokens
            bool operator<(const TokenPair& rhs) const;

            // Comparison operator for equivalence.
            // Two pairs are equal if and only if their first and second fields are equal.
            bool operator==(const TokenPair& other) const;
        };
        
        // Helper to initialize a TokenPair with its first found occurrence in the list
        void initTokenPair(TokenPair& pair, listPos_t position);

        /**
        * Functor for calculating the hash value of the TokenPair. Only the fields first and second are used to determine the hash value.
        */
        struct TokenPairHash {
            bool operator()(const TokenPair& tokenPair) const;
        };
        
        /**
        * Manages a collection of TokenPairs, providing sorting and retrieval capabilities.
        * Behaves like a managed vector that can be sorted to act as a priority structure for processing the most frequent token pairs.
        */
        class TokenPairQueue {
        private:
            // Internal storage for all tracked pairs.
            std::vector<TokenPair> queue;
        public:
            // Sorts the pairs based on frequencey s.t. the most common pair is at the top.
            void sort();

            // Remove the most frequent pair from the collection.
            void pop();

            // Returns the most frequent TokenPair without removing it.
            TokenPair top() const;

            // Scans a TokenList to identify all unique adjacent pairs and populate the queue.
            void pairs(TokenList& tokens); 

            // Registers a new occurence of a pair at a specific list position.
            void addPosition(listPos_t position); 

            // Removes a tracked occurence.
            void removePosition(listPos_t position);

            void printQueue() const;
        };
    }
}

#endif