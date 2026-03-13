#include "../../include/ds/TokenPairQueue.h"
#include <algorithm>
#include <cassert>
#include <unordered_set>

using namespace tokenizer::ds;

void initTokenPair(TokenPair& pair, listPos_t position) {
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

void TokenPairQueue::pairs(TokenList tokens) {
    std::unordered_set<TokenPair, TokenPairHash> counts;
    listPos_t p = tokens.frontPos();
    for(; p != tokens.backPos()->prev; ++p) {
        // Initialize pair
        TokenPair pair;
        initTokenPair(pair, p); 
        // Check if this pair already exist (but at a previous position in the list)
        if (auto search = std::find(counts.begin(), counts.end(), pair); search != counts.end()) {
            search->positions.push_back(p);
            search->count++;
        } else {
            counts.insert(pair);
        }
    }
    std::copy(counts.begin(), counts.end(), std::inserter(queue, queue.end()));
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
    assert(search_pair != queue.end() && "Pair position to delete not found in queue");
    auto search_pos = std::find(search_pair->positions.begin(), search_pair->positions.end(), position);
    assert(search_pos != search_pair->positions.end() && "Position to delete not found in positions array");

    search_pair->positions.erase(search_pos);
    search_pair->count--;
}