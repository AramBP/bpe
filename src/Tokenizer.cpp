#include "../include/Tokenizer.h"
#include "../include/ds/TokenList.h"
#include "../include/ds/TokenPairQueue.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>
#include <regex>

using namespace tokenizer;
using namespace tokenizer::ds;

Tokenizer::Tokenizer(string_t text, int nMerges) : regexSplitter(R"('s|'t|'re|'ve|'m|'ll|'d| ?[a-zA-Z]+| ?[0-9]+| ?[^\s\w]+|\s+(?!\S)|\s+)") {
    for (int i = 0; i < 256; ++i) {
        vocab.insert({i, string_t(1, static_cast<unsigned char>(i))});
    }

    bpeTrain(text, nMerges);
}

void Tokenizer::bpeTrain(string_t text, int nMerges) {
    TokenList tokens;
    TokenPairQueue queue;

    // Initialize list as sequence of tokens of chars
    std::sregex_iterator iter(text.begin(), text.end(), regexSplitter);
    std::sregex_iterator end_iter;
    while (iter != end_iter) {
        for (unsigned char c : iter->str()) {
            tokens.addBack(static_cast<int>(c));
        }
        ++iter;
    }
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
            if (first != tokens.frontPos())
                queue.removePosition(first->prev);   
            if (second != tokens.backPos())
                queue.removePosition(second->next);

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
        nMerges--;
    }
}

string_t Tokenizer::decode(std::vector<token_t> tokens) {
    string_t s = "";
    for (token_t tok : tokens) {
        s += vocab.at(tok);
    }
    return s;
}

std::vector<token_t> Tokenizer::encode(string_t text) {
    std::vector<token_t> tokens;
    for (unsigned char c : text) {
        tokens.push_back(static_cast<int>(c));
    }
    
    for (auto it = tokens.begin(); it != std::prev(tokens.end());) {
        token_t first = *it;
        token_t second = *std::next(it);

        if (auto search = std::find(mergeSequence.begin(), mergeSequence.end(), std::make_pair(first, second)); search != mergeSequence.end()) {
            *it = 256 + std::distance(mergeSequence.begin(), search);
            tokens.erase(std::next(it));
            it = tokens.begin();
        } else {
            ++it;
        }
    }
    return tokens;
}

void Tokenizer::printMergeSeq() const {
    for (const auto& [first, second] : mergeSequence) {
        std::cout << "(" << vocab.at(first) << ", " << vocab.at(second) << ") "; 
    }
    std::cout << std::endl;
}