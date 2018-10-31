#include "include/rom.h"

bool verify_header(Headers header) {
    return std::strstr(header.signature, "NES\x1a") != NULL;
}

ROM load_rom(std::string filename) {
    // TODO: Add error handling
    // For simplicity Playchoice part just ignored.
    std::ifstream fin(filename);

    ROM rom;
    fin.read((char*)&rom.header, sizeof(rom.header));
    if (!verify_header(rom.header)) {
        std::cout << "Wrong file\n";
        exit(1);
    }

    bool is_trainer = (rom.header.flags6 >> 3) & 1;
    if (is_trainer) {
        rom.trainer = new byte[512];
        fin.read((char*)rom.trainer, 512);
    }

    rom.prg_data = new byte[rom.header.prg_size * 0x4000];
    fin.read((char*)rom.prg_data, rom.header.prg_size * 0x4000);

    rom.chr_data = new byte[rom.header.chr_size * 0x2000];
    fin.read((char*)rom.chr_data, rom.header.chr_size * 0x2000);

    std::cout << "ROM file loaded\n";
    return rom;
}

