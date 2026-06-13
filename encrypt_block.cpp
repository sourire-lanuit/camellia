#include "Header.h"

void encrypt_block(uint8_t block[16]) {
    uint64_t L = load64(block), R = load64(block+8);
    L = L ^ kw[0]; R = R ^ kw[1]; 

    if (key_lenght == 128) {
        R = R ^ F(L, k[0]); 
        L = L ^ F(R, k[1]);
        R = R ^ F(L, k[2]); 
        L = L ^ F(R, k[3]);
        R = R ^ F(L, k[4]); 
        L = L ^ F(R, k[5]);

        L = FL(L, kl[0]); 
        R = FLIN(R, kl[1]);

        R = R ^ F(L, k[6]); 
        L = L ^ F(R, k[7]);
        R = R ^ F(L, k[8]); 
        L = L ^ F(R, k[9]);
        R = R ^ F(L, k[10]); 
        L = L ^ F(R, k[11]);

        L = FL(L, kl[2]); 
        R = FLIN(R, kl[3]);

        R = R ^ F(L, k[12]); 
        L = L ^ F(R, k[13]);
        R = R ^ F(L, k[14]); 
        L = L ^ F(R, k[15]);
        R = R ^ F(L, k[16]); 
        L = L ^ F(R, k[17]);
    } else {
        R = R ^ F(L, k[0]); 
        L = L ^ F(R, k[1]);
        R = R ^ F(L, k[2]); 
        L = L ^ F(R, k[3]);
        R = R ^ F(L, k[4]); 
        L = L ^ F(R, k[5]);

        L = FL(L, kl[0]); 
        R = FLIN(R, kl[1]);

        R = R ^ F(L, k[6]); 
        L = L ^ F(R, k[7]);
        R = R ^ F(L, k[8]); 
        L = L ^ F(R, k[9]);
        R = R ^ F(L, k[10]); 
        L = L ^ F(R, k[11]);

        L = FL(L, kl[2]); 
        R = FLIN(R, kl[3]);

        R = R ^ F(L, k[12]); 
        L = L ^ F(R, k[13]);
        R = R ^ F(L, k[14]); 
        L = L ^ F(R, k[15]);
        R = R ^ F(L, k[16]); 
        L = L ^ F(R, k[17]);

        L = FL(L, kl[4]); 
        R = FLIN(R, kl[5]);

        R = R ^ F(L, k[18]); 
        L = L ^ F(R, k[19]);
        R = R ^ F(L, k[20]); 
        L = L ^ F(R, k[21]);
        R = R ^ F(L, k[22]); 
        L = L ^ F(R, k[23]);
    }

    uint64_t out_L = R ^ kw[2];
    uint64_t out_R = L ^ kw[3];
    store64(block, out_L); 
    store64(block + 8, out_R);
}

std::vector<uint8_t> encrypt_data(const std::vector<uint8_t>& text) {
    std::vector<uint8_t> out_data = text; 
    while (out_data.size() % block_size) out_data.push_back(0);
    for (size_t i = 0; i < out_data.size(); i += block_size) encrypt_block(out_data.data()+i);
    return out_data;
}