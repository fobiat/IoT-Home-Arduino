# Quick Start Guide - Arduino IoT Home Assistant

Get your Arduino R4 IoT system up and running in 30 minutes!

## What You'll Have Working

✅ Real-time temperature, humidity, and pressure readings
✅ Distance measurement with ultrasonic sensor
✅ Soil moisture monitoring
✅ Sound level detection
✅ Rain detection
✅ Motion detection (MPU6050)
✅ Home Assistant dashboard showing all data
✅ Voice control via Home Assistant

## 5-Minute Setup Overview

### Step 1: Prepare Your Arduino (2 min)
1. Download Arduino IDE 2.0+ from arduino.cc
2. Open IDE → Preferences → Additional Board Manager URLs
3. Add: `https://github.com/arduino/ArduinoCore-renesas/releases/download/0.2.1/package_arduino_renesas_index.json`
4. Board Manager → Search "Arduino Uno R4" → Install
5. Tools → Board → Select "Arduino Uno R4 WiFi"

### Step 2: Install Sensor Libraries (2 min)
```
Tools → Manage Libraries → Search and Install:
- PubSubClient
- Adafruit BMP280
- DHT sensor library
- MPU6050
- LiquidCrystal I2C
```

### Step 3: Configure WiFi & MQTT (1 min)
Edit lines 10-16 in `IoT_Home_Assistant.ino`:
```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
const char* mqtt_server = "192.168.1.100";  // Your Home Assistant IP
```

---

## Wiring - Quick Reference

**Don't have much time? Wire these first:**

1. **DHT22** (Temperature)
   - VCC → 5V
   - GND → GND
   - DATA → GPIO 2

2. **BMP280** (Pressure)
   - VCC → 3.3V
   - GND → GND
   - SDA → A4
   - SCL → A5

3. **LCD1602** (Display)
   - VCC → 5V
   - GND → GND
   - SDA → A4
   - SCL → A5

4. **Relay** (Control)
   - VCC → 5V
   - GND → GND
   - IN1 → GPIO 6
   - IN2 → GPIO 7

**Add more sensors after you get these 4 working!**

For complete wiring, see `WIRING_GUIDE.md`

---

## Upload & Test

1. Connect Arduino via USB-C cable
2. Open `IoT_Home_Assistant.ino`
3. Click Upload (arrow icon)
4. Wait for "Done uploading"
5. Open Serial Monitor (Tools → Serial Monitor)
6. Set baud rate to 115200
7. Check for messages:
   ```
   WiFi connected!
   IP address: 192.168.1.XXX
   Attempting MQTT connection...
   ```

---

## Home Assistant Integration - 3 Steps

### 1. Ensure MQTT is running
- Settings → Devices & Services → MQTT
- If not installed: Settings → Add-ons → MQTT Broker

### 2. Add sensors to configuration.yaml
Copy from `config/home_assistant.yaml` or use auto-discovery:
- Restart Home Assistant
- Check Settings → Devices & Services → MQTT
- New devices should appear

### 3. Add to Dashboard
- Create new dashboard
- Add card → Entities
- Search for "Arduino"
- Add sensors you want to display

---

## Troubleshooting in 60 Seconds

**Arduino won't upload**
- Try different USB port
- Check Tools → Board is "Arduino R4"

**WiFi won't connect**
- Check SSID/password in code
- Restart router
- Check serial output for error

**MQTT won't connect**
- Verify broker IP (check Home Assistant IP)
- Check MQTT add-on is running
- Restart Home Assistant

**Sensors show "0" or "NaN"**
- Check I2C pull-up resistors installed
- Verify A4/A5 connections
- Reseat all wires

**LCD not showing anything**
- Check I2C address (0x27 or 0x3F)
- Adjust contrast pot on backpack
- Verify power connections

---

## Next Steps After Setup

