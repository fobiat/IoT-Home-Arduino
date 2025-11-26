# Sensor Calibration & Troubleshooting Guide

## Sensor-by-Sensor Calibration

### 1. DHT22 (Temperature & Humidity)

**Expected Values:**
- Temperature: 15-35°C (room conditions)
- Humidity: 30-80% (indoor)

**Calibration Procedure:**
```cpp
// Test sketch - print raw values
void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print(" Humidity: ");
  Serial.println(h);
  
  delay(2000);
}
```

**Troubleshooting:**
- Shows NaN: DHT not connected, bad wire, need 10kΩ pull-up
- Consistently +2°C high: Normal (DHT has ~±2°C error)
- Erratic readings: Sensor too close to heat source
- Always "0%": Check DHT address (should be on pin 2)

**Offset Correction:**
```cpp
float temp_offset = -2.0;  // Adjust based on comparison
sensorData.temperature = dht.readTemperature() + temp_offset;
```

---

### 2. BMP280 (Pressure & Altitude)

**Expected Values:**
- Pressure: 950-1050 hPa (sea level variation)
- Altitude: 0m (sea level) or local elevation

**Calibration Procedure:**

1. **Find Local Barometric Pressure:**
   - Check weather station (e.g., NOAA, weather.com)
   - Record: `seaLevelPressure`

2. **Run Calibration:**
```cpp
#include <Adafruit_BMP280.h>

void setup() {
  Serial.begin(115200);
  if (!bmp280.begin(0x76)) Serial.println("BMP280 not found!");
}

void loop() {
  Serial.print("Pressure: ");
  Serial.print(bmp280.readPressure() / 100.0);
  Serial.print(" hPa | Altitude: ");
  Serial.println(bmp280.readAltitude(1013.25));  // Use known sea level
  
  delay(1000);
}
```

3. **Update Altitude Reference:**
```cpp
float localSeaLevelPressure = 1013.25;  // Your local value
sensorData.altitude = bmp280.readAltitude(localSeaLevelPressure);
```

**Troubleshooting:**
- Altitude way off: Wrong sea level pressure
- Address not found: Check 0x76 (some clones use 0x77)
- I2C errors: Verify pull-up resistors and wiring

---

### 3. MPU6050 (Gyroscope & Accelerometer)

**Expected Values:**
- Accel: ±16384 LSB/g (when stationary: Z ≈ 16384, X/Y ≈ 0)
- Gyro: ±131 LSB/°/sec (when still: all ≈ 0)

**Calibration Procedure:**

1. **Zero-point Calibration:**
```cpp
int16_t accel_x_offset = 0;
int16_t accel_y_offset = 0;
int16_t accel_z_offset = 0;

void calibrateMPU6050() {
  int16_t ax, ay, az;
  
  // Place MPU6050 flat on table, no movement
  Serial.println("Calibrating... Keep still!");
  
  int32_t sum_x = 0, sum_y = 0, sum_z = 0;
  for (int i = 0; i < 200; i++) {
    mpu6050.getAcceleration(&ax, &ay, &az);
    sum_x += ax;
    sum_y += ay;
    sum_z += (az - 16384);  // 16384 is 1G
    delay(10);
  }
  
  accel_x_offset = sum_x / 200;
  accel_y_offset = sum_y / 200;
  accel_z_offset = sum_z / 200;
  
  Serial.println("Calibration complete!");
}
```

2. **Apply Offsets in Read Function:**
```cpp
int16_t ax, ay, az, gx, gy, gz;
mpu6050.getAcceleration(&ax, &ay, &az);

ax -= accel_x_offset;
ay -= accel_y_offset;
az -= accel_z_offset;
```

**Troubleshooting:**
- Address error: Should be 0x68, some clones use 0x69
- No response: Check I2C wiring, power supply
- Gyro drift: Normal - requires periodic recalibration

---

### 4. HC-SR04 Ultrasonic (Distance)

**Expected Range:** 2cm - 400cm

**Calibration Procedure:**

