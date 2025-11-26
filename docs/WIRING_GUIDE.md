# Arduino R4 Mini - IoT Home Assistant Wiring Guide

## Overview
This guide shows how to wire all your components to the Arduino R4 Mini on a breadboard.

## Pin Configuration

### Power Rails
- **GND (Ground)**: Connect to negative rail
- **5V**: Connect to positive rail (from USB or battery)

### Sensors

#### 1. **DHT22 (Temperature & Humidity)**
- **VCC** → 5V
- **GND** → GND
- **DATA** → GPIO 2
- **Note**: Add 10kΩ pull-up resistor between DATA and VCC

#### 2. **BMP280 (Air Pressure & Altitude)**
- **VCC** → 3.3V
- **GND** → GND
- **SDA** → A4 (I2C)
- **SCL** → A5 (I2C)
- **Address**: 0x76 (default)

#### 3. **MPU6050 (Gyroscope & Accelerometer)**
- **VCC** → 3.3V
- **GND** → GND
- **SDA** → A4 (I2C, shared with BMP280)
- **SCL** → A5 (I2C, shared with BMP280)
- **Address**: 0x68 (default)
- **Note**: Add 4.7kΩ pull-up resistors on SDA/SCL if needed

#### 4. **Ultrasonic Sensor HC-SR04 (Distance)**
- **VCC** → 5V
- **GND** → GND
- **TRIG** → GPIO 4
- **ECHO** → GPIO 5

#### 5. **Soil Moisture Sensor**
- **VCC** → 5V
- **GND** → GND
- **AO (Analog)** → A1
- **DO (Digital)** → Not used (can connect to GPIO for threshold alerts)

#### 6. **Sound Sensor**
- **VCC** → 5V
- **GND** → GND
- **AO (Analog)** → A0
- **DO (Digital)** → Optional GPIO

#### 7. **Rain Drop Sensor**
- **VCC** → 5V
- **GND** → GND
- **AO (Analog)** → A2 (optional)
- **DO (Digital)** → A2 (used for wet/dry detection)

#### 8. **Capacitive Touch Module**
- **VCC** → 5V
- **GND** → GND
- **OUT** → A3

### Outputs & Controls

#### 9. **2-Channel Relay Module**
- **VCC** → 5V
- **GND** → GND
- **IN1** → GPIO 6 (Relay 1)
- **IN2** → GPIO 7 (Relay 2)
- **Common/NO/NC**: Wire to controllable devices

#### 10. **Servo Motor (9g)**
- **VCC** → 5V
- **GND** → GND
- **Signal** → GPIO 8
- **Note**: Can draw significant current; consider separate power supply if issues occur

#### 11. **Buzzer**
- **VCC** → GPIO 3
- **GND** → GND
- **Note**: For passive buzzer, add 220Ω resistor in series

#### 12. **Speaker Module**
- **VCC** → 5V
- **GND** → GND
- **IN** → Optional GPIO (currently not used in code, but available on GPIO 9-12)
- **Note**: Use speaker amplifier module if adding audio output

#### 13. **LCD1602 Display (I2C backpack)**
- **VCC** → 5V
- **GND** → GND
- **SDA** → A4 (I2C, shared)
- **SCL** → A5 (I2C, shared)
- **Address**: 0x27 or 0x3F (configure in code)

### Push Buttons & Indicators

#### 14. **Push Buttons (4 used)**
- **Button 1** → GPIO 9 (with 10kΩ pull-down, pressed connects to GND)
- **Button 2** → GPIO 10
- **Button 3** → GPIO 11
- **Button 4** → GPIO 12
- Each button: One side to GPIO, other side to GND

#### 15. **LEDs (with 220Ω resistors)**
- **Blue LED** → GPIO 22 (anode) → 220Ω resistor → GND
- **Yellow LED** → GPIO 23 → 220Ω resistor → GND
- **Red LED** → GPIO 24 → 220Ω resistor → GND
- **White LED** → GPIO 25 → 220Ω resistor → GND

---

## Breadboard Layout

```
POWER SECTION (Left side)
┌─────────────────────────────────┐
│  Red Rail: 5V                   │
│  Black Rail: GND                │
│  Blue Rail: 3.3V (from R4)      │
└─────────────────────────────────┘

I2C DEVICES (Share A4/A5)
┌─────────────────────────────────┐
│  BMP280                         │
│  MPU6050                        │
│  LCD1602 (I2C backpack)         │
│  Pull-ups: 4.7kΩ each on SDA/SCL
└─────────────────────────────────┘

ANALOG INPUTS
┌─────────────────────────────────┐
│  A0: Sound Sensor               │
│  A1: Soil Moisture              │
│  A2: Rain Sensor                │
│  A3: Capacitive Touch           │
└─────────────────────────────────┘

DIGITAL I/O
┌─────────────────────────────────┐
│  GPIO 2: DHT22 DATA             │
│  GPIO 3: Buzzer                 │
│  GPIO 4: Ultrasonic TRIG        │
│  GPIO 5: Ultrasonic ECHO        │
│  GPIO 6: Relay 1                │
│  GPIO 7: Relay 2                │
│  GPIO 8: Servo                  │
│  GPIO 9-12: Push Buttons        │
│  GPIO 22-25: LEDs (w/ resistors)│
└─────────────────────────────────┘
```

---

## Connection Priority (Wiring Order)

1. **Power Distribution** (5V and GND rails)
2. **I2C Devices** (BMP280, MPU6050, LCD - share A4/A5)
3. **DHT22** (Temperature sensor)
4. **Relay Module** (GPIO 6-7)
5. **Buttons** (GPIO 9-12 with GND)
6. **LEDs** (GPIO 22-25 through 220Ω resistors)
7. **Sensors** (Ultrasonic, moisture, sound, rain)
8. **Output Devices** (Servo on GPIO 8, Speaker amplifier)

---

## Power Considerations

- **5V Rail**: DHT22, Ultrasonic, Soil Moisture, Sound, Rain sensors, Relays, Buzzer
- **3.3V Rail**: BMP280, MPU6050 (from Arduino's 3.3V pin)
- **Total Current**: Estimate ~500mA peak (reduce with separate 5V supply if needed)
- **Battery Option**: 9V battery via battery snap to Arduino's power input

---

## Testing Checklist

- [ ] All I2C devices detected (BMP280 0x76, MPU6050 0x68)
- [ ] DHT22 readings stable (20-30°C typical)
- [ ] Ultrasonic distance measurement correct
- [ ] Relay clicks when commanded
- [ ] Buzzer sounds when triggered
- [ ] LEDs light up correctly
- [ ] LCD displays sensor values
- [ ] Buttons register presses
- [ ] WiFi connects successfully
- [ ] MQTT topics publish data to Home Assistant

---

## Troubleshooting

**I2C Devices Not Found**
- Check SDA/SCL connections to A4/A5
- Verify 4.7kΩ pull-up resistors
- Confirm voltage (3.3V for BMP280/MPU6050)

**DHT22 Not Reading**
- Ensure 10kΩ pull-up on DATA line
- Check for loose wires (DHT is sensitive)

**Ultrasonic Showing Wrong Distance**
- Verify TRIG and ECHO on correct pins
- Check sensor isn't blocked

**LEDs Not Lighting**
- Verify 220Ω resistor in series
- Check LED polarity (long leg to GPIO, short to GND through resistor)

**Relay Not Clicking**
- Test with buzzer relay test code
- Ensure GPIO pins set to OUTPUT mode
