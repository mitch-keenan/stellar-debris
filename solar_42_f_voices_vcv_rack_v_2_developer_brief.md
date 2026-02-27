# Solar 42F Voices – VCV Rack v2 Developer Brief

## Overview
This document specifies the **panel controls, I/O, and behaviors** for a VCV Rack v2 plugin inspired by the **Solar 42F** instrument. The goal is an **MVP with extensibility hooks**, preserving the Solar’s sonic character while adding Rack-friendly conveniences.

**Module lineup (4):**
1. Drone Voice (Classic)
2. FM Voice (Papa Srapa)
3. Subtractive Voice (Dual VCO A + B + ADSR/VCA)
4. Dual Filter (Polivoks-inspired + Distortion)

---

## Global Conventions
- **Target:** VCV Rack v2 only
- **Audio levels:** Standard Rack audio
- **Gate behavior (voices):** Gate-sensitive AR (gate high sustains at peak; gate low releases)
- **Normalization:** Implemented via internal routing and context-menu toggles where specified
- **Extensibility:** DSP and parameter surfaces must allow future CV exposure and oscillator/filter upgrades without breaking patches

---

## 1) Drone Voice (Classic)
Emulates Solar voices **1, 2, 4, 5**.

### Panel Controls
- **OSC 1–5 TUNE (Coarse/Fine):** Usability-focused tuning controls (implementation may be per-osc or global pairs)
- **OSC 1–5 MOD AMOUNT:** Depth of modulation per oscillator
- **VOLT (Macro):** Faithful two-region behavior
  - Pre-mid: benign/tonal
  - Post-mid: introduces cross-mod-like instability
- **ATTACK:** AR envelope attack time
- **RELEASE:** AR envelope release time

### CV Inputs
- **GATE:** Triggers internal AR envelope (gate-sensitive)
- **PITCH OFFSET CV:** Global, non-1V/Oct pitch offset applied to all oscillators
- **MOD CV (Global):** Modulation source feeding all per-osc MOD AMOUNT knobs

### Audio Outputs
- **OUT:** Single post-VCA mixed audio output

### Notes / Hooks
- No 1V/Oct tracking
- No internal modulation source
- Leave internal architecture open for future per-osc CV inputs

---

## 2) FM Voice (Papa Srapa)
Emulates Solar voices **3, 6**.

### Panel Controls
- **CARRIER TUNE (Coarse/Fine):** Main oscillator pitch
- **MODULATOR RATE (Coarse/Fine):** Modulator frequency
- **MOD DEPTH:** Amount of AM/FM applied to carrier
- **MODE SWITCH:** Discrete states
  - AM
  - FM
  - AM + FM
- **ATTACK:** AR envelope attack time
- **RELEASE:** AR envelope release time

### CV Inputs
- **V/OCT (Carrier):** 1V/Oct pitch for carrier oscillator
- **GATE:** Triggers internal AR envelope

### Audio Outputs
- **OUT:** Single post-VCA audio output

### Notes / Hooks
- Modulator is free-running (no pitch CV)
- DSP should allow future gate/CV-addressable mode switching

---

## 3) Subtractive Voice (Dual VCO A + B + ADSR/VCA)
Emulates Solar subtractive voices **4 & 5**.

### Panel Controls
#### VCO A
- **TUNE (Coarse/Fine)**
- **FM AMOUNT**
- **PWM AMOUNT**
- **WAVE MIX / SHAPE** (per Solar behavior)

#### VCO B
- **TUNE (Coarse/Fine)**
- **FM AMOUNT**
- **PWM AMOUNT**
- **WAVE MIX / SHAPE**

#### Shared Voice
- **MIX A/B:** Balance between VCO A and B
- **ATTACK:** ADSR attack
- **DECAY:** ADSR decay
- **SUSTAIN:** ADSR sustain
- **RELEASE:** ADSR release

