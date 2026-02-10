#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// OLED Display (0.96" SSD1306)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT11 Sensor
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Soil Moisture Sensor
#define SOIL_MOISTURE_PIN 34
const int dryValue = 4095; // Calibrate these based on your sensor
const int wetValue = 3000;

// PIR Motion Sensor
#define PIR_PIN 5

// Relay (water pump)
#define RELAY_PIN 23

void setup() {
  Serial.begin(115200);
  
  // Initialize OLED (SSD1306)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED initialization failed");
    while (1);
  }
  display.display();
  delay(1000);
  display.clearDisplay();
  
  // Initialize DHT
  dht.begin();
  
  // Set pin modes
  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Start with relay OFF
  
  // Show startup message
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Plant Monitor");
  display.println("Initializing...");
  display.display();
  delay(2000);
}

void loop() {
  // Read sensors
  float humidity = dht.readHumidity();
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  int soilPercent = map(soilMoisture, dryValue, wetValue, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);
  bool motionDetected = digitalRead(PIR_PIN);
  
  // Control relay based on soil moisture only
  if (soilPercent < 10) {
    digitalWrite(RELAY_PIN, LOW); // Turn ON pump
  } else if (soilPercent >= 90) {
    digitalWrite(RELAY_PIN, HIGH); // Turn OFF pump
  }
  
  // Update OLED display
  display.clearDisplay();
  display.setCursor(0, 0);
  
  display.setTextSize(2);
  display.print("Hum: ");
  display.print(humidity);
  display.println("%");

  display.setTextSize(1);
  display.print("Soil: ");
  display.print(soilPercent);
  display.println("%");
  
  display.print("Motion: ");
  display.println(motionDetected ? "DETECTED" : "No motion");
  
  
  display.display();
  
  // Serial monitor output
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  
  Serial.print("Soil: ");
  Serial.print(soilPercent);
  Serial.println("%");
  
  Serial.print("Motion: ");
  Serial.println(motionDetected ? "Yes" : "No");
  
  Serial.print("Pump: ");
  Serial.println(digitalRead(RELAY_PIN) == LOW ? "ON" : "OFF");
  Serial.println("-----");
  
  delay(1000);
}
