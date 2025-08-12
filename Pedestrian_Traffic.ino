/*
 * Combined PIR Motion Sensor + RGB LED Traffic Light + Buzzer
 * 
 * Default: Green light
 * Motion detected: Red light + buzzer sound
 */

// Pin definitions
const int PIN_RED   = 2;
const int PIN_GREEN = 3;
const int PIN_BLUE  = 4;
const int buzzer = 9;
const int sensor = 7;

// Variables for motion detection
int state = LOW;       // Current state of motion detection
int val = 0;           // Reading from sensor
bool motionDetected = false;
unsigned long motionStartTime = 0;
const unsigned long ALERT_DURATION = 5000; // How long to keep red light and buzzer (5 seconds)

void setup() {
  // Initialize LED pins
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);
  
  // Initialize buzzer and sensor
  pinMode(buzzer, OUTPUT);
  pinMode(sensor, INPUT);
  
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Set default green light
  setGreenLight();
}

void loop() {
  // Read PIR sensor
  val = digitalRead(sensor);
  
  if (val == HIGH && !motionDetected) {
    // Motion just detected
    Serial.println("Motion detected - ALERT!");
    motionDetected = true;
    motionStartTime = millis();
    setRedLight();
    soundAlarm();
  }
  else if (val == LOW && motionDetected) {
    // No motion and we were in alert state
    Serial.println("Motion stopped");
    motionDetected = false;
    setGreenLight();
    noTone(buzzer);
  }
  
  // If in alert state, check if we should continue the alarm
  if (motionDetected) {
    // Continue sounding alarm while motion is detected
    if (millis() - motionStartTime > 1000) {
      // Reset timer for next beep cycle
      motionStartTime = millis();
      soundAlarm();
    }
  }
  
  delay(100); // Small delay for stability
}

void setGreenLight() {
  analogWrite(PIN_RED,   0);
  analogWrite(PIN_GREEN, 255);
  analogWrite(PIN_BLUE,  0);
}

void setRedLight() {
  analogWrite(PIN_RED,   255);
  analogWrite(PIN_GREEN, 0);
  analogWrite(PIN_BLUE,  0);
}

void soundAlarm() {
  tone(buzzer, 100); // 1KHz tone
  delay(200);         // Short beep
  noTone(buzzer);
}
