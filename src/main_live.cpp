#include <iostream>

#include "decoder.hpp"
#include "signal_table.hpp"
#include "socketcan_reader.hpp"

int main(int argc, char** argv) {
    const char* iface = (argc > 1) ? argv[1] : "vcan0";

    int sock_fd = open_socketcan(iface);
    if (sock_fd < 0) {
        std::cerr << "Failed to open SocketCAN interface: " << iface << "\n";
        std::cerr << "Is it up? Try: sudo ip link set up " << iface << "\n";
        return 1;
    }

    std::cout << "canbus-decoder (LIVE): listening on " << iface << "\n";
    std::cout << "Send test frames from another shell, e.g.:\n";
    std::cout << "  cansend " << iface << " 200#DEADBEEF01020304\n\n";

    auto table = default_signal_table();
    CanFrame frame{};

    while (read_can_frame(sock_fd, frame)) {
        auto decoded = decode_frame(frame, table);
        std::cout << "[0x" << std::hex << frame.id << std::dec << "] ";
        if (decoded) {
            for (const auto& [name, value] : *decoded) {
                std::cout << name << "=" << value << "  ";
            }
        } else {
            std::cout << "(no signal definition for this id)";
        }
        std::cout << "\n";
    }

    close_socketcan(sock_fd);
    return 0;
}
