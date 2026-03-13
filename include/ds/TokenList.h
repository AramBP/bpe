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
            Node* tailer;
        public:
            using listPos_t = Node*;
            bool empty() const;
            const Node& front();
            const Node& back();
            void addBack(token_t elem);

            void fromString(const string_t& s);
        };
    }
}

#endif