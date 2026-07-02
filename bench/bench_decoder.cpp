#include <chrono>
#include <iostream>

#include "decoder.hpp"
#include "signal_table.hpp"
#include "simulator.hpp"

int main() {
    auto table = default_signal_table();
    constexpr int kIterations = 1'000'000;

    auto start = std::chrono::high_resolution_clock::now();

    volatile double sink = 0.0; // prevent the optimizer from eliding the loop
    for (int i = 0; i < kIterations; ++i) {
        CanFrame frame = next_simulated_frame(i);
        auto decoded = decode_frame(frame, table);
        if (decoded) {
            sink += decoded->at(0).second;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    std::cout << "Decoded " << kIterations << " frames in " << ns << " ns\n";
    std::cout << "Average: " << (static_cast<double>(ns) / kIterations) << " ns/frame\n";
    std::cout << "(sink=" << sink << ")\n";

    return 0;
}
