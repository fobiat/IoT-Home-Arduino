# MQTT Topic Reference

## All MQTT Topics Used

### Publishing (Arduino → Home Assistant)

**Environmental Sensors**
- `home/arduino/sensor/temperature` - Float, °C
- `home/arduino/sensor/humidity` - Float, %
- `home/arduino/sensor/pressure` - Float, hPa
- `home/arduino/sensor/altitude` - Float, m

**Movement & Distance**
- `home/arduino/sensor/distance` - Integer, cm
- `home/arduino/sensor/accel_x` - Integer, m/s²
- `home/arduino/sensor/accel_y` - Integer, m/s²
- `home/arduino/sensor/accel_z` - Integer, m/s²
- `home/arduino/sensor/gyro_x` - Integer, °/s
- `home/arduino/sensor/gyro_y` - Integer, °/s
- `home/arduino/sensor/gyro_z` - Integer, °/s

**Plant/Soil Sensors**
- `home/arduino/sensor/soil_moisture` - Integer, 0-1023
- `home/arduino/sensor/rain` - String, "wet" or "dry"

**Audio**
- `home/arduino/sensor/sound_level` - Integer, 0-1023

**Button Events**
- `home/arduino/button/button1` - String, "pressed"
- `home/arduino/button/button2` - String, "pressed"
- `home/arduino/button/button3` - String, "pressed"
- `home/arduino/button/button4` - String, "pressed"

### Subscribing (Home Assistant → Arduino)

**Control Topics**
- `home/arduino/control/relay1` - "ON" or "OFF"
- `home/arduino/control/relay2` - "ON" or "OFF"
- `home/arduino/control/buzzer` - "ON" or "OFF"
- `home/arduino/control/servo` - Integer, 0-180 (future)
- `home/arduino/control/led` - Color code (future)

## Payload Examples

### Sensor Data
```
home/arduino/sensor/temperature: 23.45
home/arduino/sensor/humidity: 55.20
home/arduino/sensor/pressure: 1013.25
home/arduino/sensor/distance: 15
```

### Control Commands
```
home/arduino/control/relay1: ON
home/arduino/control/relay1: OFF
home/arduino/control/buzzer: ON
```

### Events
```
home/arduino/button/button1: pressed
home/arduino/sensor/rain: wet
```

## Testing Topics

Use MQTT Explorer or mosquitto_sub to monitor:

```bash
# Subscribe to all Arduino topics
mosquitto_sub -h 192.168.1.X -u homeassistant -P password -t "home/arduino/#"

# Subscribe to specific sensor
mosquitto_sub -h 192.168.1.X -u homeassistant -P password -t "home/arduino/sensor/temperature"

# Publish test command
mosquitto_pub -h 192.168.1.X -u homeassistant -P password -t "home/arduino/control/relay1" -m "ON"
```

## Home Assistant Template Sensors

For calculated values, add to `configuration.yaml`:

```yaml
template:
  - sensor:
      - name: "Arduino Heat Index"
        unit_of_measurement: "°C"
        state: >
          {% set t = states('sensor.arduino_temperature') | float %}
          {% set h = states('sensor.arduino_humidity') | float %}
          {% set c1 = 0.5555 %}
          {% set c2 = 4.8455 %}
          {% set c3 = 1.20672 %}
          {% set c4 = 0.22469 %}
          {% set c5 = 0.00426 %}
          {{ (c1*t + c2 + c3*h - c4*h*c5*t) | round(1) }}

      - name: "Arduino Dew Point"
        unit_of_measurement: "°C"
        state: >
          {% set t = states('sensor.arduino_temperature') | float %}
          {% set h = states('sensor.arduino_humidity') | float %}
          {% set a = 17.27 %}
          {% set b = 237.7 %}
          {% set alpha = ((a*t) / (b + t)) + log(h/100) %}
          {{ ((b * alpha) / (a - alpha)) | round(1) }}

      - name: "Arduino Plant Status"
        state: >
          {% set soil = states('sensor.arduino_soil_moisture') | int %}
          {% if soil < 200 %} Dry
          {% elif soil < 400 %} Needs Water
          {% elif soil < 700 %} Moist
          {% else %} Too Wet
          {% endif %}
```

## Automation Trigger Examples

```yaml
automation:
  # Trigger on temperature value
  - alias: "Temperature Above 25"
    trigger:
      platform: numeric_state
      entity_id: sensor.arduino_temperature
      above: 25
    action:
      service: notify.email
      data:
        message: "Temperature is high"

  # Trigger on MQTT message
  - alias: "Button Pressed"
    trigger:
      platform: mqtt
      topic: "home/arduino/button/button1"
      payload: "pressed"
    action:
      service: light.toggle
      target:
        entity_id: light.living_room

  # Trigger on sensor state
  - alias: "Rain Detected"
    trigger:
      platform: state
      entity_id: sensor.arduino_rain_status
      to: "wet"
    action:
      service: notify.mobile_app_iphone
      data:
        message: "Rain detected!"
```

## Performance Tips

1. **Reduce publish frequency** if MQTT broker is overloaded
2. **Use state_class: measurement** for graphs in HA
3. **Archive old data** to InfluxDB for long-term storage
4. **Group automations** to reduce trigger overhead
5. **Use templates sparingly** on high-frequency sensors

## Troubleshooting MQTT

**Topics not appearing in Home Assistant:**
```bash
# Check broker is receiving data
mosquitto_sub -h 192.168.1.X -u homeassistant -P password -t "home/arduino/#" -v

# Verify Home Assistant MQTT integration enabled
# Settings → Devices & Services → MQTT
```

**Publish not working:**
- Check topic name exactly matches
- Verify connection to broker
- Monitor with `mosquitto_sub` to see if message arrives
- Check Arduino serial output for MQTT state

**Delayed updates:**
- Increase `PUBLISH_INTERVAL` if too frequent
- Check WiFi signal strength
- Monitor broker CPU/memory usage
