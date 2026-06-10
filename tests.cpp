#include "Header.h"
#include <iostream>
#include <string>
#include <cstring>

static std::vector<uint8_t> hex_to_bytes(const char* hex) {
    std::vector<uint8_t> out;
    size_t len = strlen(hex);
    for (size_t i = 0; i < len; i += 2) {
        char buf[3] = { hex[i], hex[i+1], 0 };
        out.push_back((uint8_t)strtol(buf, nullptr, 16));
    }
    return out;
}

static std::string bytes_to_hex(const uint8_t* p, size_t n) {
    std::string s;
    for (size_t i = 0; i < n; i++) {
        char buf[3];
        snprintf(buf, sizeof(buf), "%02x", p[i]);
        s += buf;
    }
    return s;
}

static bool unit_test(const char* name, const char* hex_key, const char* text_hex, const char* ciph_hex) {
    auto key = hex_to_bytes(hex_key);
    auto plain = hex_to_bytes(text_hex);
    auto expected = hex_to_bytes(ciph_hex);

    cipher_use(key);
    uint8_t block[16];
    memcpy(block, plain.data(), 16);
    encrypt_block(block);

    bool encrypt = (memcmp(block, expected.data(), 16) == 0);

    std::vector<uint8_t> actual_ciph(block, block + 16);

    decrypt_block(block);
    bool decrypt = (memcmp(block, plain.data(), 16) == 0);

    std::cout << (encrypt && decrypt ? "Pass " : "Fail") << name << "\n";
    if (!encrypt) {
        std::cout << "Expected: " << ciph_hex << "\n";
        std::cout << "Got: " << bytes_to_hex(actual_ciph.data(), 16) << "\n";
    }

    if (!decrypt) std::cout << "Decryption failed: original text isn't recovered\n";

    return encrypt && decrypt;
}

void run_tests() {
    int pass = 0, fail = 0;
    auto test = [&](const char* n, const char* k, const char* p, const char* c) {
        if (unit_test(n, k, p, c)) ++pass; else ++fail;
    };

    std::cout << "Camellia unit-tests\n\n";

    test("128-bit key, test 1",
         "0123456789abcdeffedcba9876543210",
         "0123456789abcdeffedcba9876543210",
         "67673138549669730857065648eabe43");

    test("192-bit key, test 1",
         "0123456789abcdeffedcba98765432100011223344556677",
         "0123456789abcdeffedcba9876543210",
         "b4993401b3e996f84ee5cee7d79b09b9");

    test("256-bit key, test 1",
         "0123456789abcdeffedcba987654321000112233445566778899aabbccddeeff",
         "0123456789abcdeffedcba9876543210",
         "9acc237dff16d76c20ef7c919e3a7509");

    std::cout << "\nECB-mode round-trip tests\n";
    {
        std::vector<uint8_t> key = hex_to_bytes("0123456789abcdeffedcba9876543210");
        std::string example_text = "I (Romance). Dedicated to the blossoming apples."; 
        std::vector<uint8_t> plain(example_text.begin(), example_text.end());
        
        cipher_use(key);
        auto ciphertext = encrypt_data(plain);
        auto recovered = decrypt_data(ciphertext);
        
        bool ok = (recovered == plain);
        std::cout << (ok ? "Pass" : "Fail") << "ECB round-trip (16 bytes)\n";
        if (pass += ok, !ok) fail++;
    }
    {
        std::vector<uint8_t> key = hex_to_bytes("0123456789abcdeffedcba9876543210");
        std::vector<uint8_t> plain = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
        
        cipher_use(key);
        auto ciphertext = encrypt_data(plain);
        bool is_size_right = (ciphertext.size() == 32);
        std::cout << (is_size_right ? "Pass" : "Fail") << "ECB padding (17 bytes to 32 bytes)\n";
        if (pass += is_size_right, !is_size_right) fail++;

        bool threw = false;
        try {
            std::vector<uint8_t> bad = {1, 2, 3};
            decrypt_data(bad);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        std::cout << (threw ? "Pass" : "Fail") << "Decryption failed due to unalinged text length\n";
        if (pass += threw, !threw) fail++;
    }

    std::cout << "\nResults: " << pass << " passed, " << fail << " failed\n";
}

int main() {
    run_tests();
    return 0;
}