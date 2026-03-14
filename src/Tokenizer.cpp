#include "../include/Tokenizer.h"
#include "../include/ds/TokenList.h"
#include "../include/ds/TokenPairQueue.h"
#include <utility>

using namespace tokenizer;
using namespace tokenizer::ds;

Tokenizer::Tokenizer() {
    for (int i = 0; i < 256; ++i) {
        vocab.insert({i, string_t(1, static_cast<unsigned char>(i))});
    }
}

void Tokenizer::bpeTrain(string_t text, int nMerges) {
    TokenList tokens;
    TokenPairQueue queue;

    // Initialize list as sequence of tokens of chars
    for (unsigned char c : text) {
        tokens.addBack(static_cast<int>(c));
    }
    tokens.printList();
    queue.pairs(tokens);
    
    while (nMerges != 0) {
        TokenPair top = queue.top();
        mergeSequence.push_back(std::make_pair(top.first, top.second));
        token_t nextTok = vocab.size();
        vocab.insert({nextTok, vocab.at(top.first) + vocab.at(top.second)});

        for (listPos_t p : top.positions) {
            if (p == nullptr || p == tokens.backPos() || p->token != top.first || p->next->token != top.second)
                continue;

            listPos_t first = p;
            listPos_t second = p->next;

            // Remove positions from the heap
            if (first != tokens.frontPos()) {
                queue.removePosition(first->prev);   }
            if (second != tokens.backPos()) {
                queue.removePosition(second->next); }
            // Update the TokenList
            first->token = nextTok;
            first->next = second->next;
            second->next->prev = first;
            delete second;

            if (first != tokens.frontPos()) 
                queue.addPosition(first->prev);
            if (first != tokens.backPos())
                queue.addPosition(first);
        }
        queue.pop();
        queue.sort();
        tokens.printList();
        nMerges--;
    }
}
