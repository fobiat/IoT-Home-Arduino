# 🚀 Deployment Checklist - Get Your System Running!

## Pre-Deployment Verification

### Prerequisites ✅
- [ ] Arduino IDE 2.0+ installed
- [ ] Arduino R4 WiFi board selected
- [ ] All 5 sensor libraries installed
- [ ] Arduino connected via USB-C
- [ ] Home Assistant running with MQTT broker
- [ ] WiFi network name & password available

### Code Configuration ✅
- [ ] SSID updated (line 10)
- [ ] WiFi password updated (line 11)
- [ ] MQTT broker IP entered (line 12)
- [ ] MQTT username/password verified (lines 13-14)
- [ ] Pin definitions reviewed

---

## Setup Timeline

### **Phase 1: Preparation (15 minutes)**
```
□ 1. Download Arduino IDE from arduino.cc
□ 2. Install Arduino core (Renesas board package)
□ 3. Select: Arduino Uno R4 WiFi
□ 4. Open Arduino → Manage Libraries
□ 5. Install these 5 libraries:
     □ PubSubClient
     □ Adafruit BMP280
     □ DHT sensor library
     □ MPU6050 (by Electronic Cats)
     □ LiquidCrystal I2C
□ 6. Restart Arduino IDE
```

### **Phase 2: Configuration (5 minutes)**
```
□ 1. Open: arduino/IoT_Home_Assistant.ino
□ 2. Find WiFi configuration (lines 10-14)
□ 3. Enter your WiFi SSID
□ 4. Enter your WiFi password
□ 5. Find MQTT server IP (line 12)
□ 6. Enter Home Assistant MQTT IP
□ 7. Verify MQTT credentials
□ 8. Save file (Ctrl+S)
```

### **Phase 3: Upload (5 minutes)**
```
□ 1. Connect Arduino via USB-C
□ 2. Click Upload button (→)
□ 3. Wait for: "Done uploading"
□ 4. Open Serial Monitor (Ctrl+Shift+M)
□ 5. Set baud to 115200
□ 6. Watch for WiFi connection messages
□ 7. Look for "MQTT connected"
□ 8. Verify messages appear every 10 seconds
```

### **Phase 4: Initial Test (5 minutes)**
```
□ 1. Open Home Assistant
□ 2. Go to: Settings → Devices & Services
□ 3. Find: MQTT
□ 4. Check device shows as "Arduino-R4"
□ 5. Verify sensor entities appeared
□ 6. Click on temperature entity
□ 7. Confirm value is updating (15-30°C)
□ 8. Check all 8 sensor readings
```

---

## Wiring Checklist

### Essential Components (Wire First)
```
PRIORITY 1 - Get These Working First:
□ 1. DHT22 → GPIO 2 (temperature)
□ 2. BMP280 → A4/A5 (pressure)
□ 3. LCD1602 → A4/A5 (display)
□ 4. 2-Relay → GPIO 6-7 (control)

Time: 20 minutes
Result: 4 sensors + display working
```

### Extended Components (Wire After)
```
PRIORITY 2 - Add More Sensors:
□ 1. HC-SR04 Ultrasonic → GPIO 4-5
□ 2. Soil Moisture → A1
□ 3. Sound Sensor → A0
□ 4. Rain Sensor → A2
□ 5. Capacitive Touch → A3
□ 6. MPU6050 → A4/A5 (I2C)
□ 7. Servo → GPIO 8
□ 8. Buzzer → GPIO 3
□ 9. Push Buttons → GPIO 9-12
□ 10. LEDs → GPIO 22-25 (with resistors)

Time: 60-90 minutes
Result: 8 sensors + all controls
```

### Wiring Verification
```
□ 1. Check all I2C pull-ups installed (2× 4.7kΩ)
□ 2. Verify DHT22 pull-up (10kΩ)
□ 3. Confirm all LEDs have series resistors (220Ω)
□ 4. Test power rails with multimeter
   □ 5V rail: 4.8-5.2V
   □ 3.3V rail: 3.2-3.4V
   □ GND: 0V everywhere
□ 5. Check for no shorts (multimeter continuity)
□ 6. Verify no loose wires
```

