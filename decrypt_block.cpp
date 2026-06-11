#include "Header.h"
#include <stdexcept>

void decrypt_block(uint8_t block[16]) {
    uint64_t R = load64(block), L = load64(block + 8);
    R = R ^ kw[2]; L = L ^ kw[3];
 
#define round_keys(r, l, ki) { uint64_t _t = (r); (r) = (l); (l) = _t ^ F((l), (ki)); }
 
    if (key_lenght == 128) {
        round_keys(R, L, k[17]); round_keys(R, L, k[16]); round_keys(R, L, k[15]);
        round_keys(R, L, k[14]); round_keys(R, L, k[13]); round_keys(R, L, k[12]);
        L = FLIN(L, kl[2]); R = FL(R, kl[3]);

        round_keys(R, L, k[11]); round_keys(R, L, k[10]); round_keys(R, L, k[9]);
        round_keys(R, L, k[8]); round_keys(R, L, k[7]); round_keys(R, L, k[6]);
        L = FLIN(L, kl[0]); R = FL(R, kl[1]);

        round_keys(R, L, k[5]); round_keys(R, L, k[4]); round_keys(R, L, k[3]);
        round_keys(R, L, k[2]); round_keys(R, L, k[1]); round_keys(R, L, k[0]);
    } else {
        round_keys(R, L, k[23]); round_keys(R, L, k[22]); round_keys(R, L, k[21]);
        round_keys(R, L, k[20]); round_keys(R, L, k[19]); round_keys(R, L, k[18]);
        L = FLIN(L, kl[4]); R = FL(R, kl[5]);

        round_keys(R, L, k[17]); round_keys(R, L, k[16]); round_keys(R, L, k[15]);
        round_keys(R, L, k[14]); round_keys(R, L, k[13]); round_keys(R, L, k[12]);
        L = FLIN(L, kl[2]); R = FL(R, kl[3]);

        round_keys(R, L, k[11]); round_keys(R, L, k[10]); round_keys(R, L, k[9]);
        round_keys(R, L, k[8]); round_keys(R, L, k[7]); round_keys(R, L, k[6]);
        L = FLIN(L, kl[0]); R = FL(R, kl[1]);

        round_keys(R, L, k[5]); round_keys(R, L, k[4]); round_keys(R, L, k[3]);
        round_keys(R, L, k[2]); round_keys(R, L, k[1]); round_keys(R, L, k[0]);
    }
#undef round_keys
 
    L = L ^ kw[0]; R = R ^ kw[1];
    store64(block, L); store64(block + 8, R);
}

std::vector<uint8_t> decrypt_data(const std::vector<uint8_t>& ciphertext) {
    if (ciphertext.size() % block_size != 0) {
        throw std::invalid_argument("Ciphertext size must be a multiple of block size");
    }
    std::vector<uint8_t> out_data = ciphertext;
    for (size_t i = 0; i < out_data.size(); i += block_size) decrypt_block(out_data.data() + i);
    return out_data;
}
 