#include "../include/Tokenizer.h"
#include <iostream>
#include <vector>

using namespace tokenizer;

int main() {
    Tokenizer tokenizer1;
    Tokenizer tokenizer2;
    std::string text = "aaabdaaabac";
    std::string random_text = "7mK9pL2vR5nW1xZ8bQ4tY6cJ3fH0sM1gN9vK2rL5pW8xZ3bQ7tY4cJ6fH1sM0gN2vK9rL5pW8xZ3bQ7tY4cJ6fH1sM0gN2vK9r";
    
    tokenizer1.bpeTrain(text, 3);
    std::vector<token_t> encoded1 = tokenizer1.encode(text);
    std::string decoded1 = tokenizer1.decode(encoded1);
    tokenizer2.bpeTrain(random_text, 3);
    std::vector<token_t> encoded2 = tokenizer2.encode(random_text);
    std::string decoded2 = tokenizer2.decode(encoded2);

    std::cout << "Tokenizer1 input: " << text << std::endl;
    std::cout << "Encoded: ";
    for (token_t tok : encoded1) {
        std::cout << tok << " ";
    }
    std::cout << std::endl;
    std::cout << "Decoded: " << decoded1 << std::endl;
    std::cout << "Merge sequence : ";
    tokenizer1.printMergeSeq();

    std::cout << "Tokenizer2 input: " << random_text << std::endl;
    std::cout << "Encoded: ";
    for (token_t tok : encoded2) {
        std::cout << tok << " ";
    }
    std::cout << std::endl;
    std::cout << "Decoded: " << decoded2 << std::endl;
    std::cout << "Merge sequence : ";
    tokenizer2.printMergeSeq();

    return 0;
}
