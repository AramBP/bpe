#include "TokenPair.h"

TokenPair::TokenPair(listPos_t x, listPos_t y) : count{1}, first {*x}, second {*y} {
	positions.push_back(std::make_pair(x, y));
}

tokenId_t TokenPair::getFirst() const { return first; }
tokenId_t TokenPair::getSecond() const { return second; }
int TokenPair::getCount() const { return count; }
std::vector<std::pair<listPos_t, listPos_t>>& TokenPair::getPositions() { return positions; }

void TokenPair::newPositions(listPos_t x, listPos_t y) const {
	positions.push_back(std::make_pair(x, y));
	count++;
}

void TokenPair::deletePositions(listPos_t x, listPos_t y) const {
	for (auto it = positions.begin(); it != positions.end();) {
		auto [first, second] = *it;
		if (first == x && second == y) {
			it = positions.erase(it);
			count--;
		} else {
			++it;
		}
	}
}

void TokenPair::print() {
	std::cout << first << " " << second << " : " << count << std::endl;
}

bool TokenPair::operator==(const TokenPair& other) const {
	return (first == other.getFirst()) && (second == other.getSecond());
}

bool TokenPair::operator<(const TokenPair& rhs) const {
	if (count != rhs.getCount())
		return count < rhs.getCount();
	if (first != rhs.getFirst())
		return first < rhs.getFirst();
	return second < rhs.getSecond();
}

std::size_t TokenPairHash::operator()(const TokenPair& bp) const {
	std::size_t h1 = std::hash<tokenId_t>()(bp.getFirst());
	std::size_t h2 = std::hash<tokenId_t>()(bp.getSecond());
	return h1^(h2 << 1);
}

TokenPairHeap::TokenPairHeap(std::list<tokenId_t>& tokens) {
	std::unordered_set<TokenPair, TokenPairHash> counts;
	for (auto it = tokens.begin(); it != std::prev(tokens.end()); ++it) {
		TokenPair pair(it, std::next(it));
		if (auto search = counts.find(pair); search != counts.end())
			search->newPositions(it, std::next(it));
		else
			counts.insert(pair);
	}
	std::copy(counts.begin(), counts.end(), std::inserter(heap, heap.begin()));
	sort();
}

void TokenPairHeap::sort() {
	std::sort(heap.begin(), heap.end());
}

TokenPair TokenPairHeap::top() {
	return heap.back(); 
}

void TokenPairHeap::pop() {
	heap.pop_back();
	sort();
}

void TokenPairHeap::addPosition(listPos_t x, listPos_t y) {
	auto it = heap.begin();
	for (; it != heap.end(); ++it) {
		if (it->getFirst() == *x && it->getSecond() == *y) {
			it->newPositions(x, y);
			break;
		}
	}
	if (it == heap.end()) {
		TokenPair pair(x, y);
		heap.push_back(pair);
	}
}

std::pair<listPos_t, listPos_t> TokenPairHeap::removePosition(listPos_t x, listPos_t y) {
	for (auto it = heap.begin(); it != heap.end();) {
		if(it->getFirst() == *x && it->getSecond() == *y) {
			it->deletePositions(x, y);
			if (it->getCount() == 0)
				heap.erase(it);
			break;
		} else {
			++it;
		}
	}
	return std::make_pair(x, y);
}

void TokenPairHeap::print() {
	for (auto elem : heap) {
		elem.print();
	}
}
