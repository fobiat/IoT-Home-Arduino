# IoT_Home_Assistant (Arduino sketch)

This folder contains the Arduino sketch `IoT_Home_Assistant` and its C++ implementation in `src/`.

Local build & upload (using `arduino-cli`)

1. Ensure `arduino-cli` is installed and in your `PATH`.

2. Compile the sketch for Arduino UNO R4 WiFi (FQBN `arduino:renesas_uno:unor4wifi`):

```bash
cd 'IoT_Home_Assistant'
arduino-cli core update-index
arduino-cli core install arduino:renesas_uno
arduino-cli lib update-index
# Install required libs (run once)
arduino-cli lib install PubSubClient "Adafruit BMP280 Library" "Adafruit Unified Sensor" "DHT sensor library" MPU6050 "LiquidCrystal I2C"

# Compile
arduino-cli compile --fqbn arduino:renesas_uno:unor4wifi .
```

3. Upload to a connected board (example port `/dev/ttyACM0`):

```bash
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:renesas_uno:unor4wifi .
```

4. Monitor serial output at 115200 baud:

```bash
arduino-cli monitor -p /dev/ttyACM0 -b 115200
```

Notes
- The sketch prints `Booting IoT Home Assistant...` at startup.
- If you change libraries or board, update the FQBN accordingly.

Secrets (WiFi / MQTT)

- There is a `secrets.example.h` file in this folder. Copy it to `secrets.h` and fill in your credentials:

```bash
cd IoT_Home_Assistant
cp secrets.example.h secrets.h
# edit secrets.h to add your SSID/MQTT credentials
```

- `secrets.h` is ignored by the repository (`.gitignore`) so your private credentials will not be published.

- If you prefer not to keep credentials in a file, you can also inject them at build time or use a hardware secure element.
