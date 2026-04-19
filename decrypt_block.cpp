#include "Header.h"

void decrypt_block(uint8_t* b, Key& roundkey) {
    uint64_t R =* (uint64_t*)b, L =* (uint64_t*)(b + 8);
    R ^= roundkey.kw[3]; L ^= roundkey.kw[4];

    if(i = 13 || i = 7) {
        uint64_t t = R;
        R = FL(R, roundkey.k[i]) ^ L;
        L = t;
    }
}

std::vector<uint8_t> decrypt_data(std::vector<uint8_t> data, Key& roundkey){
    for(size_t i = 0; i < data.size(); i += 16) decrypt_block(&data[i], roundkey);
    return data;
}