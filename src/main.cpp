#include "../include/Tokenizer.h"
using namespace tokenizer;

int main() {
    Tokenizer tokenizer1;
    Tokenizer tokenizer2;
    std::string text = "aaabdaaabac";
    std::string random_text = "7mK9pL2vR5nW1xZ8bQ4tY6cJ3fH0sM1gN9vK2rL5pW8xZ3bQ7tY4cJ6fH1sM0gN2vK9rL5pW8xZ3bQ7tY4cJ6fH1sM0gN2vK9r";
    tokenizer1.bpeTrain(text, 3);
    tokenizer2.bpeTrain(random_text, 3);
    return 0;
}
