#include "../../include/ds/TokenList.h"
#include <iostream>

using namespace tokenizer::ds;

TokenList::TokenList() {
    header = new Node();
    trailer = new Node();


    header->next = trailer;
    header->prev = nullptr;
    trailer->prev = header;
    trailer->next = nullptr;
}

TokenList::~TokenList() {
    Node* current = header;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

bool TokenList::empty() const {
    return header->next == trailer;
}

TokenList::listPos_t TokenList::frontPos() {
    return header->next;
}

TokenList::listPos_t TokenList::backPos() {
    return trailer->prev;
}

void TokenList::addBack(token_t elem) {
    Node* u = new Node();
    u->token = elem;
    Node* v = trailer->prev;
    v->next = u;
    u->prev = v;
    u->next = trailer;
    trailer->prev = u; 
}

void TokenList::printList() const {
    if (!header || !header->next) return;
    Node* current = header->next;
    while (current != trailer) {
        std::cout << current->token << " ";
        current = current->next;
    }
    std::cout << std::endl;
}