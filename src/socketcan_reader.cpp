#include "socketcan_reader.hpp"

#include <cstring>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <linux/can.h>
#include <linux/can/raw.h>

int open_socketcan(const char* iface_name) {
    int sock_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock_fd < 0) {
        return -1;
    }

    struct ifreq ifr {};
    std::strncpy(ifr.ifr_name, iface_name, IFNAMSIZ - 1);
    if (ioctl(sock_fd, SIOCGIFINDEX, &ifr) < 0) {
        close(sock_fd);
        return -1;
    }

    struct sockaddr_can addr {};
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sock_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) {
        close(sock_fd);
        return -1;
    }

    return sock_fd;
}

bool read_can_frame(int sock_fd, CanFrame& out) {
    struct can_frame raw_frame {};
    ssize_t nbytes = read(sock_fd, &raw_frame, sizeof(raw_frame));
    if (nbytes != sizeof(raw_frame)) {
        return false;
    }

    out.id = raw_frame.can_id & CAN_EFF_MASK;
    out.dlc = raw_frame.can_dlc;
    for (int i = 0; i < 8; ++i) {
        out.data[static_cast<size_t>(i)] = raw_frame.data[i];
    }

    return true;
}

void close_socketcan(int sock_fd) {
    close(sock_fd);
}
