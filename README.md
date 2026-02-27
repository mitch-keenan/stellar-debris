# Stellar Debris — VCV Rack v2 Plugin

A VCV Rack v2 plugin inspired by the **Solar 42F** synthesizer. Four modules capturing the Solar's drone, FM, subtractive, and filter character.

## Modules

| Module | Description |
|---|---|
| **Drone Voice** | Five free-running oscillators with per-osc modulation and a two-region VOLT macro. Gate-sensitive AR envelope. |
| **FM Voice** | Carrier + modulator with switchable AM / FM / AM+FM modes. 1V/Oct carrier tracking, gate-sensitive AR envelope. |
| **Subtractive Voice** | Dual VCO (A/B) with FM, PWM, hard sync, wave shape, and per-VCO waveform outputs. ADSR envelope. V/Oct normalization option. |
| **Dual Filter** | Dual Polivoks-inspired 12dB/oct SVF (LP/BP) with linkable channels and post-filter distortion. |

## Building

Requires the [VCV Rack SDK](https://vcvrack.com/manual/PluginDevelopmentStarter) (v2).

```bash
# Set the SDK path (default: /Users/mitch/vcv-rack-dev/Rack-SDK)
export RACK_DIR=/path/to/Rack-SDK

# Build the plugin
make

# Run DSP tests (no Rack SDK needed)
make -C test && ./test/run_tests

# Full check (tests + plugin build)
./scripts/check.sh
```

## Project Structure

```
├── src/
│   ├── plugin.hpp / plugin.cpp   # Plugin registration
│   ├── DroneVoice.cpp            # Module implementations
│   ├── FMVoice.cpp
│   ├── SubtractiveVoice.cpp
│   ├── DualFilter.cpp
│   └── dsp/                      # Header-only DSP (no Rack dependencies)
│       ├── AREnvelope.hpp
│       ├── ADSREnvelope.hpp
│       ├── Oscillator.hpp
│       └── PolivoksFilter.hpp
├── res/                          # SVG panel graphics
├── test/                         # Standalone DSP test suite
└── scripts/check.sh              # Build verification script
```

## Architecture

DSP classes in `src/dsp/` are header-only with zero Rack dependencies. They take `float sampleRate` and expose `float process(...)` methods. This separation allows headless testing via `make -C test` without the Rack SDK.

Module files in `src/*.cpp` are self-contained — each defines its Module, ModuleWidget, and Model in a single file.

## License

GPL-3.0-or-later
