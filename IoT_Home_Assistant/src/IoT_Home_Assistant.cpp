#include <Arduino.h>
#include <WiFiS3.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <DHT.h>
#include <MPU6050.h>
#include <LiquidCrystal_I2C.h>

// Forward declarations for functions used before their definitions
void initializeSensors();
void connectToWiFi();
void reconnectMQTT();
void mqttCallback(char* topic, byte* payload, unsigned int length);
void readAllSensors();
int getUltrasonicDistance();
void publishSensorData();
void updateDisplay();
void checkButtons();

// WiFi / MQTT Configuration
// Attempt to include a local `secrets.h` (created from `secrets.example.h`).
// This file is ignored by git so credentials remain private.
#if defined(__has_include)
#  if __has_include("secrets.h")
#    include "secrets.h"
#  endif
#endif

// Provide compile-time fallbacks if `secrets.h` is not present.
#ifndef WIFI_SSID
#define WIFI_SSID "YOUR_SSID"
#endif
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "YOUR_PASSWORD"
#endif
#ifndef MQTT_SERVER
#define MQTT_SERVER "192.168.1.X"
#endif
#ifndef MQTT_PORT
#define MQTT_PORT 1883
#endif
#ifndef MQTT_USER
#define MQTT_USER "homeassistant"
#endif
#ifndef MQTT_PASSWORD
#define MQTT_PASSWORD "YOUR_MQTT_PASSWORD"
#endif

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* mqtt_server = MQTT_SERVER;  // Home Assistant IP
const int mqtt_port = MQTT_PORT;
const char* mqtt_user = MQTT_USER;
const char* mqtt_password = MQTT_PASSWORD;

// MQTT Client
WiFiClient espClient;
PubSubClient client(espClient);

// Sensor Configuration
#define DHTPIN 2
#define DHT_TYPE DHT22
DHT dht(DHTPIN, DHT_TYPE);

Adafruit_BMP280 bmp280;
// MPU6050 sensor (use default constructor compatible with installed library)
MPU6050 mpu6050;

// I2C LCD Display (0x27 or 0x3F - adjust address if needed)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin Configuration
#define BUZZER_PIN 3
#define SOUND_SENSOR_PIN A0
#define SOIL_MOISTURE_PIN A1
#define RAIN_SENSOR_PIN A2
#define ULTRASONIC_TRIG 4
#define ULTRASONIC_ECHO 5
#define RELAY_PIN1 6
#define RELAY_PIN2 7
#define SERVO_PIN 8
#define BUTTON1 9
#define BUTTON2 10
#define BUTTON3 11
#define BUTTON4 12

// LED Pins
#define LED_BLUE 22
#define LED_YELLOW 23
#define LED_RED 24
#define LED_WHITE 25

// Map LED_GREEN to LED_WHITE to avoid undefined symbol
#define LED_GREEN LED_WHITE

// Touch sensor
#define TOUCH_SENSOR_PIN A3

// Variables for sensor readings
struct SensorData {
  float temperature;
  float humidity;
  float pressure;
  float altitude;
  int ultrasonic_distance;
  int soil_moisture;
  int rain_detected;
  int sound_level;
  int16_t accel_x, accel_y, accel_z;
  int16_t gyro_x, gyro_y, gyro_z;
} sensorData;

// Timing
unsigned long lastPublish = 0;
const unsigned long PUBLISH_INTERVAL = 10000; // 10 seconds

