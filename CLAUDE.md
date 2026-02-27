# Stellar Debris — VCV Rack v2 Plugin

## Build Commands
- **Plugin build:** `make` (requires `RACK_DIR`, defaults to `/Users/mitch/vcv-rack-dev/Rack-SDK`)
- **DSP tests:** `make -C test && ./test/run_tests`
- **Full check:** `scripts/check.sh` (runs tests then plugin build)
- **Clean:** `make clean` / `make -C test clean`

## Architecture
- **Modules** (`src/*.cpp`): VCV Rack Module/Widget subclasses. Each file is self-contained with its Module, Widget, and Model.
- **DSP** (`src/dsp/*.hpp`): Header-only DSP classes with **zero Rack dependencies**. These take `float sampleRate` and have `float process(...)` methods. This separation enables headless testing.
- **Panels** (`res/*.svg`): SVG panel graphics for each module.
- **Tests** (`test/`): Standalone C++ tests for DSP classes. Build with `make -C test` — no Rack SDK needed.

## Module Lineup
1. **DroneVoice** — 5-oscillator drone voice (Solar 42F Classic voices 1,2,4,5)
2. **FMVoice** — FM/AM synthesis voice (Solar 42F Papa Srapa voices 3,6)
3. **SubtractiveVoice** — Dual VCO + ADSR subtractive voice (Solar 42F voices 4,5)
4. **DualFilter** — Dual Polivoks-inspired filter with distortion

## DSP Components
- `AREnvelope` — Gate-sensitive AR envelope (sustains at peak while gate high)
- `ADSREnvelope` — Standard ADSR envelope
- `Oscillator` — PolyBLEP anti-aliased oscillator (sine, saw, square, triangle)
- `PolivoksFilter` — 12dB/oct Polivoks-style SVF (LP/BP modes)

## Conventions
- **C++ standard:** C++11 (per Rack SDK)
- **No external dependencies** for DSP code
- **Parameter ranges:** See `solar_42_f_voices_vcv_rack_v_2_developer_brief.md` for canonical parameter specs
- **Audio levels:** Standard VCV Rack levels (±5V audio, 0-10V CV)
- **Gate threshold:** High >= 1V
- Every DSP class in `src/dsp/` must have a corresponding test in `test/`

## Adding a New DSP Component
1. Create `src/dsp/YourComponent.hpp` (header-only, no Rack deps)
2. Create `test/test_your_component.cpp` with test suite function
3. Add the source to `test/Makefile` SOURCES list
4. Add `run_your_component_tests()` call in `test/test_main.cpp`

## Adding a New Module
1. Create `src/YourModule.cpp` with Module, Widget, and Model
2. Add `extern Model* modelYourModule;` to `src/plugin.hpp`
3. Add `p->addModel(modelYourModule);` to `src/plugin.cpp` init()
4. Add module entry to `plugin.json`
5. Create `res/YourModule.svg` panel
