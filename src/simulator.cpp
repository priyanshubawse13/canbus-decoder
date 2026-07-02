#include "simulator.hpp"
#include <cmath>
#include <cstring>

namespace {

void write_u16_le(CanFrame& f, uint16_t value) {
    f.data[0] = static_cast<uint8_t>(value & 0xFF);
    f.data[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
}

} // namespace

CanFrame next_simulated_frame(int tick) {
    CanFrame frame{};
    int which = tick % 3;

    if (which == 0) {
        frame.id = 0x200;
        frame.dlc = 8;
        // Speed oscillates between 0 and 120 km/h like a stop-and-go drive cycle.
        double speed_kph = 60.0 + 60.0 * std::sin(tick * 0.05);
        uint16_t raw = static_cast<uint16_t>(speed_kph / 0.01);
        write_u16_le(frame, raw);
    } else if (which == 1) {
        frame.id = 0x201;
        frame.dlc = 8;
        // RPM correlates loosely with speed to look like a real trace.
        double rpm = 900.0 + 3000.0 * std::abs(std::sin(tick * 0.05));
        uint16_t raw = static_cast<uint16_t>(rpm / 0.25);
        write_u16_le(frame, raw);
    } else {
        frame.id = 0x202;
        frame.dlc = 8;
        // Door bitmask: bit0=driver, bit1=passenger, bit2=rear-left, bit3=rear-right.
        uint8_t mask = (tick % 40 == 0) ? 0b0001 : 0b0000;
        frame.data[0] = mask;
    }

    return frame;
}
