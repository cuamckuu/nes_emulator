#ifndef ROM_H
#define ROM_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdint>

using byte = uint8_t;
using word = uint16_t;

struct Headers {
    char signature[4];

    byte prg_size;  // In 16KB units; ROM?
    byte chr_size; // In 8KB units; VROM?

    byte flags6;
    byte flags7;

    byte prg_ram_size; // In 8KB units

    byte flags9;
    byte flags10;

    byte reserved[5];
};

struct ROM {
    Headers header;
    byte *trainer = nullptr;
    byte *prg_data = nullptr;
    byte *chr_data = nullptr;
};

bool verify_header(Headers header);
ROM load_rom(std::string filename);

#endif //ROM_H
