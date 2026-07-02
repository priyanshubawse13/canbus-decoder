#pragma once
#include "can_frame.hpp"

int open_socketcan(const char* iface_name);
bool read_can_frame(int sock_fd, CanFrame& out);
void close_socketcan(int sock_fd);
