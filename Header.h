#ifndef HEADER_H
#define HEADER_H

#include <vector>
#include <cstdint>

void generate_keys();
void encrypt_block(uint8_t block[16]);
std::vector<uint8_t> encrypt_data(std::vector<uint8_t>& text);
void decrypt_block(uint8_t block[16]);
std::vector<uint8_t> decrypt_data(const std::vector<uint8_t>& ciphertext);

static constexpr size_t block_size = 16;

int key_lenght;
uint64_t kw[4];
uint64_t k[24];
uint64_t kl[6];

static uint8_t SBOX1[256];
static uint8_t SBOX2[256];
static uint8_t SBOX3[256];
static uint8_t SBOX4[256];
static bool inited_sbox;
static void init_sboxes();

void generate_keys(const std::vector<uint8_t>& key);
static const uint64_t sigma[6];
void cipher_use(const std::vector<uint8_t>& key);

static inline uint64_t load64(const uint8_t* p);
static inline void store64(uint8_t* p, uint64_t v);
static inline uint64_t high128(uint64_t high, uint64_t low, int n);
static inline uint64_t low128(uint64_t high, uint64_t low, int n);

uint64_t F(uint64_t x, uint64_t ke);
uint64_t FL(uint64_t x, uint64_t kl);
uint64_t FLIN(uint64_t y, uint64_t kl);

#endif