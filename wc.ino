#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// --- Pin Definitions ---
#define DHTPIN 5
#define DHTTYPE DHT11
#define TRIG 7
#define ECHO 8
#define PIRPIN 6
#define LED_ULTRASONIC 13
#define LED_PIR 10
#define HEART_SENSOR A0

// --- Objects ---
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- Degree Symbol for LCD ---
byte degree_symbol[8] = {
  0b00111,
  0b00101,
  0b00111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

int screen = 0; // Display screen tracker

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(PIRPIN, INPUT);
  pinMode(LED_ULTRASONIC, OUTPUT);
  pinMode(LED_PIR, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.createChar(1, degree_symbol);

  Serial.println("System Initialized...");
}

void loop() {
  // --- Sensor Readings ---
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // Ultrasonic
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH);
  int distance = duration * 0.034 / 2;

  // PIR
  int motion = digitalRead(PIRPIN);
  digitalWrite(LED_PIR, motion);

  // Heart Rate Sensor
  int pulseValue = analogRead(HEART_SENSOR);
  int bpm = map(pulseValue, 500, 1023, 60, 100); // rough estimate

  // LED for ultrasonic
  if (distance >= 1 && distance <= 50) {
    digitalWrite(LED_ULTRASONIC, HIGH);
  } else {
    digitalWrite(LED_ULTRASONIC, LOW);
  }

  // --- Serial Output ---
  Serial.println("===========================");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println(" %");

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("Ultrasonic LED: ");
  Serial.println((distance >= 1 && distance <= 50) ? "ON" : "OFF");

  Serial.print("Motion Detected (PIR): ");
  Serial.println(motion ? "YES" : "NO");

  Serial.print("PIR LED: ");
  Serial.println(motion ? "ON" : "OFF");

  Serial.print("Pulse Sensor Raw Value: ");
  Serial.println(pulseValue);

  Serial.print("Estimated BPM: ");
  Serial.println(bpm);
  Serial.println("===========================\n");

  // --- LCD Display ---
  lcd.clear();
  if (screen == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.write(1);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(hum);
    lcd.print("%");
  }

  else if (screen == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.print("cm");

    lcd.setCursor(0, 1);
    if (distance < 10) lcd.print("Very Close");
    else if (distance < 50) lcd.print("Medium");
    else lcd.print("Far");
  }

  else if (screen == 2) {
    lcd.setCursor(0, 0);
    lcd.print("Motion: ");
    lcd.print(motion ? "Yes" : "None");

    lcd.setCursor(0, 1);
    lcd.print("PIR LED: ");
    lcd.print(motion ? "ON" : "OFF");
  }

  else if (screen == 3) {
    lcd.setCursor(0, 0);
    lcd.print("Pulse Sensor");

    lcd.setCursor(0, 1);
    lcd.print("BPM: ");
    lcd.print(bpm);
  }

  screen = (screen + 1) % 4;
  delay(2000);
}
