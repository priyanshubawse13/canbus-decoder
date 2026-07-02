#include "decoder.hpp"
#include <cstdint>

double decode_signal(const CanFrame& frame, const SignalDef& def) {
    // Pack the 8 data bytes into a 64-bit little-endian integer,
    // then mask out the bits belonging to this signal.
    uint64_t raw64 = 0;
    for (int i = 7; i >= 0; --i) {
        raw64 = (raw64 << 8) | frame.data[static_cast<size_t>(i)];
    }

    uint64_t mask = (def.length_bits >= 64)
        ? ~0ULL
        : ((1ULL << def.length_bits) - 1ULL);

    uint64_t raw_value = (raw64 >> def.start_bit) & mask;

    return static_cast<double>(raw_value) * def.scale + def.offset;
}

std::optional<std::vector<std::pair<std::string, double>>>
decode_frame(const CanFrame& frame, const std::vector<SignalDef>& table) {
    std::vector<std::pair<std::string, double>> results;

    for (const auto& def : table) {
        if (def.can_id == frame.id) {
            results.emplace_back(def.name, decode_signal(frame, def));
        }
    }

    if (results.empty()) {
        return std::nullopt;
    }
    return results;
}
