// Include the SoftwareSerial library
#include <SoftwareSerial.h>

// RPM Variables
const int IR_PIN = 2;                 // Pin connected to the OUT pin of the IR sensor
const int UPDATE_INTERVAL = 1000;     // Update interval in milliseconds (1 second)
volatile int revolutionCount = 0;     // Count of revolutions
unsigned long previousTime = 0;       // Time of the previous update
int rpm = 0;                          // RPM value

// Distance Variables
const int TRIG_PIN = 4;    // Pin connected to the Trig pin of the ultrasonic sensor
const int ECHO_PIN = 3;    // Pin connected to the Echo pin of the ultrasonic sensor
long duration;             // Time taken for the ultrasonic pulse to return
int distance;              // Calculated distance in centimeters

// Braking Distance Variables
const int BRAKE_PIN = 6;
const int BUZZER_PIN = 5;
bool BarkeSwitch = false;
bool IsBrake = false;
float Braking_Distance;
const int SWITCH_PIN = 7;

// Rain Condition Variables
const int RAIN_PIN = A0;         // Pin connected to the signal pin of the raindrop sensor
const int RAIN_THRESHOLD = 500;
bool IsRaining;

// Define the software serial object
SoftwareSerial mySerial(10, 11);  // RX, TX pins

void setup() {
  Serial.begin(9600);           // Initialize the hardware serial connection
  mySerial.begin(9600);         // Initialize the software serial connection

  // RPM Setup
  pinMode(IR_PIN, INPUT);                                      // Set the IR pin as input
  attachInterrupt(digitalPinToInterrupt(IR_PIN), countRevolutions, RISING);  // Attach interrupt for rising edge detection

  // Distance Setup
  pinMode(TRIG_PIN, OUTPUT);   // Set Trig pin as output
  pinMode(ECHO_PIN, INPUT);    // Set Echo pin as input

  // Brake Setup
  pinMode(BUZZER_PIN, OUTPUT);   // Set BUZZER
  pinMode(BRAKE_PIN, OUTPUT);    // Set BRAKE
  pinMode(SWITCH_PIN, INPUT_PULLUP); // Brake Stop Pin

  // Rain Condition Setup
  pinMode(RAIN_PIN, INPUT);
}

void loop() {
  // Check if the switch is pressed (LOW)
  if (digitalRead(SWITCH_PIN) == LOW) {
    Serial.println("Switch is pressed!");
    BarkeSwitch = true;
  } else {
    Serial.println("Switch is not pressed.");
    BarkeSwitch = false;
  }

  // RPM Calculation
  unsigned long currentTime = millis();   // Current time
  unsigned long elapsedTime = currentTime - previousTime;  // Calculate time elapsed since the previous update
  if (elapsedTime >= UPDATE_INTERVAL) {
    rpm = (revolutionCount * 60000) / elapsedTime;
    revolutionCount = 0;
    previousTime = currentTime;
    Serial.print("RPM: ");
    Serial.println(rpm);
  }

  // Braking Distance Calculation
  BrakingDistance();

  // Distance Measurement
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Rain Condition Measurement
  int sensorValue = analogRead(RAIN_PIN);
  if (sensorValue > RAIN_THRESHOLD) {
    Serial.println("It's not raining!");
    IsRaining = false;
  } else {
    Serial.println("It's raining.");
    IsRaining = true;
  }

  // Sending values to Python via the serial connection
  mySerial.print("RPM: ");
  mySerial.println(rpm);
  mySerial.print("Braking Distance: ");
  mySerial.println(Braking_Distance);
  mySerial.print("Distance: ");
  mySerial.println(distance);
  mySerial.print("Raining: ");
  mySerial.println(IsRaining);

  delay(1000);
}

void countRevolutions() {
  revolutionCount++;
}

void BrakingDistance() {
  if (IsRaining == false) {
    Braking_Distance = (0.006514 * rpm) + (0.00000945 * rpm * rpm);
    Serial.print("Braking Distance: ");
    Serial.println(Braking_Distance);
    delay(1000);
  } else {
    Braking_Distance = (0.0114 * rpm) + (0.00001654 * rpm * rpm);
    Serial.print("Braking Distance: ");
    Serial.println(Braking_Distance);
    delay(1000);
  }
}
