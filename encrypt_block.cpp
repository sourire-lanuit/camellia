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

} //судячи по методє для 128 бітов

std::vector<uint8_t> encrypt_data(std::vector<uint8_t> data, Key& roundkey){
    while(data.size() % 16) data.push_back(0);
    for(size_t i = 0; i < data.size(); i += 16) encrypt_block(&data[i], roundkey);
    return data;
}