### CV Inputs
- **V/OCT A:** Pitch input for VCO A
- **V/OCT B:** Pitch input for VCO B
- **FM A (Lin/Exp):** Frequency modulation for VCO A
- **FM B (Lin/Exp):** Frequency modulation for VCO B
- **PWM A:** Pulse-width modulation for VCO A
- **PWM B:** Pulse-width modulation for VCO B
- **SYNC A:** Hard sync input for VCO A
- **GATE:** Triggers ADSR

### Audio Outputs
- **VCO A WAVE OUTS:** Multiple waveform outputs (per Solar)
- **VCO B WAVE OUTS:** Multiple waveform outputs (per Solar)
- **OUT:** Single post-VCA audio output

### Context Menu Options
- **Normalize V/OCT A → V/OCT B:** On/Off

### Notes / Hooks
- MVP uses behavioral-match oscillators
- Oscillator core must be swappable later

---

## 4) Dual Filter (Polivoks-Inspired + Distortion)
Post-voice processing inspired by Solar’s filter section.

### Panel Controls
#### Channel 1
- **FREQ 1:** Cutoff frequency
- **RES 1:** Resonance

#### Channel 2
- **FREQ 2:** Cutoff frequency (offset when linked)
- **RES 2:** Resonance (offset when linked)

#### Global
- **MODE:** LP / BP toggle
- **LINK:** Button
  - ON: FREQ 1 / RES 1 control both channels
  - FREQ 2 / RES 2 act as offsets for channel 2 only
- **DRIVE:** Distortion amount

### CV Inputs
- **IN 1:** Audio input channel 1
- **IN 2:** Audio input channel 2
- **FREQ CV 1:** Cutoff modulation for channel 1
- **FREQ CV 2:** Cutoff modulation for channel 2
- **RES CV 1:** Resonance modulation for channel 1
- **RES CV 2:** Resonance modulation for channel 2

### Audio Outputs
- **OUT 1:** Filtered/distorted output channel 1
- **OUT 2:** Filtered/distorted output channel 2

### Notes / Hooks
- 12 dB/oct Polivoks-inspired topology
- Distortion is post-filter
- DSP structured for future oversampling and character refinement

---

## Parameter Map Tables

> Parameter ranges are suggested defaults for MVP and may be tuned by ear. CV ranges follow common VCV Rack conventions unless otherwise noted.

---

### 1) Drone Voice (Classic) — Parameter Map

| Parameter | Type | Range | CV | Notes |
|---------|------|-------|----|------|
| OSC 1–5 TUNE (Coarse) | Knob | ±2 oct (free) | — | Free-running; not 1V/Oct |
| OSC 1–5 TUNE (Fine) | Knob | ±50 cents | — | Usability convenience |
| OSC 1–5 MOD AMOUNT | Knob | 0 → 100% | — | Scales global MOD CV |
| VOLT | Knob | 0 → 100% | CV-ready (future) | Two-region macro behavior |
| ATTACK | Knob | 5 ms → 5 s | — | AR envelope |
| RELEASE | Knob | 5 ms → 10 s | — | AR envelope |
| PITCH OFFSET | CV In | ±5 V | ✔ | Global offset; non-quantized |
| MOD | CV In | ±5 V | ✔ | Shared modulation source |
| GATE | CV In | High ≥1 V | ✔ | Gate-sensitive AR |
| OUT | Audio Out | — | — | Post-VCA mix |

---

### 2) FM Voice (Papa Srapa) — Parameter Map

