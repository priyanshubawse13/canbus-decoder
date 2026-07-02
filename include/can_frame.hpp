#pragma once
#include <array>
#include <cstdint>

struct CanFrame {
    uint32_t id;                    // 11-bit standard CAN identifier
    uint8_t dlc;                    // data length code (0-8)
    std::array<uint8_t, 8> data{};  // raw payload bytes
};
