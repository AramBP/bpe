#include "../../include/ds/TokenList.h"

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
    while(!empty()) {
        Node* u = header->next;
        Node* v = u->next;
        Node* w = u->prev;
        v->prev = w;
        w->next = v;
        delete u;
    }
    delete header;
    delete trailer;
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