| Parameter | Type | Range | CV | Notes |
|---------|------|-------|----|------|
| CARRIER TUNE (Coarse) | Knob | ±4 oct | — | Main pitch control |
| CARRIER TUNE (Fine) | Knob | ±50 cents | — | |
| MODULATOR RATE (Coarse) | Knob | 0.05 Hz → 5 kHz | — | LFO → audio |
| MODULATOR RATE (Fine) | Knob | ±1 oct | — | |
| MOD DEPTH | Knob | 0 → 100% | — | FM/AM amount |
| MODE | Switch | AM / FM / AM+FM | Gate-ready (future) | Discrete states |
| ATTACK | Knob | 5 ms → 5 s | — | AR envelope |
| RELEASE | Knob | 5 ms → 10 s | — | AR envelope |
| V/OCT (Carrier) | CV In | 1V/Oct | ✔ | Carrier only |
| GATE | CV In | High ≥1 V | ✔ | Gate-sensitive AR |
| OUT | Audio Out | — | — | Post-VCA |

---

### 3) Subtractive Voice (Dual VCO A + B) — Parameter Map

#### VCO A / VCO B (identical sets)

| Parameter | Type | Range | CV | Notes |
|---------|------|-------|----|------|
| TUNE (Coarse) | Knob | ±4 oct | — | |
| TUNE (Fine) | Knob | ±50 cents | — | |
| FM AMOUNT | Knob | 0 → 100% | ✔ | Lin/Exp applied internally |
| PWM AMOUNT | Knob | 0 → 100% | ✔ | |
| WAVE SHAPE / MIX | Knob | 0 → 100% | — | Solar-style morph |
| V/OCT | CV In | 1V/Oct | ✔ | Per VCO |
| FM | CV In | ±5 V | ✔ | |
| PWM | CV In | ±5 V | ✔ | |
| SYNC (A only) | CV In | Rising edge | ✔ | Hard sync |
| WAVE OUTS | Audio Out | — | — | Multiple per VCO |

#### Shared Voice Controls

| Parameter | Type | Range | CV | Notes |
|---------|------|-------|----|------|
| MIX A/B | Knob | 0 → 100% | — | Balance A vs B |
| ATTACK | Knob | 5 ms → 5 s | — | ADSR |
| DECAY | Knob | 5 ms → 5 s | — | ADSR |
| SUSTAIN | Knob | 0 → 100% | — | ADSR |
| RELEASE | Knob | 5 ms → 10 s | — | ADSR |
| GATE | CV In | High ≥1 V | ✔ | Triggers ADSR |
| OUT | Audio Out | — | — | Post-VCA |

Context Menu:
- Normalize **V/OCT A → V/OCT B** (On/Off)

---

### 4) Dual Filter (Polivoks-Inspired) — Parameter Map

| Parameter | Type | Range | CV | Notes |
|---------|------|-------|----|------|
| FREQ 1 | Knob | 20 Hz → 18 kHz | ✔ | Master when LINK on |
| RES 1 | Knob | 0 → self-osc | ✔ | Polivoks-style |
| FREQ 2 | Knob | ±2 oct offset | ✔ | Offset when LINK on |
| RES 2 | Knob | ±50% offset | ✔ | Offset when LINK on |
| MODE | Switch | LP / BP | — | 12 dB/oct |
| LINK | Button | On / Off | — | Shared control mode |
| DRIVE | Knob | 0 → 100% | — | Post-filter distortion |
| IN 1 | Audio In | — | — | Channel 1 |
| IN 2 | Audio In | — | — | Channel 2 |
| OUT 1 | Audio Out | — | — | Channel 1 |
| OUT 2 | Audio Out | — | — | Channel 2 |

---

## Internal Signal-Flow (Textual Diagrams)

> These are implementation-oriented descriptions. They define expected ordering of blocks and where key parameters apply. Exact DSP choices (band-limiting, oversampling) are left to the implementer within the stated behaviors.

---

### 1) Drone Voice (Classic) — Signal Flow

**High-level:**
1. **Gate In** → **AR Envelope Generator** (gate-sensitive: sustain at peak while gate high)
2. **5× Oscillators** (free-running)
   - Each oscillator frequency = base tune (coarse/fine) + **global PITCH OFFSET CV** (scaled)
   - Each oscillator has a **per-osc MOD AMOUNT** applied to shared **MOD CV**
   - MOD application point: **osc frequency / phase / waveshape** is implementation-defined, but must respond musically and interact with VOLT as specified
