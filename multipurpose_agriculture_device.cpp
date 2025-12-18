char command;

// Motor driver pins
int IN1 = 2;
int IN2 = 3;
int IN3 = 4;
int IN4 = 5;
int ENA = 9;
int ENB = 10;

// Tools
int cutter = 7;
int collector = 8;
int waterPump = 6;

// Soil moisture
int soilSensor = A0;
int moistureThreshold = 500;  // Adjust after testing

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(cutter, OUTPUT);
  pinMode(collector, OUTPUT);
  pinMode(waterPump, OUTPUT);

  Serial.begin(9600);

  stopRobot();
  digitalWrite(cutter, LOW);
  digitalWrite(collector, LOW);
  digitalWrite(waterPump, LOW);
}

void loop() {

  // 🔹 Automatic Watering Logic
  int moistureValue = analogRead(soilSensor);

  if (moistureValue > moistureThreshold) {
    digitalWrite(waterPump, HIGH);   // Soil dry → Pump ON
  } else {
    digitalWrite(waterPump, LOW);    // Soil wet → Pump OFF
  }

  // 🔹 Bluetooth control
  if (Serial.available()) {
    command = Serial.read();

    switch (command) {
      case 'F': forward(); break;
      case 'B': backward(); break;
      case 'L': left(); break;
      case 'R': right(); break;
      case 'S': stopRobot(); break;

      case 'C': digitalWrite(cutter, HIGH); break;
      case 'c': digitalWrite(cutter, LOW); break;

      case 'T': digitalWrite(collector, HIGH); break;
      case 't': digitalWrite(collector, LOW); break;
    }
  }
}

// 🔹 Movement functions
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopRobot() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