---

## Testing Sequence

### Serial Monitor (Auto-runs after upload)
```
Expected sequence:
1. Board startup message
2. "Starting Arduino IoT Home Assistant"
3. "Sensors initialized"
4. "Connecting to WiFi: [YOUR_SSID]"
5. ".........." (dots = connecting)
6. "WiFi connected!"
7. "IP address: 192.168.X.X"
8. "Attempting MQTT connection..."
9. "connected"
10. Messages with sensor values every 10s

Errors to check:
- "DHT22 not found!" → Check pin 2 and 10k resistor
- "BMP280 not found!" → Check I2C wires and pull-ups
- "MPU6050 not found!" → Check I2C address (0x68)
- WiFi won't connect → Check SSID/password
- MQTT failed → Check broker IP and connection
```

### Home Assistant Dashboard
```
□ 1. Temperature sensor shows value (°C)
□ 2. Humidity sensor shows percentage (%)
□ 3. Pressure sensor shows hPa value
□ 4. Altitude sensor shows meters
□ 5. Distance sensor shows centimeters
□ 6. All values update every 10 seconds

If not showing:
□ Check Settings → Devices & Services → MQTT
□ Verify MQTT is installed and running
□ Restart Home Assistant
□ Check firewall allows port 1883
```

### Control Test
```
□ 1. Find "Arduino Relay 1" in Home Assistant
□ 2. Click toggle
□ 3. Relay should click (audible sound)
□ 4. Monitor shows state changed
□ 5. Can toggle on/off multiple times
□ 6. Works reliably every time

If not working:
□ Check GPIO 6 wiring
□ Verify relay module powered
□ Check relay switch closes/opens (LED indicator)
```

---

## Troubleshooting Decision Tree

```
Arduino won't upload?
├─ Try different USB port → YES → Done!
├─ Check Board: Arduino R4 WiFi → YES → Done!
└─ Update CH340 driver → DONE

WiFi won't connect?
├─ SSID visible in phone? YES → Check password
├─ Password 100% correct? YES → Try restart router
├─ Check serial output for error → Done!
└─ Try factory reset Arduino

MQTT won't connect?
├─ Broker IP correct? (192.168.1.X) → Check in HA
├─ Port 1883 accessible? → Check firewall
├─ MQTT add-on running? → Check HA settings
└─ Check serial output: "connection refused"

Sensor shows NaN?
├─ I2C wired correctly? → Check A4/A5
├─ Pull-ups installed? (4.7k) → YES → Done!
├─ Sensor power connected? → YES → Try I2C scanner
└─ Reseat all wires

Temperature reads wrong?
├─ Off by ~2°C? → Normal DHT22 error
├─ Way off (50°C)? → Check I2C conflict
├─ Erratic readings? → Check 10k resistor on DHT
└─ Run calibration: SENSOR_CALIBRATION.md
```

---

## First 24 Hours Tasks

### Hour 1: Get Running
- [ ] Install IDE & libraries (15 min)
- [ ] Configure WiFi/MQTT (5 min)
- [ ] Upload sketch (5 min)
- [ ] Watch Serial Monitor (10 min)
- **Result**: System connected ✅

### Hour 2-3: Wire Essential Components
- [ ] DHT22 (temperature)
- [ ] BMP280 (pressure)
- [ ] LCD display
- [ ] Relay module
- **Result**: 4 sensors in Home Assistant ✅

### Hour 4-8: Add More Sensors
- [ ] Ultrasonic (distance)
- [ ] Soil moisture
- [ ] Sound sensor
- [ ] Rain sensor
- [ ] More outputs (buttons, LEDs)
- **Result**: Full 8-sensor system working ✅

### Hour 8+: Fine-tuning
- [ ] Calibrate sensors
- [ ] Create first automation
- [ ] Setup dashboard
- [ ] Plan upgrades
- **Result**: Production system ready ✅