### Week 1: Get comfortable
- Monitor sensor readings in Home Assistant
- Learn MQTT topics (`docs/MQTT_TOPICS.md`)
- Create simple automations

### Week 2: Add more sensors
- Wire soil moisture sensor
- Add sound detector
- Install rain sensor
- Follow `BREADBOARD_LAYOUT.md`

### Week 3: Create automations
- High temperature alert
- Watering reminder
- Noise detection
- See `config/home_assistant.yaml` examples

### Week 4: Expand system
- Add second Arduino with different sensors
- Setup data logging to InfluxDB
- Create custom dashboards
- See `RECOMMENDED_UPGRADES.md`

---

## Sensor Readings Expected

| Sensor | Typical Value |
|--------|---------------|
| Temperature | 15-30°C |
| Humidity | 30-70% |
| Pressure | 1000-1020 hPa |
| Distance | 10-200cm (if object nearby) |
| Soil Moisture | 300-700 (between dry and wet) |
| Sound Level | 0-300 (quiet room) |
| Rain | "dry" (unless wet) |

If values are way off, see calibration guide: `docs/SENSOR_CALIBRATION.md`

---

## File Reference

| File | Purpose |
|------|---------|
| `IoT_Home_Assistant.ino` | Main sketch - upload this |
| `WIRING_GUIDE.md` | Complete breadboard diagram |
| `BREADBOARD_LAYOUT.md` | Visual layout reference |
| `home_assistant.yaml` | Copy to HA configuration |
| `MQTT_TOPICS.md` | All available topics |
| `SENSOR_CALIBRATION.md` | Calibrate sensors accurately |
| `RECOMMENDED_UPGRADES.md` | What to buy next |

---

## Common Modifications

**Change update frequency:**
```cpp
const unsigned long PUBLISH_INTERVAL = 5000;  // 5 seconds instead of 10
```

**Add more buttons:**
```cpp
#define BUTTON5 13
pinMode(BUTTON5, INPUT_PULLUP);
// Check in loop with debounce
```

**Control more relays:**
```cpp
// Add to wiring:
#define RELAY_PIN3 14
#define RELAY_PIN4 15

// Subscribe to more topics:
client.subscribe("home/arduino/control/relay3");
client.subscribe("home/arduino/control/relay4");
```

---

## Safety Notes

⚠️ **Important:**
- Don't connect 5V sensors to 3.3V inputs
- Use resistors with LEDs (prevents burnout)
- Keep liquids away from circuit (soil sensor excepted)
- Restart Arduino if WiFi acts weird
- Change MQTT password in Home Assistant

---

## Getting Help

**Arduino Issues:**
- Arduino forum: https://forum.arduino.cc
- Adafruit support: https://learn.adafruit.com

**Home Assistant Issues:**
- HA community: https://community.home-assistant.io
- HA docs: https://www.home-assistant.io

**This Project:**
- Check MQTT topics with: `mosquitto_sub -h 192.168.1.X -t "home/arduino/#"`
- Monitor serial output for debug messages
- Verify wiring with multimeter (continuity test)

---

## Success Checklist

- [ ] Arduino IDE installed and board selected
- [ ] Libraries installed (5 of them)
- [ ] WiFi credentials entered in code
- [ ] MQTT broker IP entered
- [ ] Code uploaded successfully
- [ ] Serial monitor shows WiFi connected
- [ ] Serial monitor shows "Attempting MQTT connection..."
- [ ] Home Assistant shows new MQTT devices
- [ ] Temperature reading appears in HA
- [ ] Can control relay from HA

**If all ✓ = You're ready to expand!**

---

**Estimated Time to Full Setup:**
- Fast (wiring only essentials): 30 minutes
- Typical (all components): 2 hours
- Thorough (with calibration): 3 hours

**Time to First Reading:** 5 minutes
**Time to Home Assistant Integration:** 10 minutes
**Time to Full Functionality:** 30 minutes

You got this! 🚀
