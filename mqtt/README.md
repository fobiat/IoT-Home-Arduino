# Mosquitto configuration and passwordfile

This folder contains configuration and helper scripts for running a local Mosquitto MQTT broker used for development and testing.

Important notes
- Do NOT commit `mqtt/config/passwordfile` to the repository. It contains authentication data and is deliberately not tracked.
- Keep `IoT_Home_Assistant/secrets.h` outside version control (it is in `.gitignore`).

Generating the password file

This repository includes a helper script to generate a Mosquitto passwordfile from the local `secrets.h` file:

```bash
# From repository root
./scripts/generate_mosquitto_password.sh
```

How the script works
- It reads `MQTT_USER` and `MQTT_PASSWORD` from `IoT_Home_Assistant/secrets.h`.
- It requires `mosquitto_passwd` to be installed on the host. The script will error if that tool is not present.
- It writes `mqtt/config/passwordfile` and sets restrictive permissions. The script will restart the Mosquitto container so the new password file is loaded.

Installing `mosquitto_passwd`
- Debian/Ubuntu: `sudo apt-get update && sudo apt-get install -y mosquitto-clients`
- Arch Linux: `sudo pacman -S mosquitto`

Security
- The password file should be owned by `root` and have permissions `640` (the included script attempts to set these).
- Do not add `mqtt/config/passwordfile` to version control. If you accidentally commit credentials, rotate the password and remove the commit from history.
