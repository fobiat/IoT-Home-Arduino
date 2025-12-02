# Arduino R4 Mini - IoT Home Assistant Integration

A comprehensive IoT system using Arduino Uno R4 WiFi to collect sensor data and integrate with Home Assistant via MQTT.

## Project Overview

This project demonstrates a multi-sensor IoT setup that:
- Reads environmental and motion data
- Publishes to Home Assistant via MQTT
- Controls relays and outputs
- Responds to button input
- Displays real-time data on LCD

## Hardware Components Used

### Sensors (8)
- DHT22 - Temperature & Humidity
- BMP280 - Air Pressure & Altitude
- MPU6050 - Gyroscope & Accelerometer
- HC-SR04 Ultrasonic - Distance measurement
- Soil Moisture Sensor
- Sound Level Sensor
- Rain Drop Sensor
- Capacitive Touch Module

### Outputs (4)
- 2-Channel Relay Module (control AC/DC devices)
- Servo Motor (9g)
- Buzzer (passive)
- 4× LEDs (Blue, Yellow, Red, White) with 220Ω resistors

### Input (5)
- 4× Push Buttons
- LCD1602 Display (I2C)

### Main Board
- Arduino Uno R4 WiFi
- Full-size Breadboard + wiring

### Power
- 9V Battery Cap (optional)
- USB-C Programming Cable
- Multiple jumper wire types

## Quick Start

### 1. Prerequisites
- Arduino IDE 2.0+
- Arduino R4 WiFi board support installed
- Home Assistant instance with MQTT broker

### 2. Install Libraries
```
Arduino IDE → Sketch → Include Library → Manage Libraries

Search and install:
- PubSubClient (by Nick O'Leary)
- Adafruit BMP280 (by Adafruit)
- DHT sensor library (by Adafruit)
- MPU6050 (by Electronic Cats)
- LiquidCrystal I2C (by Frank de Brabander)
```

### 3. Configure Settings
Edit `arduino/IoT_Home_Assistant.ino`:
```cpp
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const char* mqtt_server = "192.168.1.X";  // Home Assistant IP
const char* mqtt_user = "homeassistant";
const char* mqtt_password = "YOUR_MQTT_PASSWORD";
```

### 4. Wire Components
Follow detailed wiring guide in `docs/WIRING_GUIDE.md`

### 5. Upload to Arduino
- Connect Arduino R4 via USB-C
- Select Board: Arduino Uno R4 WiFi
- Select correct COM port
- Click Upload

### 6. Monitor in Home Assistant
- Check MQTT integration
- Entities should auto-discover on `home/arduino/` topic
- Create automations as needed

## File Structure

```
IoT Home/
├── arduino/
│   └── IoT_Home_Assistant.ino     # Main sketch
├── config/
│   └── home_assistant.yaml        # HA configuration
├── docs/
│   ├── WIRING_GUIDE.md           # Breadboard layout & connections
│   ├── RECOMMENDED_UPGRADES.md   # Component suggestions
│   └── MQTT_TOPICS.md            # Topic reference
├── src/
│   └── (Python support scripts)
└── README.md                       # This file
```

## MQTT Topic Structure

### Sensor Publishing
```
home/arduino/sensor/temperature      # Float: °C
home/arduino/sensor/humidity         # Float: %
home/arduino/sensor/pressure         # Float: hPa
home/arduino/sensor/altitude         # Float: m
home/arduino/sensor/distance         # Int: cm
home/arduino/sensor/soil_moisture    # Int: 0-1023
home/arduino/sensor/sound_level      # Int: 0-1023
home/arduino/sensor/rain             # String: "wet" or "dry"
```

### Control Subscriptions
```
home/arduino/control/relay1          # "ON" or "OFF"
home/arduino/control/relay2          # "ON" or "OFF"
home/arduino/control/buzzer          # "ON" or "OFF"
home/arduino/control/led             # Color code (optional)
```

### Button Events
```
home/arduino/button/button1          # "pressed"
home/arduino/button/button2          # "pressed"
home/arduino/button/button3          # "pressed"
home/arduino/button/button4          # "pressed"
```

## Home Assistant Integration

### Auto-Discovery (Recommended)
Entities auto-discover on startup. View in:
- Settings → Devices & Services → MQTT

### Manual Setup
Add to `configuration.yaml` (see `config/home_assistant.yaml` for full template):
```yaml
mqtt:
  broker: 192.168.1.X
  port: 1883
  username: homeassistant
  password: mqtt_password

sensor:
  - platform: mqtt
    name: "Arduino Temperature"
    state_topic: "home/arduino/sensor/temperature"
    unit_of_measurement: "°C"
```

## Pin Configuration Reference

| Pin | Component | Type |
|-----|-----------|------|
| GPIO 2 | DHT22 | I/O |
| GPIO 3 | Buzzer | Output |
| GPIO 4 | Ultrasonic TRIG | Output |
| GPIO 5 | Ultrasonic ECHO | Input |
| GPIO 6-7 | Relay Module | Output |
| GPIO 8 | Servo | PWM Output |
| GPIO 9-12 | Push Buttons | Input |
| GPIO 22-25 | LEDs | Output |
| A0 | Sound Sensor | Analog Input |
| A1 | Soil Moisture | Analog Input |
| A2 | Rain Sensor | Analog Input |
| A3 | Capacitive Touch | Analog Input |
| A4 (SDA) | I2C Devices | I2C |
| A5 (SCL) | I2C Devices | I2C |

