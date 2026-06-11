#include "Header.h"
#include <stdexcept>

void decrypt_block(uint8_t block[16]) {
    uint64_t R = load64(block) ^ kw[2]; 
    uint64_t L = load64(block + 8) ^ kw[3];
 
    if (key_lenght == 128) {
        L = L ^ F(R, k[17]); 
        R = R ^ F(L, k[16]);
        L = L ^ F(R, k[15]); 
        R = R ^ F(L, k[14]);
        L = L ^ F(R, k[13]); 
        R = R ^ F(L, k[12]);

        L = FLIN(L, kl[2]); 
        R = FL(R, kl[3]);

        L = L ^ F(R, k[11]); 
        R = R ^ F(L, k[10]);
        L = L ^ F(R, k[9]);  
        R = R ^ F(L, k[8]);
        L = L ^ F(R, k[7]);  
        R = R ^ F(L, k[6]);

        L = FLIN(L, kl[0]); 
        R = FL(R, kl[1]);

        L = L ^ F(R, k[5]); 
        R = R ^ F(L, k[4]);
        L = L ^ F(R, k[3]); 
        R = R ^ F(L, k[2]);
        L = L ^ F(R, k[1]); 
        R = R ^ F(L, k[0]);
    } else {
        L = L ^ F(R, k[23]); 
        R = R ^ F(L, k[22]);
        L = L ^ F(R, k[21]); 
        R = R ^ F(L, k[20]);
        L = L ^ F(R, k[19]); 
        R = R ^ F(L, k[18]);

        L = FLIN(L, kl[4]); 
        R = FL(R, kl[5]);

        L = L ^ F(R, k[17]); 
        R = R ^ F(L, k[16]);
        L = L ^ F(R, k[15]); 
        R = R ^ F(L, k[14]);
        L = L ^ F(R, k[13]); 
        R = R ^ F(L, k[12]);

        L = FLIN(L, kl[2]); 
        R = FL(R, kl[3]);

        L = L ^ F(R, k[11]); 
        R = R ^ F(L, k[10]);
        L = L ^ F(R, k[9]); 
        R = R ^ F(L, k[8]);
        L = L ^ F(R, k[7]);  
        R = R ^ F(L, k[6]);

        L = FLIN(L, kl[0]); 
        R = FL(R, kl[1]);

        L = L ^ F(R, k[5]); 
        R = R ^ F(L, k[4]);
        L = L ^ F(R, k[3]); 
        R = R ^ F(L, k[2]);
        L = L ^ F(R, k[1]); 
        R = R ^ F(L, k[0]);
    }
 
    uint64_t out_L = L ^ kw[0];
    uint64_t out_R = R ^ kw[1];
    store64(block, out_L); 
    store64(block + 8, out_R);
}

std::vector<uint8_t> decrypt_data(const std::vector<uint8_t>& ciphertext) {
    if (ciphertext.size() % block_size != 0) {
        throw std::invalid_argument("Ciphertext size must be a multiple of block size");
    }
    std::vector<uint8_t> out_data = ciphertext;
    for (size_t i = 0; i < out_data.size(); i += block_size) decrypt_block(out_data.data() + i);
    return out_data;
}