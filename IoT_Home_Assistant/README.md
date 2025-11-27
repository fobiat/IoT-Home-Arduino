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

CI / GitHub Actions

- The repository GitHub Actions workflow can build the sketch using repository secrets. To enable CI builds that use your credentials, add the following repository secrets under Settings → Secrets & variables → Actions:

	- `WIFI_SSID`
	- `WIFI_PASSWORD`
	- `MQTT_SERVER`
	- `MQTT_PORT`
	- `MQTT_USER`
	- `MQTT_PASSWORD`

	The Action will create a `secrets.h` file at build time from those secrets so credentials are never stored in the repository.

Local safety: pre-commit hook

- The repository contains a sample pre-commit hook in `.githooks/pre-commit` that prevents committing `secrets.h`. To enable it locally run:

```bash
git config core.hooksPath .githooks

Or run the provided script from the repo root to install hooks for this checkout:

```bash
./scripts/install-hooks.sh
```

This will make the pre-commit hook active locally and prevent accidental commits of `secrets.h`.
```

This will make Git run the included hook locally, further protecting against accidental commits of private files.