3. **VOLT Macro Block**
   - Applies a macro transformation to the oscillator bank / interactions
   - Must exhibit two-region behavior:
     - 0→~50%: relatively benign tonal variation
     - ~50→100%: introduces cross-mod-like instability / complexity
4. **Internal Mix** (sum of 5 oscillators, with headroom)
5. **VCA** controlled by AR envelope
6. **OUT**

**Notes:**
- No pre-VCA taps
- No internal modulation source
- Keep oscillator design modular so per-osc CV inputs can be added later

---

### 2) FM Voice (Papa Srapa) — Signal Flow

**High-level:**
1. **Gate In** → **AR Envelope Generator** (gate-sensitive)
2. **Carrier Oscillator**
   - Frequency = **V/OCT (1V/Oct)** + coarse/fine tune
3. **Modulator Oscillator**
   - Frequency = coarse/fine rate (no pitch CV in MVP)
4. **Mode Switch (Discrete)**
   - **AM:** carrier amplitude multiplied by (1 + modulator × depth)
   - **FM:** carrier frequency/phase modulated by (modulator × depth)
   - **AM+FM:** both applied (ordering may be tuned; recommend FM then AM for stability)
5. **VCA** controlled by AR envelope
6. **OUT**

**Hooks:**
- Implement mode selection in a way that could later accept gate/CV-addressing without changing patch I/O

---

### 3) Subtractive Voice (Dual VCO A + B + ADSR/VCA) — Signal Flow

**High-level:**
1. **Gate In** → **ADSR Envelope Generator**
2. **VCO A Core**
   - Frequency = **V/OCT A** + coarse/fine
   - FM input (lin/exp behavior) scaled by FM AMOUNT
   - PWM input scaled by PWM AMOUNT
   - Sync input hard-resets (or phase-syncs) oscillator (A only)
   - Provides multiple waveform outputs
3. **VCO B Core**
   - Frequency = **V/OCT B** + coarse/fine
   - If context-menu normalization enabled: **V/OCT B normalled from V/OCT A when B input unpatched**
   - FM and PWM as above
   - Provides multiple waveform outputs
4. **A/B Mix**
   - Selected internal mix for final voice output uses a defined pair of waveforms (or a Solar-like wave select/morph per VCO)
   - Apply **MIX A/B** to combine into a single voice signal
5. **VCA** controlled by ADSR
6. **OUT**

**Notes / Hooks:**
- Oscillator cores should be abstracted behind an interface so a higher-fidelity model can replace them later
- Keep waveform outs “raw” relative to the voice output (i.e., before VCA), consistent with typical Rack expectations

---

### 4) Dual Filter (Polivoks-Inspired + Distortion) — Signal Flow

**Per-channel baseline (LINK off):**
1. **IN n**
2. **CV Summing**
   - Cutoff control signal = FREQ n knob + FREQ CV n (scaled)
   - Resonance control signal = RES n knob + RES CV n (scaled)
3. **12 dB Polivoks-Inspired Filter**
   - Mode select: LP or BP
4. **Distortion (Post-filter)**
   - DRIVE controls input gain / waveshaping amount
5. **OUT n**

**LINK on behavior:**
- Channel 1 control signals become master:
  - Cutoff1 drives both channels
  - Res1 drives both channels
- Channel 2 knobs and CVs become **offsets applied only to channel 2**:
  - Cutoff2 effective = Cutoff1 + Offset2
  - Res2 effective = Res1 + Offset2

**Notes / Hooks:**
- Consider clamping/rescaling offsets to avoid unusable extremes
- Keep filter core modular to allow future oversampling or refined character

---

## Implementation Notes (Non-UI)
- Ensure deterministic save/load of context-menu states
- Avoid breaking patch compatibility when adding future CV inputs
- Document internal signal flow for future contributors

