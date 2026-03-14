#ifndef __TOKENIZER__DS__TOKENLIST_H
#define __TOKENIZER__DS__TOKENLIST_H

#include "../types.h"

namespace tokenizer {
    namespace ds {
        class TokenList {
        private:
            struct Node {
                token_t token;
                Node* next;
                Node* prev;
             };
            Node* header;
            Node* trailer;
        public:
            TokenList();
            ~TokenList();
            
            using listPos_t = Node*;
            bool empty() const;
            listPos_t frontPos();
            listPos_t backPos();
            void addBack(token_t elem);
            void printList() const;
        };
    }
}

#endif