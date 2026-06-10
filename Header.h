// вас вітає програма прапала всьо! 
// і стопятсот годин виправлення помилок по всім файлам за підтримки джеміні
// штучка інтелект дебажить код краще за мене

#ifndef HEADER_H
#define HEADER_H

#include <vector>
#include <cstdint>
#include <stdexcept>

extern int key_lenght;
extern uint64_t kw[4];
extern uint64_t k[24];
extern uint64_t kl[6];

extern uint8_t SBOX1[256];
extern uint8_t SBOX2[256];
extern uint8_t SBOX3[256];
extern uint8_t SBOX4[256];
extern const uint64_t sigma[6];

static constexpr size_t block_size = 16;

void init_sboxes();
void generate_keys(const std::vector<uint8_t>& key);
void cipher_use(const std::vector<uint8_t>& key);

void encrypt_block(uint8_t block[16]);
std::vector<uint8_t> encrypt_data(std::vector<uint8_t>& text);
void decrypt_block(uint8_t block[16]);
std::vector<uint8_t> decrypt_data(const std::vector<uint8_t>& ciphertext);

uint64_t load64(const uint8_t* p);
void store64(uint8_t* p, uint64_t v);
uint64_t high128(uint64_t high, uint64_t low, int n);
uint64_t low128(uint64_t high, uint64_t low, int n);

uint64_t F(uint64_t x, uint64_t ke);
uint64_t FL(uint64_t x, uint64_t kl);
uint64_t FLIN(uint64_t y, uint64_t kl);

#endif