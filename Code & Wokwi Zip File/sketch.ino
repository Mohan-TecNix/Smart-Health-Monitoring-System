#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Pins
#define TEMP_PIN 4
#define POT_PIN 34
#define BUZZER_PIN 5

// OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Temperature Sensor
OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);

  // OLED I2C
  Wire.begin(21, 22);

  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // DS18B20
  sensors.begin();

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED Failed");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);

  // Welcome Screen
  display.setTextSize(2);
  display.setCursor(10, 20);
  display.println("WELCOME");
  display.display();

  delay(2000);
}

void loop() {

  // Read Temperature
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);

  // Read Potentiometer
  int potValue = analogRead(POT_PIN);

  // Convert Pot Value to BPM
  int bpm = map(potValue, 0, 4095, 60, 120);

  // Alert Conditions
  bool alert = false;

  if (temp > 38.0 || bpm > 110 || bpm < 60) {
    alert = true;
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  // OLED Display
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Smart Health Monitor");

  display.drawLine(0, 10, 128, 10, WHITE);

  display.setCursor(0, 18);
  display.print("Heart Rate: ");
  display.print(bpm);
  display.println(" BPM");

  display.setCursor(0, 35);
  display.print("Temp: ");
  display.print(temp, 1);
  display.println(" C");

  display.setCursor(0, 52);

  if (alert) {
    display.print("Status: ALERT!");
  } else {
    display.print("Status: NORMAL");
  }

  display.display();

  // Serial Monitor
  Serial.print("BPM: ");
  Serial.print(bpm);
  Serial.print(" | Temp: ");
  Serial.print(temp);
  Serial.print(" C | Status: ");

  if (alert)
    Serial.println("ALERT");
  else
    Serial.println("NORMAL");

  delay(1000);
}