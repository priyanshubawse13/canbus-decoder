#pragma once
#include <optional>
#include "can_frame.hpp"
#include "signal_table.hpp"

// Extracts the raw unsigned integer for a signal from a frame's data bytes,
// then applies physical scaling: physical_value = raw * scale + offset.
double decode_signal(const CanFrame& frame, const SignalDef& def);

// Looks up the signal table for the frame's CAN id and returns a
// name -> physical_value map ready for a dashboard to consume.
// Returns std::nullopt if no signal definitions exist for this id.
std::optional<std::vector<std::pair<std::string, double>>>
decode_frame(const CanFrame& frame, const std::vector<SignalDef>& table);