void app_setup() {
  Serial.begin(115200);
  Serial.println("Booting IoT Home Assistant...");
  delay(100);
  
  // Initialize pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_WHITE, OUTPUT);
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(ULTRASONIC_TRIG, OUTPUT);
  pinMode(ULTRASONIC_ECHO, INPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.print("IoT Home Init...");
  
  Serial.println("\nStarting Arduino IoT Home Assistant");
  
  // Initialize sensors
  initializeSensors();
  
  // Connect to WiFi
  connectToWiFi();
  
  // Setup MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);
  
  lcd.clear();
  lcd.print("Setup Complete!");
  delay(2000);
}

void app_loop() {
  // Ensure MQTT connection
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();
  
  // Read sensors periodically
  if (millis() - lastPublish > PUBLISH_INTERVAL) {
    readAllSensors();
    publishSensorData();
    updateDisplay();
    lastPublish = millis();
  }
  
  // Check buttons
  checkButtons();
  
  delay(100);
}

void initializeSensors() {
  // DHT22
  dht.begin();
  
  // BMP280
  if (!bmp280.begin(0x76)) {
    Serial.println("BMP280 not found!");
  }
  
  // MPU6050
  Wire.begin();
  mpu6050.initialize();
  if (!mpu6050.testConnection()) {
    Serial.println("MPU6050 not found!");
  }
  
  Serial.println("Sensors initialized");
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  int attempts = 0;
  
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Flash blue LED to indicate connection
    digitalWrite(LED_BLUE, HIGH);
    delay(500);
    digitalWrite(LED_BLUE, LOW);
  } else {
    Serial.println("\nFailed to connect to WiFi");
    digitalWrite(LED_RED, HIGH);
  }
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    if (client.connect("Arduino-R4", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      digitalWrite(LED_GREEN, HIGH);
      
      // Subscribe to control topics
      client.subscribe("home/arduino/control/relay1");
      client.subscribe("home/arduino/control/relay2");
      client.subscribe("home/arduino/control/buzzer");
      client.subscribe("home/arduino/control/led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);
  
  if (String(topic) == "home/arduino/control/relay1") {
    digitalWrite(RELAY_PIN1, message == "ON" ? HIGH : LOW);
  } else if (String(topic) == "home/arduino/control/relay2") {
    digitalWrite(RELAY_PIN2, message == "ON" ? HIGH : LOW);
  } else if (String(topic) == "home/arduino/control/buzzer") {
    if (message == "ON") {
      digitalWrite(BUZZER_PIN, HIGH);
    } else {
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}

void readAllSensors() {
  // DHT22 - Temperature & Humidity
  sensorData.humidity = dht.readHumidity();
  sensorData.temperature = dht.readTemperature();
  
  // BMP280 - Pressure & Altitude
  sensorData.pressure = bmp280.readPressure() / 100.0; // Convert to hPa
  sensorData.altitude = bmp280.readAltitude(1013.25);
  
  // MPU6050 - Gyroscope & Accelerometer
  mpu6050.getAcceleration(&sensorData.accel_x, &sensorData.accel_y, &sensorData.accel_z);
  mpu6050.getRotation(&sensorData.gyro_x, &sensorData.gyro_y, &sensorData.gyro_z);
  
  // Ultrasonic Distance
  sensorData.ultrasonic_distance = getUltrasonicDistance();
  
  // Soil Moisture
  sensorData.soil_moisture = analogRead(SOIL_MOISTURE_PIN);
  
  // Rain Sensor
  sensorData.rain_detected = digitalRead(RAIN_SENSOR_PIN);
  
  // Sound Level
  sensorData.sound_level = analogRead(SOUND_SENSOR_PIN);
}

int getUltrasonicDistance() {
  digitalWrite(ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG, LOW);
  
  long duration = pulseIn(ULTRASONIC_ECHO, HIGH);
  int distance = duration * 0.034 / 2; // Convert to cm
  
  return distance;
}

void publishSensorData() {
  char buffer[50];
  
  // Temperature
  dtostrf(sensorData.temperature, 5, 2, buffer);
  client.publish("home/arduino/sensor/temperature", buffer);
  
  // Humidity
  dtostrf(sensorData.humidity, 5, 2, buffer);
  client.publish("home/arduino/sensor/humidity", buffer);
  
  // Pressure
  dtostrf(sensorData.pressure, 7, 2, buffer);
  client.publish("home/arduino/sensor/pressure", buffer);
  
  // Altitude
  dtostrf(sensorData.altitude, 7, 2, buffer);
  client.publish("home/arduino/sensor/altitude", buffer);
  
  // Distance
  itoa(sensorData.ultrasonic_distance, buffer, 10);
  client.publish("home/arduino/sensor/distance", buffer);
  
  // Soil Moisture
  itoa(sensorData.soil_moisture, buffer, 10);
  client.publish("home/arduino/sensor/soil_moisture", buffer);
  
  // Sound Level
  itoa(sensorData.sound_level, buffer, 10);
  client.publish("home/arduino/sensor/sound_level", buffer);
  
  // Rain Detection
  client.publish("home/arduino/sensor/rain", sensorData.rain_detected ? "wet" : "dry");
  
  Serial.println("Sensor data published to MQTT");
}

void updateDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  
  // Display temperature and humidity
  lcd.print("T:");
  lcd.print(sensorData.temperature, 1);
  lcd.print("C H:");
  lcd.print(sensorData.humidity, 0);
  lcd.print("%");
  
  lcd.setCursor(0, 1);
  lcd.print("D:");
  lcd.print(sensorData.ultrasonic_distance);
  lcd.print("cm S:");
  lcd.print(sensorData.sound_level);
}

void checkButtons() {
  if (digitalRead(BUTTON1) == LOW) {
    delay(20); // Debounce
    if (digitalRead(BUTTON1) == LOW) {
      digitalWrite(LED_BLUE, !digitalRead(LED_BLUE));
      client.publish("home/arduino/button/button1", "pressed");
      while (digitalRead(BUTTON1) == LOW) delay(10);
      delay(200); // Debounce
    }
  }
  
  if (digitalRead(BUTTON2) == LOW) {
    delay(20);
    if (digitalRead(BUTTON2) == LOW) {
      digitalWrite(RELAY_PIN1, !digitalRead(RELAY_PIN1));
      client.publish("home/arduino/button/button2", "pressed");
      while (digitalRead(BUTTON2) == LOW) delay(10);
      delay(200);
    }
  }
}
