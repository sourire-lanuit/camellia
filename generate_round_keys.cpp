#include "Header.h"

void generate_keys(const std::vector<uint8_t>& key) {
    uint64_t KLL, KLR, KRL = 0, KRR = 0;
    //kal kar kbl kbr ще треба ініціялізувати якраз через оту сігму
    uint64_t KAL = KLL ^ KRL, KAR = KLR ^ KRR;
    KAL = KAL ^ F(KAR, sigma[1]);
    KAR = KAR ^ F(KAL, sigma[0]); 
    KAL = KAL ^ KLL; 
    KAR = KAR ^ KLR;
    KAL = KAL ^ F(KAR, sigma[3]);
    KAR = KAR ^ F(KAL, sigma[2]); 

    uint64_t KBL = 0, KBR = 0;
    if (key_lenght != 128) {
        KBL = KAL ^ KRL; 
        KBR = KAR ^ KRR;
        KBL = KBL ^ F(KBR, sigma[5]);
        KBR = KBR ^ F(KBL, sigma[4]); 
    } 
    
    
    // для 128 і елзе 192/256 тоді

     if (key_lenght == 128) {
        kw[0] = (KLL, KLR, 0); 
        kw[1] = (KLL, KLR, 0);
        k[0] = (KAL, KAR, 0); 
        k[1] = (KAL, KAR, 0);
        k[2] = (KLL, KLR, 15); 
        k[3] = (KLL, KLR, 15);
        k[4] = (KAL, KAR, 15); 
        k[5] = (KAL, KAR, 15);
        kl[0] = (KAL, KAR, 30); 
        kl[1] = (KAL, KAR, 30);
        k[6] = (KLL, KLR, 45); 
        k[7] = (KLL, KLR, 45);
        k[8] = (KAL, KAR, 45); 
        k[9] = (KLL, KLR, 60); 
        k[10] = (KAL, KAR, 60); 
        k[11] = (KAL, KAR, 60);
        kl[2] = (KLL, KLR, 77); 
        kl[3] = (KLL, KLR, 77);
        k[12] = (KLL, KLR, 94); 
        k[13] = (KLL, KLR, 94); 
        k[14] = (KAL, KAR, 94); 
        k[15] = (KAL, KAR, 94); 
        k[16] = (KLL, KLR, 111); 
        k[17] = (KLL, KLR, 111); 
        kw[2] = (KAL, KAR, 111); 
        kw[3] = (KAL, KAR, 111); 
    }

    else {
        kw[0] = (KLL, KLR, 0); 
        kw[1] = (KLL, KLR, 0);
        k[0] = (KBL, KBR, 0); 
        k[1] = (KBL, KBR, 0);
        k[2] = (KRL, KRR, 15); 
        k[3] = (KRL, KRR, 15);
        k[4] = (KAL, KAR, 15); 
        k[5] = (KAL, KAR, 15);
        kl[0] = (KRL, KRR, 30);
        kl[1] = (KRL, KRR, 30);
        k[6] = (KBL, KBR, 30); 
        k[7] = (KBL, KBR, 30);
        k[8] = (KLL, KLR, 45); 
        k[9] = (KLL, KLR, 45);
        k[10] = (KAL, KAR, 45); 
        k[11] = (KAL, KAR, 45);
        kl[2] = (KLL, KLR, 60); 
        kl[3] = (KLL, KLR, 60); 
        k[12] = (KRL, KRR, 60); 
        k[13] = (KRL, KRR, 60);
        k[14] = (KBL, KBR, 60); 
        k[15] = (KBL, KBR, 60);
        k[16] = (KLL, KLR, 77); 
        k[17] = (KLL, KLR, 77); 
        kl[4] = (KAL, KAR, 77); 
        kl[5] = (KAL, KAR, 77);
        k[18] = (KRL, KRR, 94); 
        k[19] = (KRL, KRR, 94); 
        k[20] = (KAL, KAR, 94); 
        k[21] = (KAL, KAR, 94); 
        k[22] = (KLL, KLR, 111); 
        k[23] = (KLL, KLR, 111); 
        kw[2] = (KBL, KBR, 111); 
        kw[3] = (KBL, KBR, 111);
    }
}

// достатньо...
// там точно має бути отой порядок ключів типу K = K_L, K_R = 0 etc але я поки не поняв його подогнать
//апд: вот смішно буде якщо я читаю неправильно і читати правильно я теж нж можу

//взагалі є ідея на іфах зробити типу еее
//залежно від довжини ключа то різний тип генерації
//тіпа іф кейленгхт = 128 елзе 

void cipher_use(const std::vector<uint8_t>& key) {
    if (key.size() == 16) key_lenght = 128;
    else if (key.size() == 24) key_lenght = 192;
    else if (key.size() == 32) key_lenght = 256;
    init_sboxes();
    generate_keys(key);
}