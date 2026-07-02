#pragma once
#include "can_frame.hpp"

// Generates one synthetic CAN frame per call, cycling through
// VehicleSpeed (0x200), EngineRPM (0x201), and DoorStatus (0x202)
// with values that vary smoothly over time, similar to a real drive cycle.
CanFrame next_simulated_frame(int tick);
