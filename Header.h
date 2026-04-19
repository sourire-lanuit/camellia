#ifndef HEADER_H
#define HEADER_H

#include <vector>
#include <cstdint>

void encrypt(uint8_t* b, Key& roundkey);
void decrypt(uint8_t* b, Key& roundkey);
void generate_keys();
std::vector<uint8_t> encrypt_data(std::vector<uint8_t> text, Key& roundkey);
std::vector<uint8_t> decrypt_data(std::vector<uint8_t> data, Key& roundkey)

struct Key {
    uint64_t kw[4];
    uint64_t k[24];
    uint64_t kl[6];
};  

static uint8_t SBOX1[256];
static uint8_t SBOX2[256];
static uint8_t SBOX3[256];
static uint8_t SBOX4[256];
static bool inited_sbox;
static void init_sboxes();

#endif