1. **Measure Known Distances:**
```cpp
void calibrateUltrasonic() {
  for (int distance = 10; distance <= 100; distance += 10) {
    Serial.print("Place object at ");
    Serial.print(distance);
    Serial.println("cm, press button...");
    
    while (digitalRead(BUTTON1) == HIGH);  // Wait for button
    delay(500);
    
    int measured = getUltrasonicDistance();
    Serial.print("Measured: ");
    Serial.print(measured);
    Serial.print(" (Error: ");
    Serial.print(measured - distance);
    Serial.println("cm)");
    
    delay(1000);
  }
}
```

2. **Typical Corrections:**
- Add 0.5-1cm offset (sensor has small offset)
- Filter noisy readings with moving average

**Troubleshooting:**
- Reading 0 or max: Check trigger/echo wires
- Unstable readings: Add 100nF capacitor to power
- Slow response: Increase trigger pulse width

---

### 5. Soil Moisture Sensor

**Expected Values:** 0-1023 (10-bit ADC)
- Dry soil: 700-1023
- Moist soil: 400-700
- Wet soil: 0-400

**Calibration Procedure:**

1. **Create Calibration Data:**
```cpp
void calibrateSoil() {
  Serial.println("Dry calibration - remove sensor from water for 30s");
  delay(30000);
  int dry_value = analogRead(SOIL_MOISTURE_PIN);
  
  Serial.println("Wet calibration - place in water");
  delay(10000);
  int wet_value = analogRead(SOIL_MOISTURE_PIN);
  
  Serial.print("Dry: ");
  Serial.print(dry_value);
  Serial.print(" | Wet: ");
  Serial.println(wet_value);
}
```

2. **Map to Percentage:**
```cpp
int getMoisturePercent() {
  int raw = analogRead(SOIL_MOISTURE_PIN);
  int dry_cal = 750;    // Adjust based on calibration
  int wet_cal = 200;    // Adjust based on calibration
  
  int percent = map(raw, dry_cal, wet_cal, 0, 100);
  return constrain(percent, 0, 100);
}
```

**Troubleshooting:**
- Always reads high: Sensor not in soil
- Noisy readings: Add 100nF capacitor to analog input
- Not responding to moisture: Check sensor pins not corroded

---

### 6. Sound Sensor

**Expected Values:** 0-1023 (10-bit ADC)
- Quiet room: 0-200
- Normal speech: 200-500
- Loud noise: 500-1023

**Calibration Procedure:**

1. **Establish Baseline:**
```cpp
void calibrateSound() {
  Serial.println("Measuring noise floor (quiet for 10s)...");
  int baseline = 0;
  for (int i = 0; i < 100; i++) {
    baseline += analogRead(SOUND_SENSOR_PIN);
    delay(100);
  }
  baseline /= 100;
  
  Serial.print("Noise floor: ");
  Serial.println(baseline);
  
  #define SOUND_THRESHOLD (baseline + 50)
}
```

2. **Noise Detection Logic:**
```cpp
int threshold = 250;  // Adjust after calibration
if (analogRead(SOUND_SENSOR_PIN) > threshold) {
  Serial.println("Noise detected!");
}
```

**Troubleshooting:**
- No response: Check mic isn't muted
- Always high: Sensor orientation (mic facing noise source)
- Erratic: Needs low-pass filter

---

### 7. Rain Drop Sensor

**Expected Values:**
- Dry: HIGH (1023)
- Wet: LOW (0-500)

**Calibration Procedure:**

```cpp
void calibrateRain() {
  Serial.print("Dry reading: ");
  Serial.println(analogRead(RAIN_SENSOR_PIN));
  
  // Spray water on sensor
  delay(2000);
  
  Serial.print("Wet reading: ");
  Serial.println(analogRead(RAIN_SENSOR_PIN));
  
  // Set threshold midway
  #define RAIN_THRESHOLD 500
}
```

**Troubleshooting:**
- Stuck at one value: Check wiring
- Corroding contacts: Apply thin oil coating
- False positives: Increase threshold

---

### 8. Capacitive Touch Sensor

**Expected Values:**
- Not touched: ~0-50 ADC
- Touched: 200-1023 ADC

