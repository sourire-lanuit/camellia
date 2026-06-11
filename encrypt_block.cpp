#include "Header.h"

void encrypt_block(uint8_t block[16]) {
    uint64_t L = load64(block), R = load64(block+8);
    L = L ^ kw[0]; R = R ^ kw[1];
 
#define round_key(l, r, ki) { uint64_t _t = (l); (l) = (r); (r) = _t ^ F((r), (ki)); }
 
    if (key_lenght == 128) {
        round_key(L, R, k[0]); round_key(L, R, k[1]); round_key(L, R, k[2]);
        round_key(L, R, k[3]); round_key(L, R, k[4]); round_key(L, R, k[5]);

        L = FL(L, kl[0]); R = FLIN(R, kl[1]);

        round_key(L, R, k[6]); round_key(L, R, k[7]); round_key(L, R, k[8]);
        round_key(L ,R, k[9]); round_key(L, R, k[10]); round_key(L, R, k[11]);

        L = FL(L, kl[2]); R = FLIN(R, kl[3]);

        round_key(L, R, k[12]); round_key(L, R, k[13]); round_key(L, R, k[14]);
        round_key(L, R, k[15]); round_key(L, R, k[16]); round_key(L, R, k[17]);
    } else {
        round_key(L, R, k[0]); round_key(L, R, k[1]); round_key(L, R, k[2]);
        round_key(L, R, k[3]); round_key(L, R, k[4]); round_key(L, R, k[5]);

        L = FL(L, kl[0]); R = FLIN(R, kl[1]);

        round_key(L, R, k[6]); round_key(L, R, k[7]); round_key(L, R, k[8]);
        round_key(L, R, k[9]); round_key(L, R, k[10]); round_key(L, R, k[11]);

        L = FL(L, kl[2]); R = FLIN(R, kl[3]);

        round_key(L, R, k[12]); round_key(L, R, k[13]); round_key(L, R, k[14]);
        round_key(L, R, k[15]); round_key(L, R, k[16]); round_key(L, R, k[17]);

        L = FL(L, kl[4]); R = FLIN(R, kl[5]);

        round_key(L, R, k[18]); round_key(L, R, k[19]); round_key(L, R, k[20]);
        round_key(L, R, k[21]); round_key(L, R, k[22]); round_key(L, R, k[23]);
    }

#undef round_key

    R = R ^ kw[2]; L = L ^ kw[3];
    store64(block, R); store64(block + 8, L);
}

std::vector<uint8_t> encrypt_data(const std::vector<uint8_t>& text) {
    std::vector<uint8_t> out_data = text; 
    while (out_data.size() % block_size) out_data.push_back(0);
    for (size_t i = 0; i < out_data.size(); i += block_size) encrypt_block(out_data.data()+i);
    return out_data;
}