
// RPM_Veriables
const int IR_PIN = 2;    // Pin connected to the OUT pin of the IR sensor
const int UPDATE_INTERVAL = 1000;   // Update interval in milliseconds (1 second)
volatile int revolutionCount = 0;   // Count of revolutions
unsigned long previousTime = 0;     // Time of the previous update
int rpm = 0;                        // RPM value
////////////////////////////////////////////////////////////////////////////////////////////

// Distance_Veriables
const int TRIG_PIN = 4;    // Pin connected to the Trig pin of the ultrasonic sensor
const int ECHO_PIN = 3;    // Pin connected to the Echo pin of the ultrasonic sensor
long duration;             // Time taken for the ultrasonic pulse to return
int distance;              // Calculated distance in centimeters
//////////////////////////////////////////////////////////////////////////////////////////////

// BrakingDistance_Veriables

int Braking_Distance;

void setup() {
  Serial.begin(9600);               // Initialize serial communication
  
   ////////////////RPM_Functions///////////////////////////
  pinMode(IR_PIN, INPUT);           // Set the IR pin as input
  attachInterrupt(digitalPinToInterrupt(IR_PIN), countRevolutions, RISING);  // Attach interrupt for rising edge detection
  ////////////////////////////////////////////////////////////

  //////////////////Distance_Veriables////////////////////////////////////////////////
  pinMode(TRIG_PIN, OUTPUT);   // Set Trig pin as output
  pinMode(ECHO_PIN, INPUT);    // Set Echo pin as input
  ////////////////////////////////////////////////////////////////////////////////////
  
}

void loop() {


  ////////////////RPM_Functions///////////////////////////
  unsigned long currentTime = millis();   // Current time
  // Calculate time elapsed since the previous update
  unsigned long elapsedTime = currentTime - previousTime;
  // If the update interval has elapsed
  if (elapsedTime >= UPDATE_INTERVAL) {
    // Calculate RPM
    rpm = (revolutionCount * 60000) / elapsedTime;
    // Reset revolution count and update time
    revolutionCount = 0;
    previousTime = currentTime;
    // Print RPM value
    Serial.print("RPM: ");
    Serial.println(rpm);
  }
  /////////////////////////////////////////////////////////

  ////////////////BrakingDistance_Functions///////////////////////////
   BrakingDistance();
  //////////////////Distance_Function/////////////////////
    digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the duration it takes for the pulse to return
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance in centimeters
  distance = duration * 0.034 / 2;

  // Print the distance value
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(1000);    // Wait for a second before the next measurement
  //////////////////////////////////////////////////////////////////
}


  ////////////////RPM_Functions///////////////////////////
void countRevolutions() {
  // Increment revolution count
  revolutionCount++;
}

void BrakingDistance()
{
  int value = (0.006514*rpm)+(0.00000945*rpm*rpm);
  //value =Braking_Distance;
  Serial.print("BrakingDistance: ");
  Serial.print(value);
  delay(1000);    // Wait for a second before the next measurement
  
  }
