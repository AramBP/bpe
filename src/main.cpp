#include "pch.h"
#include "Tokenizer.h"

int main() {
    Tokenizer tokenizer;
    std::string text = "aaabdaaabac";
    tokenizer.bpeTrain(text, 3);
    std::vector<tokenId_t> encoded = tokenizer.encode(text);
    std::string decoded = tokenizer.decode(encoded);
    std::cout << "Test text : " << text << std::endl
              << "Encoded: [ ";
    for (const tokenId_t& tok : encoded)
        std::cout << tok << " ";
    std::cout << " ]" << std::endl
            << "Decoded : " << decoded << std::endl;

    return 0;
}
