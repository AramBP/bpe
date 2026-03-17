#ifndef __TOKENIZER__DS__TOKENLIST_H
#define __TOKENIZER__DS__TOKENLIST_H

#include "../types.h"

namespace tokenizer {
    namespace ds {
        /**
        * A doubly linked list implementation specifically designed to store and manage 
        * token_t elements. 
        */
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

            // Returns a position pointer to the first actual data element.
            listPos_t frontPos(); 

            // Returns a position pointer to the last actual data elements.
            listPos_t backPos(); 

            // Appends a new token_t element to the end of the list
            void addBack(token_t elem);

            void printList() const;
        };
    }
}

#endif