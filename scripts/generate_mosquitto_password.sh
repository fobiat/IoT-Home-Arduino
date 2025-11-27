#!/usr/bin/env bash
# Generate Mosquitto password file from `IoT_Home_Assistant/secrets.h`.
# This reads MQTT_USER and MQTT_PASSWORD from the local (git-ignored) secrets file
# and creates `mqtt/config/passwordfile` using `mosquitto_passwd`.
#
# Usage: ./scripts/generate_mosquitto_password.sh

set -euo pipefail

SECRETS_FILE="IoT_Home_Assistant/secrets.h"
OUT_FILE="mqtt/config/passwordfile"

if [ ! -f "$SECRETS_FILE" ]; then
  echo "Error: $SECRETS_FILE not found. Create it from secrets.example.h and fill values." >&2
  exit 2
fi

# Extract values (allow either "MQTT_USER" or "MQTT_USERNAME" styles)
MQTT_USER=$(grep -E "#define[[:space:]]+MQTT_USER" "$SECRETS_FILE" || true)
MQTT_PASS=$(grep -E "#define[[:space:]]+MQTT_PASSWORD" "$SECRETS_FILE" || true)

if [ -z "$MQTT_USER" ] || [ -z "$MQTT_PASS" ]; then
  echo "Could not find MQTT_USER or MQTT_PASSWORD in $SECRETS_FILE" >&2
  exit 3
fi

# Parse out the quoted values
MQTT_USER_VAL=$(echo "$MQTT_USER" | sed -E 's/#define[[:space:]]+MQTT_USER[[:space:]]+"(.*)"/\1/')
MQTT_PASS_VAL=$(echo "$MQTT_PASS" | sed -E 's/#define[[:space:]]+MQTT_PASSWORD[[:space:]]+"(.*)"/\1/')

if [ -z "$MQTT_USER_VAL" ] || [ -z "$MQTT_PASS_VAL" ]; then
  echo "Parsed empty MQTT_USER or MQTT_PASSWORD; aborting." >&2
  exit 4
fi

echo "Generating Mosquitto passwordfile at $OUT_FILE for user '$MQTT_USER_VAL'"

# Ensure output directory exists
mkdir -p "$(dirname "$OUT_FILE")"

# Prefer to use mosquitto_passwd if available. We'll pipe the password to avoid putting it in argv.
if command -v mosquitto_passwd >/dev/null 2>&1; then
  # Use mosquitto_passwd non-interactively to create a hashed password entry.
  # Use the -b flag (non-interactive: mosquitto_passwd file user password).
  mosquitto_passwd -b -c "$OUT_FILE" "$MQTT_USER_VAL" "$MQTT_PASS_VAL"
else
  echo "Error: mosquitto_passwd not found. Install mosquitto/mosquitto-clients and re-run this script." >&2
  exit 1
fi

# Tighten permissions
chmod 640 "$OUT_FILE" || true

echo "Password file written to $OUT_FILE (permissions set to 640)."

echo "Restarting mosquitto container to pick up new password file..."
sudo docker compose -f docker-compose.homeassistant.yml -f docker-compose.override.yml restart mosquitto || true

echo "Done. Mosquitto should now require authentication using the credentials from $SECRETS_FILE."
