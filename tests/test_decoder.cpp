// Minimal header-only test harness — no external framework required.
#include <cassert>
#include <iostream>

#include "decoder.hpp"
#include "signal_table.hpp"

static void test_speed_decode() {
    SignalDef speed_def{"VehicleSpeed", 0x200, 0, 16, 0.01, 0.0, "km/h"};

    CanFrame frame{};
    frame.id = 0x200;
    frame.dlc = 8;
    // raw = 6000 -> 6000 * 0.01 = 60.0 km/h
    frame.data[0] = 0x70; // low byte of 6000 (0x1770)
    frame.data[1] = 0x17; // high byte

    double value = decode_signal(frame, speed_def);
    assert(value > 59.99 && value < 60.01);
    std::cout << "test_speed_decode passed\n";
}

static void test_unknown_id_returns_nullopt() {
    auto table = default_signal_table();

    CanFrame frame{};
    frame.id = 0x999; // not in the table
    frame.dlc = 8;

    auto result = decode_frame(frame, table);
    assert(!result.has_value());
    std::cout << "test_unknown_id_returns_nullopt passed\n";
}

static void test_door_status_bitmask() {
    auto table = default_signal_table();

    CanFrame frame{};
    frame.id = 0x202;
    frame.dlc = 8;
    frame.data[0] = 0b0001; // driver door open

    auto result = decode_frame(frame, table);
    assert(result.has_value());
    assert(result->at(0).second == 1.0);
    std::cout << "test_door_status_bitmask passed\n";
}

int main() {
    test_speed_decode();
    test_unknown_id_returns_nullopt();
    test_door_status_bitmask();
    std::cout << "All tests passed.\n";
    return 0;
}
