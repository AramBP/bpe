#ifndef __TOKENIZER__BPE_H
#define __TOKENIZER__BPE_H

#include "types.h"
#include "ds/TokenList.h"
#include "ds/TokenPairQueue.h"

namespace tokenizer {
    class BPE {
    private:
        ds::TokenList list;
        ds::TokenPairQueue queue;
        friend class Tokenizer; // for access to the merge sequence and vocabulairy
    public:
        void trainString(string_t text);
    };
}

#endif