---

## Success Indicators

### Phase 1 Complete ✅
- Arduino uploads without errors
- Serial monitor shows WiFi connected
- Serial monitor shows "MQTT connected"

### Phase 2 Complete ✅
- Home Assistant shows 4 sensor entities
- Values updating every 10 seconds
- Relay responds to toggle command

### Phase 3 Complete ✅
- All 8 sensors visible in Home Assistant
- Readings reasonable for current environment
- All controls (relay, servo, buzzer) working
- LCD displaying sensor values

### Phase 4 Complete ✅
- Sensor values calibrated accurately
- Created at least 1 automation
- Custom dashboard created
- Documentation reviewed

---

## Performance Targets

| Metric | Target | How to Check |
|--------|--------|-------------|
| Startup Time | < 10s | Serial monitor timestamps |
| WiFi Connection | 95%+ reliability | Monitor logs |
| MQTT Uptime | 99%+ | Check last message time |
| Sensor Update Rate | 10 seconds | Dashboard refresh |
| Accuracy | ±5% of reference | Compare to known values |
| Power Draw | < 500mA | USB power meter |
| Latency (HA→Relay) | < 1 second | Physical observation |

---

## Backup & Recovery

### If Arduino Becomes Unresponsive
```
1. Power cycle (unplug USB 30 seconds)
2. If still no response: Upload empty sketch
3. Restore from EEPROM backup (if available)
4. Use Undo button in Arduino IDE
```

### If WiFi Unstable
```
1. Check signal strength (should be > -60 dBm)
2. Reduce WiFi channel interference
3. Move router closer if possible
4. Add antenna upgrade (future)
```

### If MQTT Drops Connection
```
1. Restart Arduino (power cycle)
2. Check broker is running
3. Verify firewall rules
4. Check network throughput
5. Monitor connection logs
```

---

## Maintenance Checklist

### Weekly
- [ ] Check all sensor readings reasonable
- [ ] Verify WiFi stays connected
- [ ] Test relay operation
- [ ] Look for error messages

### Monthly
- [ ] Recalibrate sensors
- [ ] Update Arduino IDE/libraries
- [ ] Check for loose wires
- [ ] Review Home Assistant logs

### Quarterly
- [ ] Clean breadboard
- [ ] Check for corrosion
- [ ] Update firmware if available
- [ ] Backup EEPROM data

---

## Go Live Approval

Before declaring success, verify:

- [ ] Hardware wiring complete
- [ ] Code uploads & runs
- [ ] WiFi connection stable
- [ ] MQTT publishing data
- [ ] Home Assistant receiving data
- [ ] All 8 sensors working
- [ ] Controls functional
- [ ] Display shows readings
- [ ] Calibration complete
- [ ] Documentation reviewed

**Once all ✓ → YOU'RE LIVE! 🎉**

---

## Emergency Contact Reference

**If stuck, check these IN ORDER:**

1. **Serial Monitor** (most common issue revealed here)
2. **WIRING_GUIDE.md** (pin configuration)
3. **SENSOR_CALIBRATION.md** (wrong readings)
4. **MQTT_TOPICS.md** (connection issues)
5. **QUICK_START.md** (general troubleshooting)
6. **Arduino Forum** (if still stuck)

---

## Estimated Timeline

| Activity | Time | Status |
|----------|------|--------|
| Install IDE | 15 min | One-time |
| Install Libraries | 10 min | One-time |
| Configure Code | 5 min | One-time |
| Initial Upload | 5 min | One-time |
| Wire Essentials | 20 min | Setup |
| First Reading | 5 min | Setup |
| Add All Sensors | 60-90 min | Setup |
| Calibration | 30 min | Optional |
| Create Automations | 30 min | Optional |
| **TOTAL** | **2-3 hours** | **First Day** |

---

## 🎯 Next Immediate Action

**→ Open: `docs/QUICK_START.md`**

This guide will walk you through the exact 30-minute setup process.

**Happy building! 🚀**
