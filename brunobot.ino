#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// Define pins
#define TRIGGER_PIN 22 // Trigger pin for ultrasonic sensor
#define ECHO_PIN 23   // Echo pin for ultrasonic sensor

// SoftwareSerial for Bluetooth, DFPlayer Mini, and Debugging
SoftwareSerial bluetoothSerial(12, 13); // RX, TX for Bluetooth module

// Serial1 for DFPlayer Mini (TX1 and RX1 on Arduino Mega)
SoftwareSerial dfPlayerSerial(10, 11); // RX1, TX1 for DFPlayer Mini
DFRobotDFPlayerMini myDFPlayer;

// Motor control pins
#define m1a 4
#define m1b 5
#define m2a 6
#define m2b 7
#define ENA 8 // Change ENA to 6
#define ENB 9

char command = 'S'; // Initialize with 'S' for stop

void setup() {
  Serial.begin(9600); // Initialize Serial for debugging

  // Initialize DFPlayer Mini communication
  dfPlayerSerial.begin(9600);
  if (!myDFPlayer.begin(dfPlayerSerial)) {
    Serial.println("Unable to begin DFPlayer Mini:");
    Serial.println("1. Please recheck the connection!");
    Serial.println("2. Please insert the SD card!");
    while (true);
  }
  myDFPlayer.volume(30); // Set the volume (0-30)

  // Initialize Bluetooth communication
  bluetoothSerial.begin(9600);

  // Motor control pins setup
  pinMode(m1a, OUTPUT);
  pinMode(m1b, OUTPUT);
  pinMode(m2a, OUTPUT);
  pinMode(m2b, OUTPUT);
  pinMode(ENA, OUTPUT); // Change ENA to 6
  pinMode(ENB, OUTPUT);

  // Ultrasonic sensor pins setup
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void forward() {
  analogWrite(ENA, 240);
  analogWrite(ENB, 180);
  digitalWrite(m1a, LOW);
  digitalWrite(m1b, HIGH);
  digitalWrite(m2a, LOW);
  digitalWrite(m2b, HIGH);
  delay(200);
}

void stopp() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(m1a, LOW);
  digitalWrite(m1b, LOW);
  digitalWrite(m2a, LOW);
  digitalWrite(m2b, LOW);
  delay(200);
}

void loop() {
  if (bluetoothSerial.available() > 0) {
    command = bluetoothSerial.read();
    Serial.print("Received Bluetooth Command: ");
    Serial.println(command);
  }

  // Perform actions based on the received command
  switch (command) {
    case 'F': // Move forward
      forward();
      ultra();
      break;
    case 'S': // Stop
      stopp();
      break;
    default:
      // Invalid command or do nothing
      break;
  }
}
void ultra()
  {
  // Ultrasonic sensor code to detect obstacles
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH);
  unsigned int distance = duration * 0.034 / 2;
   Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm"); 
  // Check for obstacles and play audio if needed
  if (distance <= 20) {
    stopp();
    myDFPlayer.play(1); // Play audio when an obstacle is detected
    delay(5000); // Play for 5 seconds
    myDFPlayer.stop(); // Stop playing
    delay(1000); // Delay before playing another track
    myDFPlayer.play(2);
     delay(1000);
  }
} 
