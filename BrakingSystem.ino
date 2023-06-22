
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

const int BRAKE_PIN = 6;
const int BUZZER_PIN = 5;
bool BarkeSwitch  = false;
bool IsBrake = false;
float Braking_Distance;
const int SWITCH_PIN = 7;
//////////////////////////////////////

// DayCycle_Veriables
const int RAIN_PIN = A0;     // Pin connected to the signal pin of the raindrop sensor
const int RAIN_THRESHOLD = 500; 
bool IsRainnig;
/////////////////////////////////////////////////


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

  // DayCycle_Veriables////////////////
  pinMode(RAIN_PIN, INPUT); 
  ////////////////////////////////////

   pinMode(BUZZER_PIN, OUTPUT);   // Set BUZZER
   pinMode(BRAKE_PIN, OUTPUT);   // Set BRAKE
   pinMode(SWITCH_PIN, INPUT_PULLUP); // Brake Stop Pin
}

void loop() {


  // Check if the switch is pressed (LOW)
  if (digitalRead(SWITCH_PIN) == LOW) {
    Serial.println("Switch is pressed!");
    BarkeSwitch = true;
  } 
  
  else
 
  {
    Serial.println("Switch is not pressed.");
    BarkeSwitch =false;
    

  }
  
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
   Brake_Apply();
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

  //Scaller_Function///////////////
  (distance / 100) * 10;

  // Print the distance value
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" m");

  delay(1000);    // Wait for a second before the next measurement
  //////////////////////////////////////////////////////////////////

/////////////Raining_Event//////////////////////////////////////
   // Read the analog value from the raindrop sensor
  int sensorValue = analogRead(RAIN_PIN);
  // Check if the sensor value exceeds the rain threshold
  if (sensorValue > RAIN_THRESHOLD) {
    Serial.println("It's not raining!");
    IsRainnig = false;
    // Add additional actions here when it's raining
  } else {
    Serial.println("It's raining.");
    IsRainnig = true; 
    // Add additional actions here when it's not raining
  }
  delay(1000);    // Wait for a second before the next reading

  
}

//////////////////////////////////////////////////////////////


  ////////////////RPM_Functions///////////////////////////
void countRevolutions() {
  // Increment revolution count
  revolutionCount++;
}

void BrakingDistance()
{
  if(IsRainnig == false)
  {
    
      Braking_Distance = (0.006514 * rpm) + (0.00000945 * rpm * rpm);

  Serial.print("Braking Distance: ");
  Serial.println(Braking_Distance);

  delay(1000); // Wait for a second before the next calculation
    }
    else
    {
      
       Braking_Distance = (0.0114 * rpm) + (0.00001654 * rpm * rpm);

  Serial.print("Braking Distance: ");
  Serial.println(Braking_Distance);

  delay(1000); // Wait for a second before the next calculation
      
      }


  }


  void Brake_Apply()
  {

    if(distance > (Braking_Distance + 1))
    {
        digitalWrite(BUZZER_PIN, LOW);
        Serial.print("No Brake: ");
        delay(1000); // Wait for a second before the next calculation

        if(IsBrake == true)
        {
          digitalWrite(BRAKE_PIN, LOW);
          IsBrake = false;
          
          }
    }
    else
    {
        if(distance < Braking_Distance)
        {
          if(BarkeSwitch == false)
            {
          Serial.print("BUZZER: ");
          delay(1000); // Wait for a second before the next calculation
          digitalWrite(BUZZER_PIN, HIGH);
              
            }

            else
            {
              Serial.print(" NO BUZZER: ");
              delay(1000); // Wait for a second before the next calculation
              digitalWrite(BUZZER_PIN, LOW);
              
              }
         
          }
          else
          {
            digitalWrite(BUZZER_PIN, LOW);
            if(BarkeSwitch == false)
            {
              
                IsBrake = true;
                digitalWrite(BRAKE_PIN, HIGH);
            Serial.print("Apply Brake: ");
            delay(1000); // Wait for a second before the next calculation
              }
              else
              {
                 IsBrake = false;
                 digitalWrite(BRAKE_PIN, LOW);
            Serial.print("Apply Realaes: ");
            delay(1000); // Wait for a second before the next calculation
                
                }

          
            
            }
         
    }
        
    }
  
  
  
