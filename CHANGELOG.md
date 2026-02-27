# Changelog

## 2026-02-27 — Plugin Scaffolding & Agentic Dev Harness

Initial buildout of the StellarDebris VCV Rack v2 plugin. The repo went from empty to a fully compilable plugin with testable DSP and 4 module stubs.

### What was created

**Plugin infrastructure:**
- `Makefile` — standard VCV Rack v2 plugin build (targets Rack SDK at `/Users/mitch/vcv-rack-dev/Rack-SDK`)
- `plugin.json` — plugin metadata (slug: `StellarDebris`, version `2.0.0`, GPL-3.0-or-later)
- `.gitignore` — ignores build artifacts, `.dylib`, `.vcvplugin`, test binaries
- `CLAUDE.md` — project conventions for AI agents
- `README.md` — repo usage documentation

**Module stubs** (`src/*.cpp`) — each has full param/input/output enums, `configParam`/`configInput`/`configOutput` calls, empty `process()`, and a `ModuleWidget` with placeholder layout:
- `DroneVoice` — 5 oscillators, 18 params, 3 inputs, 1 output
- `FMVoice` — carrier/modulator FM/AM, 8 params, 2 inputs, 1 output
- `SubtractiveVoice` — dual VCO with ADSR, 15 params, 8 inputs, 9 outputs, V/Oct normalization context menu with JSON save/load
- `DualFilter` — dual Polivoks filter, 7 params, 6 inputs, 2 outputs, link light

**DSP layer** (`src/dsp/*.hpp`) — header-only, zero Rack dependencies:
- `AREnvelope` — gate-sensitive AR (attack/sustain-at-peak/release)
- `ADSREnvelope` — standard ADSR with edge-triggered gate
- `Oscillator` — PolyBLEP anti-aliased oscillator (sine, saw, square, triangle), with sync and pulse width
- `PolivoksFilter` — 12dB/oct SVF with damping-based resonance and tanh soft saturation on state variables (LP/BP modes)

**Panel SVGs** (`res/*.svg`) — minimal dark placeholders (correctly sized per module HP)

**Test harness** (`test/`):
- Zero-dependency minimal test framework (assertion via `test_report()`)
- 27 tests across 4 suites covering envelope shapes/timing, oscillator amplitude/stability, filter frequency response/resonance, and NaN/inf safety
- Standalone build via `make -C test` (no Rack SDK needed)

**Build verification** (`scripts/check.sh`) — single command runs DSP tests then plugin build

### Key design decisions
- **DSP/Rack separation:** All signal processing lives in `src/dsp/` with no `rack::` types, enabling headless testing. Modules in `src/*.cpp` compose these DSP classes.
- **Header-only DSP:** Each DSP class is a single `.hpp` — no separate compilation units, no linking complexity.
- **PolyBLEP for anti-aliasing:** Oscillator uses polynomial bandlimited step correction for saw and square waveforms. Triangle is derived via leaky integration of the PolyBLEP square.
- **Polivoks filter topology:** SVF with `damp = 2*(1 - resonance*0.95)` for the feedback coefficient and tanh soft clipping on both `lp` and `bp` state variables (scaled 0.5x in, 2x out) for Polivoks-style saturation character.
- **Gate-sensitive AR vs edge-triggered ADSR:** DroneVoice and FMVoice use AR (sustain at peak while gate held). SubtractiveVoice uses ADSR (triggered on rising edge).

### Verification status
- `make` — compiles to `plugin.dylib` with no errors or warnings
- `make -C test && ./test/run_tests` — 27/27 tests pass
- `scripts/check.sh` — full pipeline clean
