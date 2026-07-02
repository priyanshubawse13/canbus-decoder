# canbus-decoder

A synthetic CAN bus **simulator** + **signal decoder** written in C++17.

Simulates a small vehicle network (instrument cluster + body control module)
sending raw CAN frames, then decodes those frames back into real signals
(vehicle speed, engine RPM, door status) using a DBC-style signal definition
table — the same signal-extraction model used by tools like CANalyzer/CANoe,
just built from scratch.

## Why this project

Automotive embedded roles screen for exactly this: understanding of CAN frame
structure, signal packing (bit offset / length / scale / offset), and the
ability to reason about a vehicle network without needing real hardware or a
commercial DBC toolchain.

## Architecture

```
simulator  -->  raw CAN frames (id, dlc, 8 data bytes)
                        |
                        v
                 signal_table.hpp  (DBC-style: name, start_bit, length, scale, offset, unit)
                        |
                        v
                    decoder  -->  decoded signals (speed_kph, rpm, door_open)
                        |
                        v
                   dashboard (stdout, printed on change)
```

## Signals modeled

| Signal      | CAN ID | Start bit | Length | Scale | Offset | Unit  |
|-------------|--------|-----------|--------|-------|--------|-------|
| VehicleSpeed| 0x200  | 0         | 16     | 0.01  | 0      | km/h  |
| EngineRPM   | 0x201  | 0         | 16     | 0.25  | 0      | rpm   |
| DoorStatus  | 0x202  | 0         | 4      | 1     | 0      | bitmask |

## Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Run the simulator + decoder

```bash
./canbus_decoder
```

## Run tests

```bash
ctest --output-on-failure
```

## Run the throughput benchmark

```bash
./bench_decoder
```

## Roadmap / stretch goals

- [ ] Swap the synthetic simulator for a real SocketCAN interface on Linux (`can-utils` + `vcan0`)
- [ ] Load signal definitions from an actual `.dbc` file instead of the hardcoded table
- [ ] Add a minimal web dashboard (WebSocket + canvas gauge) instead of stdout
- [ ] Add CRC/checksum validation and out-of-range fault injection

## License

MIT

## Live mode — real SocketCAN

Beyond the synthetic simulator, this project also runs against a **real Linux
SocketCAN virtual bus**, using the same standard interface (`AF_CAN`,
`SOCK_RAW`) that production automotive tooling like CANalyzer/CANoe or a real
Linux ECU stack uses.

### Setup (Linux only — tested on Ubuntu 26.04 via a Multipass VM on macOS)

```bash
sudo apt install -y can-utils
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
```

### Build and run the live decoder

```bash
cmake --build build --target canbus_decoder_live
./build/canbus_decoder_live vcan0
```

### Send test frames from another shell

```bash
cansend vcan0 200#7017000000000000    # VehicleSpeed ≈ 60.0 km/h
cansend vcan0 201#E00E000000000000    # EngineRPM ≈ 950.0 rpm
cansend vcan0 202#01                  # DoorStatus: driver door open
```

Sample output:[0x200] VehicleSpeed=60

[0x201] EngineRPM=952

[0x202] DoorStatus=1This confirms the decoder correctly extracts signals from real CAN frames
traveling over an actual Linux CAN socket, not just synthetic in-process data.

