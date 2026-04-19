#include "Header.h"

void encrypt_block(uint8_t* b, Key& roundkey){
    uint64_t L =* (uint64_t*)b, R =* (uint64_t*)(b + 8);
    L ^= roundkey.kw[0]; R ^= roundkey.kw[1];

    for(int i = 0; i < 18; i++){
        uint64_t t = L;
        L = F(L, roundkey.k[i]) ^ R;
        R = t;
        if(i == 5 || i == 11) { 
            L = FL(L, roundkey.kl[i/6]); R = FL(R, roundkey.kl[i/6]); 
        }
    }

} 

std::vector<uint8_t> encrypt_data(std::vector<uint8_t> text, Key& roundkey){
    while(text.size() % 16) text.push_back(0);
    for(size_t i = 0; i < text.size(); i += 16) encrypt_block(&text[i], roundkey);
    return text; // стратегічне рішення перейменувати data на text щоб не було плутанини із декриптом
}