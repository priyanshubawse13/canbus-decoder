#pragma once
#include <cstdint>
#include <string>
#include <vector>

// DBC-style signal definition: how to extract a physical value
// out of a raw CAN frame's data bytes.
struct SignalDef {
    std::string name;
    uint32_t can_id;
    uint8_t start_bit;   // 0-63, little-endian bit numbering within the 8 data bytes
    uint8_t length_bits; // number of bits this signal occupies
    double scale;
    double offset;
    std::string unit;
};

// Returns the hardcoded signal table for this project.
// In a real toolchain this would be parsed from a .dbc file.
std::vector<SignalDef> default_signal_table();
