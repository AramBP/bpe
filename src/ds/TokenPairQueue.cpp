#include "../../include/ds/TokenPairQueue.h"
#include <algorithm>
#include <cassert>
#include <unordered_set>
#include <iostream>

using namespace tokenizer::ds;

void tokenizer::ds::initTokenPair(TokenPair& pair, listPos_t position) {
    pair.first = position->token;
    pair.second = position->next->token;
    pair.positions.push_back(position);
    pair.count = 1;
}

bool TokenPair::operator<(const TokenPair& rhs) const {
    if (count != rhs.count)
        return count > rhs.count;
    if (first != rhs.first)
        return first > rhs.first;
    return second > rhs.second;
}

bool TokenPair::operator==(const TokenPair& other) const {
    return first == other.first && second == other.second;
}

bool TokenPairHash::operator()(const TokenPair& tokenPair) const {
    std::size_t h1 = std::hash<token_t>()(tokenPair.first);
    std::size_t h2 = std::hash<token_t>()(tokenPair.second);
    return h1^(h2 << 1);
}

void TokenPairQueue::sort() {
    std::sort(queue.begin(), queue.end());
}

void TokenPairQueue::pop() {
    queue.erase(queue.begin());
}

TokenPair TokenPairQueue::top() const {
    return *queue.begin();
}

void TokenPairQueue::pairs(TokenList& tokens) {
    std::unordered_set<TokenPair, TokenPairHash> counts;
    listPos_t p = tokens.frontPos();
    while(p != tokens.backPos()) {
        TokenPair pair;
        initTokenPair(pair, p); 
        if(auto search = counts.find(pair); search != counts.end()) {
            search->positions.push_back(p);
            search->count++;
        } else {
            counts.insert(pair);
        }
        p = p->next;
    }

    queue.clear();
    for (auto& entry: counts) {
        queue.push_back(entry);
    }
    sort();
}

void TokenPairQueue::addPosition(listPos_t position) {
    TokenPair pair;
    initTokenPair(pair, position);
    if (auto search = std::find(queue.begin(), queue.end(), pair); search != queue.end()) {
        search->positions.push_back(position);
        search->count++;
    } else {
        queue.push_back(pair);
    }
}

void TokenPairQueue::removePosition(listPos_t position) {
    TokenPair pair;
    initTokenPair(pair, position);

    auto search_pair = std::find(queue.begin(), queue.end(), pair);
    auto search_pos = std::find(search_pair->positions.begin(), search_pair->positions.end(), position);
    search_pair->positions.erase(search_pos);
    search_pair->count--;
    if (search_pair->count == 0) {
        queue.erase(search_pair);
    }
}

void TokenPairQueue::printQueue() const {
    for (const TokenPair& e : queue) {
        std::cout << "(" << e.first << ", " << e.second << ")" << " : " << e.count << " ";
    }
    std::cout << std::endl;
}