#include "signal_table.hpp"

std::vector<SignalDef> default_signal_table() {
    return {
        {"VehicleSpeed", 0x200, 0, 16, 0.01, 0.0, "km/h"},
        {"EngineRPM",    0x201, 0, 16, 0.25, 0.0, "rpm"},
        {"DoorStatus",   0x202, 0, 4,  1.0,  0.0, "bitmask"},
    };
}
