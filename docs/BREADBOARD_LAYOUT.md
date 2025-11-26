# Arduino R4 Breadboard Wiring Diagram

## Complete Visual Layout

```
POWER DISTRIBUTION
═══════════════════════════════════════════════════════════════════
         RED (5V)                          BLUE (3.3V)
         ┃       ┃       ┃       ┃        ┃       ┃       ┃
────────┃───────┃───────┃───────┃────────┃───────┃───────┃────────
    [✓] ┃   [✓] ┃   [✓] ┃   [✓] ┃        ┃       ┃       ┃
────────┃───────┃───────┃───────┃────────┃───────┃───────┃────────
        ┃       ┃       ┃       ┃        ┃       ┃       ┃
        ↓       ↓       ↓       ↓        ↓       ↓       ↓
    [DHT] [Relay][Buzzer][Sensors]   [BMP280][MPU6050][LCD]
        ┃       ┃       ┃       ┃        ┃       ┃       ┃
────────┃───────┃───────┃───────┃────────┃───────┃───────┃────────
    [✓] ┃   [✓] ┃   [✓] ┃   [✓] ┃    [✓] ┃   [✓] ┃   [✓] ┃
────────┃───────┃───────┃───────┃────────┃───────┃───────┃────────
       BLACK (GND)              (GND from both sides)
═════════════════════════════════════════════════════════════════════

I2C BUS (Shared A4/A5)
═════════════════════════════════════════════════════════════════════
                                    ↑
                         ┌──────────┤
    A4 (SDA) ← ─ ─ ─ ─ ─ ┤4.7kΩ├─ ─ ┤─ VCC
    A5 (SCL) ← ─ ─ ─ ─ ─ ┤4.7kΩ├─ ─ ┤─ VCC
                         └──────────┤
                                    ↑
                         [BMP280]  [MPU6050]  [LCD]
                          (0x76)    (0x68)    (0x27)
═════════════════════════════════════════════════════════════════════

SENSOR CONNECTIONS
═════════════════════════════════════════════════════════════════════

┌─ Analog Inputs ────────────────────────────────────────────────┐
│ A0: Sound Sensor ────→ [Sound Module] ───→ GND               │
│ A1: Soil Moisture ───→ [Soil Sensor] ────→ GND               │
│ A2: Rain Sensor ─────→ [Rain Module] ────→ GND               │
│ A3: Touch Sensor ────→ [Capacitive Touch] → GND              │
└────────────────────────────────────────────────────────────────┘

┌─ Digital I/O ──────────────────────────────────────────────────┐
│ GPIO2: DHT22 Data ───→ [DHT22] ─────→ GND ← 10kΩ resistor    │
│                                                                  │
│ GPIO3: Buzzer ───────→ [Buzzer+] ├─ GND                       │
│                                   │ (passive buzzer)            │
│ GPIO4: Ultra TRIG ───→ [HC-SR04 Trigger]                     │
│ GPIO5: Ultra ECHO ←── [HC-SR04 Echo]                         │
│                                                                  │
│ GPIO6: Relay1 ───────→ [Relay In1] ├─ GND                     │
│ GPIO7: Relay2 ───────→ [Relay In2] ├─ GND                     │
│                                                                  │
│ GPIO8: Servo SIG ────→ [Servo Signal]                         │
│                  ├─ [Servo VCC] ───→ 5V                       │
│                  └─ [Servo GND] ───→ GND                      │
│                                                                  │
│ GPIO9-12: Buttons ───→ [Button] ───→ GND (all buttons)        │
│                       (Internal pull-up enabled)               │
│                                                                  │
│ GPIO22: Blue LED ─────→ [220Ω] ────→ [LED+] (anode)          │
│                                      [LED-] ───→ GND           │
│ GPIO23: Yellow LED ──→ [220Ω] ────→ [LED+]                  │
│ GPIO24: Red LED ─────→ [220Ω] ────→ [LED+]                  │
│ GPIO25: White LED ───→ [220Ω] ────→ [LED+]                  │
└────────────────────────────────────────────────────────────────┘

BREADBOARD ROW ASSIGNMENTS
═════════════════════════════════════════════════════════════════════

LEFT SIDE (Power Rails)
A  │ 5V  │ GND │ 3.3V │
   │  ┃  │  ┃  │  ┃   │
B  │ ┌┴┬┐ │┌─┴─┐│┌──┐ │
   │ │ DHT││Relay││LCD│ │
C  │ │22 ││2ch ││I2C │ │
   │ │Ω  ││ Mod││Back│ │
D  │ └┬┬┘ │└─┬─┘│└──┘ │
   │  ┃ ┃  │  ┃  │     │
E  │ GND  │GND  │GND  │

RIGHT SIDE (Sensors & Inputs)
   │Sound│Soil │Rain │Touch│
   │ A0  │ A1  │ A2  │ A3 │
   │     │     │     │    │
F  │ Btn │Ultra│Btn2 │Btn3│
   │  9  │ 4/5 │ 10  │ 11 │
   │     │     │     │    │
G  │Blue │Yel  │Red  │Wht │
   │ LED │ LED │ LED │ LED│
   │ 22  │ 23  │ 24  │ 25 │

═════════════════════════════════════════════════════════════════════

DETAILED COMPONENT LAYOUT (Top View)

Row 1 (Power Distribution)
┌─────────────────────────────────────────────────────────────────┐
│  +5V   GND   +3.3V  GND  |  Jmp   Jmp   Jmp   Jmp              │
│  ▓▓▓   ▓▓▓   ▓▓▓▓   ▓▓▓  |  ┌──┐  ┌──┐  ┌──┐  ┌──┐           │
│   ↓     ↓     ↓      ↓   |  │R1│  │R2│  │R3│  │R4│           │
│  ┌─┐  ┌─┐  ┌──┐   ┌──┐   |  │ │  │ │  │ │  │ │              │
│  │D│  │B│  │SM│   │Rain  |  │ │  │ │  │ │  │ │              │
│  │H│  │M│  │Oi│   │Mod   |  │ │  │ │  │ │  │ │              │
│  │T│  │28│  │ │   │      |  │ │  │ │  │ │  │ │              │
│  │ │  │0 │  │st│   │      |  │ │  │ │  │ │  │ │              │
│  │ │  │  │  │  │   │      |  └─┘  └─┘  └─┘  └─┘              │
└─────────────────────────────────────────────────────────────────┘
Row 2-8 (Component Terminals)
Row 9 (Ground Distribution)
┌─────────────────────────────────────────────────────────────────┐
│  GND   GND   GND   GND  |  GND   GND   GND   GND              │
│  ▓▓▓   ▓▓▓   ▓▓▓   ▓▓▓  |  ▓▓▓   ▓▓▓   ▓▓▓   ▓▓▓           │
└─────────────────────────────────────────────────────────────────┘

═════════════════════════════════════════════════════════════════════

JUMPER WIRE GUIDE

[Male-to-Male] (60mm): Arduino to Breadboard
- GPIO2 to DHT22
- GPIO3 to Buzzer
- GPIO4-5 to Ultrasonic
- GPIO6-7 to Relay
- GPIO8 to Servo
- GPIO9-12 to Buttons

[Female-to-Female] (80mm): Component Interconnects
- I2C lines (SDA/SCL with pull-ups)
- Power distribution
- Ground connections

[Dupont] (10cm): Fine work
- Sensor calibration wires
- LCD adjustments

═════════════════════════════════════════════════════════════════════

RESISTOR PLACEMENT

220Ω (×4): LED Series Resistors
- GPIO22 ─[220Ω]─→ LED+ ─[LED-]─→ GND
- GPIO23 ─[220Ω]─→ LED+ ─[LED-]─→ GND
- GPIO24 ─[220Ω]─→ LED+ ─[LED-]─→ GND
- GPIO25 ─[220Ω]─→ LED+ ─[LED-]─→ GND

4.7kΩ (×2): I2C Pull-ups
- A4(SDA) ─[4.7kΩ]─→ 3.3V
- A5(SCL) ─[4.7kΩ]─→ 3.3V

10kΩ (×1): DHT22 Pull-up
- GPIO2 ─[10kΩ]─→ 5V

═════════════════════════════════════════════════════════════════════

DEBUGGING CHECKPOINTS

✓ After wiring each section, verify:
  1. Power connections stable (5V ≈ 5.0V, 3.3V ≈ 3.3V)
  2. Ground continuous across board
  3. No loose wires touching adjacent components
  4. Resistors installed correctly
  5. Component orientation correct (especially polarized parts)

✓ Use multimeter to test:
  - Voltage at each power node
  - Continuity between ground points
  - Resistance of resistor networks
  - No shorts between adjacent pins

═════════════════════════════════════════════════════════════════════
