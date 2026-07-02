#include <chrono>
#include <iostream>
#include <thread>

#include "decoder.hpp"
#include "signal_table.hpp"
#include "simulator.hpp"

int main() {
    auto table = default_signal_table();
    std::cout << "canbus-decoder: simulating instrument cluster + body control module\n";
    std::cout << "Press Ctrl+C to stop.\n\n";

    for (int tick = 0; tick < 300; ++tick) {
        CanFrame frame = next_simulated_frame(tick);
        auto decoded = decode_frame(frame, table);

        if (decoded) {
            std::cout << "[0x" << std::hex << frame.id << std::dec << "] ";
            for (const auto& [name, value] : *decoded) {
                std::cout << name << "=" << value << "  ";
            }
            std::cout << "\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return 0;
}