## Features Implemented

✅ Multi-sensor data collection
✅ WiFi connectivity with error handling
✅ MQTT pub/sub for Home Assistant
✅ LCD real-time display
✅ Push button input (debounced)
✅ LED indicators
✅ Relay control
✅ Servo motor control
✅ I2C device support
✅ Analog sensor reading

## Troubleshooting

### Arduino Won't Upload
- Check USB-C cable connection
- Verify board selection: "Arduino Uno R4 WiFi"
- Try different USB port
- Hold reset button while uploading

### WiFi Won't Connect
- Verify SSID/password in code
- Check WiFi network visible from Arduino
- Monitor serial output for auth errors
- Restart Arduino with power cycle

### Sensors Not Reading
- Check I2C pull-up resistors (4.7kΩ) installed
- Verify sensor addresses match code comments
- Use I2C scanner sketch to verify addresses
- Check SDA/SCL connections to A4/A5

### MQTT Not Publishing
- Verify broker IP and port correct
- Check MQTT broker is running
- Monitor with: `mosquitto_sub -h 192.168.1.X -u homeassistant -P password -t "home/arduino/#"`
- Check firewall allows MQTT port (1883)

### LCD Not Displaying
- Verify I2C address (0x27 or 0x3F)
- Check contrast adjustment pot
- Test with I2C scanner
- Confirm 5V power supply adequate

## Performance Specs

- **Update Interval**: 10 seconds (configurable)
- **Sensor Accuracy**:
  - DHT22: ±0.5°C, ±2% RH
  - BMP280: ±1 hPa
  - MPU6050: ±2°
  - Ultrasonic: ±2cm
- **WiFi Range**: 50-100m (depending on obstacles)
- **Power Usage**: ~200mA idle, ~500mA peak
- **Processing Speed**: Reads all sensors < 500ms

## Advanced Customization

### Change Update Interval
```cpp
const unsigned long PUBLISH_INTERVAL = 10000; // milliseconds
```

### Add New Sensors
1. Define pin in pin configuration section
2. Add read function
3. Add to `readAllSensors()`
4. Add MQTT publish call in `publishSensorData()`
5. Create Home Assistant entity

### Custom Automations
Examples in `config/home_assistant.yaml`:
- High temperature alert
- Rain detection notification
- Low soil moisture warning
- High noise level response

## Data Logging

To store sensor history:
1. Enable InfluxDB integration in Home Assistant
2. Or use History Stats cards for trends
3. Export CSV via Developer Tools

## Security Considerations

⚠️ **Before deployment**:
- [ ] Change default MQTT credentials
- [ ] Use strong WiFi password
- [ ] Enable MQTT authentication
- [ ] Restrict MQTT topic access if possible
- [ ] Update Arduino firmware regularly

## Future Enhancements

- [ ] SD card data logging
- [ ] Mobile app integration
- [ ] Anomaly detection
- [ ] Local forecasting
- [ ] Advanced automation rules
- [ ] Voice control (Alexa/Google Home)
- [ ] Backup power management
- [ ] Over-the-air firmware updates

## Support & Resources

- [Arduino R4 Documentation](https://docs.arduino.cc/hardware/uno-r4-wifi/)
- [Home Assistant MQTT Integration](https://www.home-assistant.io/integrations/mqtt/)
- [PubSubClient Library](https://github.com/knolleary/pubsub_client)
- [Adafruit Sensor Libraries](https://github.com/adafruit)

## License

This project is provided as-is for educational and personal use.


## Contributing

To improve this project:
1. Test on your hardware
2. Document any issues
3. Suggest enhancements
4. Share working automations

---

**Last Updated**: November 2025
**Arduino IDE Version**: 2.0+
**Board**: Arduino Uno R4 WiFi

## Hardware / Tested Kit

This project was developed and tested with the GeeekPi Cloud-Ready IoT kit based around the Arduino UNO R4 WiFi board (example listing: https://www.ebay.co.uk/itm/226084953370). The GeeekPi kit typically includes the UNO R4 WiFi board plus common sensors and modules (DHT22, BMP280, MPU6050, HC-SR04, soil sensor, sound sensor, rain sensor, LCD, relays and small accessories). This repository provides code, wiring guides and Home Assistant configuration intended to work with that kit.

Key points about the GeeekPi kit
- Includes a pre-selected sensor set and wiring examples so you can assemble a working IoT node quickly.
- The Arduino Uno R4 WiFi uses FQBN: `arduino:renesas_uno:unor4wifi` in `arduino-cli` / Arduino IDE.
- This repository is a companion resource for that kit: follow `docs/QUICK_START.md` and `docs/WIRING_GUIDE.md` to get the kit running.

Notes
- When the Arduino is not connected, repository-level checks (CI, Docker services) still run; hardware upload steps must be performed on a machine with the device attached.
- The example kit listing above is for convenience — kit contents and sellers may change; use the wiring guide here as the authoritative reference for pin wiring and sensor connections.

See also:
- `mqtt/README.md` — instructions for generating the Mosquitto `passwordfile` from your local `secrets.h` and important security notes (do not commit the generated `passwordfile`).

