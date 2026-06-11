#include "Header.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <random>

static std::vector<uint8_t> read_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Unable to open&read the file: " + path);
    return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
}

static void write_file(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Unable to write the file: " + path);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

static const uint8_t MAGIC[4] = {'C','A','M','1'};

static std::vector<uint8_t> pack_encrypted(const std::vector<uint8_t>& ciphertext, uint32_t orig_size) {
    std::vector<uint8_t> out_data;
    out_data.insert(out_data.end(), MAGIC, MAGIC + 4);
    out_data.push_back((orig_size >> 24) & 0xFF);
    out_data.push_back((orig_size >> 16) & 0xFF);
    out_data.push_back((orig_size >> 8) & 0xFF);
    out_data.push_back((orig_size) & 0xFF);
    out_data.insert(out_data.end(), ciphertext.begin(), ciphertext.end());
    return out_data;
}

static bool unpack_encrypted(const std::vector<uint8_t>& data, std::vector<uint8_t>& ciphertext, uint32_t& orig_size) {
    if (data.size() < 8) return false;
    if (memcmp(data.data(), MAGIC, 4) != 0) return false;
    orig_size = ((uint32_t)data[4] << 24) | ((uint32_t)data[5] << 16) | ((uint32_t)data[6] << 8) | data[7];
    ciphertext = std::vector<uint8_t>(data.begin() + 8, data.end());
    return true;
}


static void generate_key(const std::string& key_file, int bits) {
    int bytes = bits / 8;
    std::vector<uint8_t> key(bytes);
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<uint16_t> dist(0, 255); 

    for (int i = 0; i < bytes; ++i) {
        key[i] = static_cast<uint8_t>(dist(gen));
    }

    write_file(key_file, key);
    std::cout << "Generated " << bits << "-bit key -> " << key_file << "\n";
}

static void instructions(const char* prog) {
    std::cout << "Camellia file encryption presents:\n\n" << "Usage:\n" << prog << "encrypt <input> <output> <key_file>\n" 
    << prog << "decrypt <input> <output> <key_file>\n" << prog << "genkey <key_file> (128|192|256)\n"
    << "\nFile format: CAM1 header + 4-byte original length + ECB ciphertext\n" << "Key file: raw binary key (16|24|32 bytes)\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        instructions(argv[0]); 
        return 1;
    }

    std::string cmd = argv[1];

    try {
        if (cmd == "genkey") {
            if (argc < 3) { 
                instructions(argv[0]); 
                return 1; 
            }

            int bits = (argc >= 4) ? std::stoi(argv[3]) : 256;
            if (bits != 128 && bits != 192 && bits != 256) {
                std::cerr << "Key size must be 128, 192, or 256\n";
                return 1;
            }
            generate_key(argv[2], bits);

        } else if (cmd == "encrypt") {
            if (argc < 5) { 
                instructions(argv[0]); 
                return 1; 
            }

            auto text = read_file(argv[2]);
            auto key = read_file(argv[4]);
            uint32_t orig_size = (uint32_t)text.size();

            cipher_use(key);
            auto ciphertext = encrypt_data(text);
            auto output = pack_encrypted(ciphertext, orig_size);
            write_file(argv[3], output);

            std::cout << "Encrypted " << orig_size << " bytes to " << ciphertext.size() << " bytes ciphertext\n";
            std::cout << "Output: " << argv[3] << "\n";

        } else if (cmd == "decrypt") {
            if (argc < 5) { 
                instructions(argv[0]); 
                return 1; 
            }

            auto file_data = read_file(argv[2]);
            auto key = read_file(argv[4]);

            std::vector<uint8_t> ciphertext;
            uint32_t orig_size;
            if (!unpack_encrypted(file_data, ciphertext, orig_size)) {
                std::cerr << "Error: input file does not have a valid CAM1 header\n";
                return 1;
            }

            cipher_use(key);
            auto text = decrypt_data(ciphertext);

            if (orig_size > text.size()) {
                std::cerr << "Error: corrupted file (size > ciphertext)\n";
                return 1;
            }
            text.resize(orig_size);
            write_file(argv[3], text);

            std::cout << "Decrypted " << ciphertext.size() << " bytes to " << orig_size << " bytes plaintext\n";
            std::cout << "Output: " << argv[3] << "\n";

        } else {
            std::cerr << "Unknown command: " << cmd << "\n";
            instructions(argv[0]);
            return 1;
        }
    } catch (const std::exception& except) {
        std::cerr << "Error: " << except.what() << "\n";
        return 1;
    }
    return 0;
}