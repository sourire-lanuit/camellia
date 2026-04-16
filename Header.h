#ifndef HEADER_H
#define HEADER_H

#include <vector>
#include <cstdint>

void encrypt(uint8_t* b, Key& roundkey);
void decrypt();
void generate_keys();
std::vector<uint8_t> encrypt_data(std::vector<uint8_t> data, Key& roundkey);

struct Key {
    uint64_t kw;
    uint64_t k;
    uint64_t kl;
}; //є таке чуство шо сюда масів надо... 

#endif