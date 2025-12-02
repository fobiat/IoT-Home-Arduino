````markdown
# ELI5 — Getting Started with the IoT Home Kit

This guide is written for absolute beginners. No prior hardware or Home Assistant experience required. Follow these steps slowly and you'll have your first sensor reading and a working dashboard in Home Assistant.

## What's happening (very short)
- The Arduino Uno R4 WiFi runs small C++ code that reads sensors and sends the measurements to an MQTT broker (a lightweight message server).
- Home Assistant subscribes to those MQTT messages and displays them as sensors on a dashboard.
- `secrets.h` holds your WiFi and MQTT credentials so they are not stored publicly in the repo.

## What you need
- The GeeekPi Cloud-Ready IoT kit (or equivalent) with an Arduino Uno R4 WiFi and sensors.
- A USB-C cable to connect the Arduino to your computer.
- A computer with the Arduino IDE (or `arduino-cli`) installed.
- (Optional) A Home Assistant instance on the same network, or run it locally with Docker Compose.

## Step-by-step (super simple)

1) Unbox and identify parts
- Find the Arduino board, breadboard, jumper wires, and the small sensor modules (DHT22, BMP280, HC-SR04, etc.).
- The kit usually includes a tiny manual — skip it for now and follow this guide.

2) Make a safe workspace
- Work on a flat table with good light. Keep water and food away from the electronics.

3) Copy the secrets file (important)
- In the repo there's a safe template: `IoT_Home_Assistant/secrets.example.h`.
- Copy it locally and edit it with your WiFi and MQTT details:

```bash
cp IoT_Home_Assistant/secrets.example.h IoT_Home_Assistant/secrets.h
# Edit IoT_Home_Assistant/secrets.h and replace placeholder values
```

- Why? `secrets.h` is ignored by git so your network password and MQTT password won't be uploaded to GitHub.
- If you want to avoid editing files on your computer, see the CI notes in the README — the GitHub Actions workflow can inject secrets at build time if you add them to the repository's Secrets.

4) Wire a minimal set of parts (start small)
- Connect only these first, then add others once you have a working system:
  - DHT22: VCC → 5V, GND → GND, DATA → GPIO2
  - BMP280: VCC → 3.3V, GND → GND, SDA → A4, SCL → A5
  - LCD1602 (I2C): VCC → 5V, GND → GND, SDA → A4, SCL → A5

5) Open the sketch and upload
- Open `arduino/IoT_Home_Assistant.ino` in the Arduino IDE.
- Select board: `Arduino Uno R4 WiFi`.
- Select the USB port for your board.
- Click Upload. Wait for the status message "Done uploading".

6) Monitor the serial output
- Open Tools → Serial Monitor and set baud to `115200`.
- You should see boot messages like:
  - `Booting IoT Home Assistant...`
  - `WiFi connected!` and an IP address
  - `Attempting MQTT connection...`

7) Start Home Assistant (if you haven't yet)
- If you run Home Assistant in Docker Compose, from the repo root:

```bash
docker compose -f docker-compose.homeassistant.yml -f docker-compose.override.yml up -d
```

8) Secure MQTT (local test)
- The repo includes `scripts/generate_mosquitto_password.sh` which reads `IoT_Home_Assistant/secrets.h` and creates a hashed `mqtt/config/passwordfile` for the Mosquitto container.
- Run that script before starting the Mosquitto container:

```bash
./scripts/generate_mosquitto_password.sh
```

9) Verify in Home Assistant
- Settings → Devices & Services → MQTT
- New devices (the Arduino) should appear automatically if discovery is enabled in the sketch.
- Or use a quick MQTT subscriber to view messages:

```bash
mosquitto_sub -h <HOME_ASSISTANT_IP> -u <MQTT_USER> -P <MQTT_PASSWORD> -t "home/arduino/#"
```

10) If something goes wrong
- No WiFi: double-check SSID/password in `secrets.h` and monitor serial output.
- No MQTT: check the Mosquitto logs and ensure the `passwordfile` was generated and mounted into the container.
- Sensors show zero: check wiring and if the device uses 3.3V vs 5V.

## Extra beginner tips
- Work iteratively: wire one sensor, confirm its readings, then add another.
- Keep a notebook of which wire goes where — photos help.
- If you're not sure, ask in the Home Assistant community or the Arduino forum (links in the main README).

## Next steps
- Once comfortable, follow `docs/QUICK_START.md` for more advanced wiring and automation examples.

Happy building! If you'd like, I can add labeled photos for each wiring step to this guide.

````
