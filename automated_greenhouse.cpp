#include <DHT.h>

// ----- DHT Configuration -----
#define DHTPIN 2
#define DHTTYPE DHT11   // Change to DHT22 if used
DHT dht(DHTPIN, DHTTYPE);

// ----- Pin Definitions -----
int soilSensor = A0;
int ldrSensor = A1;

int fan = 8;
int light = 9;
int pump = 10;

// ----- Thresholds -----
float tempThreshold = 30.0;     // Celsius
int soilThreshold = 500;        // Adjust after testing
int ldrThreshold = 600;         // Adjust after testing

void setup() {
  pinMode(fan, OUTPUT);
  pinMode(light, OUTPUT);
  pinMode(pump, OUTPUT);

  Serial.begin(9600);
  dht.begin();

  digitalWrite(fan, LOW);
  digitalWrite(light, LOW);
  digitalWrite(pump, LOW);
}

void loop() {

  // 🌡 Temperature Control
  float temperature = dht.readTemperature();

  if (!isnan(temperature)) {
    if (temperature > tempThreshold) {
      digitalWrite(fan, HIGH);
    } else {
      digitalWrite(fan, LOW);
    }
  }

  // 🌱 Soil Moisture Control
  int soilValue = analogRead(soilSensor);

  if (soilValue > soilThreshold) {
    digitalWrite(pump, HIGH);   // Soil dry
  } else {
    digitalWrite(pump, LOW);    // Soil wet
  }

  // 💡 Light Control
  int ldrValue = analogRead(ldrSensor);

  if (ldrValue < ldrThreshold) {
    digitalWrite(light, HIGH);  // Dark
  } else {
    digitalWrite(light, LOW);   // Bright
  }

  // 📊 Serial Monitor Output
  Serial.println("---- Greenhouse Status ----");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Soil Moisture: ");
  Serial.println(soilValue);

  Serial.print("Light Intensity: ");
  Serial.println(ldrValue);
  Serial.println("----------------------------");

  delay(2000);
}
