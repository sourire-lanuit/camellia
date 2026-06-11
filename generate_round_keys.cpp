#include "Header.h"

void generate_keys(const std::vector<uint8_t>& key) {
    uint64_t KLL, KLR, KRL = 0, KRR = 0;
    
    KLL = load64(key.data());
    KLR = load64(key.data() + 8);
    if (key_lenght == 192) {
        KRL = load64(key.data() + 16);
        KRR = ~KRL;
    } else if (key_lenght == 256) {
        KRL = load64(key.data() + 16);
        KRR = load64(key.data() + 24);
    }

    uint64_t KAL = KLL ^ KRL, KAR = KLR ^ KRR;
    KAR = KAR ^ F(KAL, sigma[0]); 
    KAL = KAL ^ F(KAR, sigma[1]); 
    KAL = KAL ^ KLL; 
    KAR = KAR ^ KLR;
    KAR = KAR ^ F(KAL, sigma[2]); 
    KAL = KAL ^ F(KAR, sigma[3]); 

    uint64_t KBL = 0, KBR = 0;
    if (key_lenght != 128) {
        KBL = KAL ^ KRL; 
        KBR = KAR ^ KRR;
        KBR = KBR ^ F(KBL, sigma[4]);
        KBL = KBL ^ F(KBR, sigma[5]); 
    } 

     if (key_lenght == 128) {
        kw[0] = high128(KLL, KLR, 0); 
        kw[1] = low128(KLL, KLR, 0);
        k[0] = high128(KAL, KAR, 0); 
        k[1] = low128(KAL, KAR, 0);
        k[2] = high128(KLL, KLR, 15); 
        k[3] = low128(KLL, KLR, 15);
        k[4] = high128(KAL, KAR, 15); 
        k[5] = low128(KAL, KAR, 15);
        kl[0] = high128(KAL, KAR, 30); 
        kl[1] = low128(KAL, KAR, 30);
        k[6] = high128(KLL, KLR, 45); 
        k[7] = low128(KLL, KLR, 45);
        k[8] = high128(KAL, KAR, 45); 
        k[9] = low128(KAL, KAR, 45); 
        kl[2] = high128(KLL, KLR, 60); 
        kl[3] = low128(KLL, KLR, 60); 
        k[10] = high128(KAL, KAR, 60); 
        k[11] = low128(KAL, KAR, 60);
        k[12] = high128(KLL, KLR, 77); 
        k[13] = low128(KLL, KLR, 77); 
        k[14] = high128(KLL, KLR, 94); 
        k[15] = low128(KLL, KLR, 94); 
        k[16] = high128(KAL, KAR, 94); 
        k[17] = low128(KAL, KAR, 94); 
        kw[2] = high128(KAL, KAR, 111); 
        kw[3] = low128(KAL, KAR, 111);
    }

    else {
        kw[0] = high128(KLL, KLR, 0); 
        kw[1] = low128(KLL, KLR, 0);
        k[0] = high128(KBL, KBR, 0); 
        k[1] = low128(KBL, KBR, 0);
        k[2] = high128(KRL, KRR, 15); 
        k[3] = low128(KRL, KRR, 15);
        k[4] = high128(KAL, KAR, 15); 
        k[5] = low128(KAL, KAR, 15);
        kl[0] = high128(KRL, KRR, 30);
        kl[1] = low128(KRL, KRR, 30);
        k[6] = high128(KBL, KBR, 30); 
        k[7] = low128(KBL, KBR, 30);
        k[8] = high128(KLL, KLR, 45); 
        k[9] = low128(KLL, KLR, 45);
        k[10] = high128(KAL, KAR, 45); 
        k[11] = low128(KAL, KAR, 45);
        kl[2] = high128(KLL, KLR, 60); 
        kl[3] = low128(KLL, KLR, 60); 
        k[12] = high128(KRL, KRR, 60); 
        k[13] = low128(KRL, KRR, 60);
        k[14] = high128(KBL, KBR, 60); 
        k[15] = low128(KBL, KBR, 60);
        k[16] = high128(KLL, KLR, 77); 
        k[17] = low128(KLL, KLR, 77); 
        kl[4] = high128(KAL, KAR, 77); 
        kl[5] = low128(KAL, KAR, 77);
        k[18] = high128(KRL, KRR, 94); 
        k[19] = low128(KRL, KRR, 94); 
        k[20] = high128(KAL, KAR, 94); 
        k[21] = low128(KAL, KAR, 94); 
        k[22] = high128(KLL, KLR, 111); 
        k[23] = low128(KLL, KLR, 111); 
        kw[2] = high128(KBL, KBR, 111); 
        kw[3] = low128(KBL, KBR, 111);
    }
}

void cipher_use(const std::vector<uint8_t>& key) {
    if (key.size() == 16) key_lenght = 128;
    else if (key.size() == 24) key_lenght = 192;
    else if (key.size() == 32) key_lenght = 256;
    init_sboxes();
    generate_keys(key);
}