**Calibration:**
```cpp
void calibrateTouch() {
  Serial.println("Not touching sensor...");
  int untouched = 0;
  for (int i = 0; i < 100; i++) {
    untouched += analogRead(TOUCH_SENSOR_PIN);
    delay(50);
  }
  untouched /= 100;
  
  Serial.println("Now touch sensor...");
  delay(3000);
  int touched = 0;
  for (int i = 0; i < 100; i++) {
    touched += analogRead(TOUCH_SENSOR_PIN);
    delay(50);
  }
  touched /= 100;
  
  int threshold = (untouched + touched) / 2;
  Serial.print("Touch threshold: ");
  Serial.println(threshold);
}
```

---

## Multi-Sensor Calibration Script

```cpp
// Complete calibration sketch
// Upload and check Serial Monitor

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  Serial.println("\n=== SENSOR CALIBRATION TOOL ===\n");
  
  displayMenu();
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    
    switch (cmd) {
      case '1': calibrateDHT22(); break;
      case '2': calibrateBMP280(); break;
      case '3': calibrateMPU6050(); break;
      case '4': calibrateUltrasonic(); break;
      case '5': calibrateSoil(); break;
      case '6': calibrateSound(); break;
      case '7': calibrateRain(); break;
      case '8': displayMenu(); break;
      default: Serial.println("Unknown command");
    }
  }
}

void displayMenu() {
  Serial.println("\nSELECT SENSOR TO CALIBRATE:");
  Serial.println("1) DHT22 (Temperature/Humidity)");
  Serial.println("2) BMP280 (Pressure)");
  Serial.println("3) MPU6050 (Accel/Gyro)");
  Serial.println("4) Ultrasonic (Distance)");
  Serial.println("5) Soil Moisture");
  Serial.println("6) Sound Level");
  Serial.println("7) Rain Sensor");
  Serial.println("8) Show Menu");
}

// [Include individual calibration functions here]
```

---

## Post-Calibration Testing

After calibrating all sensors:

```cpp
void testAllSensors() {
  Serial.println("\n=== SENSOR TEST REPORT ===\n");
  
  readAllSensors();
  
  Serial.print("Temperature: ");
  Serial.print(sensorData.temperature);
  Serial.println("°C [Expect: 15-30]");
  
  Serial.print("Humidity: ");
  Serial.print(sensorData.humidity);
  Serial.println("% [Expect: 20-80]");
  
  Serial.print("Pressure: ");
  Serial.print(sensorData.pressure);
  Serial.println("hPa [Expect: 950-1050]");
  
  Serial.print("Distance: ");
  Serial.print(sensorData.ultrasonic_distance);
  Serial.println("cm [Expect: 0-400]");
  
  Serial.print("Soil: ");
  Serial.print(sensorData.soil_moisture);
  Serial.println(" [Expect: 200-800]");
  
  Serial.print("Sound: ");
  Serial.print(sensorData.sound_level);
  Serial.println(" [Expect: 0-1023]");
  
  Serial.print("Rain: ");
  Serial.println(sensorData.rain_detected ? "WET" : "DRY");
}
```

---

## Common Issues & Solutions

| Issue | Likely Cause | Solution |
|-------|--------------|----------|
| Sensor not detected | Wrong I2C address | Use I2C scanner |
| NaN readings | Loose wires | Reseat all connections |
| Noisy data | ADC interference | Add capacitor to power |
| Wrong values | Uncalibrated | Run calibration procedure |
| Intermittent failures | Weak connections | Use Dupont connectors |
| High offset error | Temperature drift | Recalibrate when warm |

---

## Keeping Calibration Data

Save calibration values in EEPROM:

```cpp
#include <EEPROM.h>

struct CalibrationData {
  float bmp280_offset;
  int soil_dry_cal;
  int soil_wet_cal;
  int sound_threshold;
};

void saveCalibration(CalibrationData cal) {
  EEPROM.put(0, cal);
}

void loadCalibration(CalibrationData &cal) {
  EEPROM.get(0, cal);
}
```

This ensures calibration persists after reboot!
