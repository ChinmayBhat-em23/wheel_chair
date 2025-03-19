#include <Servo.h>

Servo myServo;
Servo myServo1;
int pos = 90;      // Neutral position of servo
int angle = 35;    // Angle to rotate the servo
const int trigPin = 7;
const int echoPin = 6;
long duration;
int distance;
const int IN1 = 2;  // Motor direction pin 1
const int IN2 = 4;  // Motor direction pin 2

void setup() {
  myServo1.attach(5);
  myServo.attach(9);
  myServo.write(pos); // Set servo to neutral position
  myServo.write(pos);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  Serial.begin(115200);
  Serial.println("Arduino Ready. Waiting for commands...");
}

void loop() {
  // Measure distance using ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Stop the motor if an object is detected within 10 cm
  if (distance > 0 && distance < 20) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    Serial.println("Object detected. Stopping motor.");

  }
  static byte receivedBytes[2];
  static int byteCount = 0;

  if (Serial.available() > 0) {
    byte receivedByte = Serial.read();
    Serial.print("Received Byte (Decimal): ");
    Serial.println(receivedByte);
    Serial.print("Received Byte (Hex): ");
    Serial.println(receivedByte, HEX);

    receivedBytes[byteCount] = receivedByte;
    byteCount++;

    if (byteCount == 2) {
      // Print received hex values
      Serial.print("Received Hex Values: ");
      Serial.print(receivedBytes[0], HEX);
      Serial.print(" ");
      Serial.println(receivedBytes[1], HEX);

      // Handle commands
      if (receivedBytes[0] == 0xAB && receivedBytes[1] == 0xEE) {
        myServo.write(pos - angle);
        myServo1.write(pos - angle);
        delay(3000);
        myServo.write(pos);
        myServo1.write(pos);
      } 
      else if (receivedBytes[0] == 0xAC && receivedBytes[1] == 0xEF) {
        myServo.write(pos + angle);
        myServo1.write(pos + angle);
        delay(3000);
        myServo.write(pos);
        myServo1.write(pos);
      } 
      else if (receivedBytes[0] == 0xAD && receivedBytes[1] == 0xFF) {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
      }
       else if (receivedBytes[0] == 0xAE && receivedBytes[1] == 0xDC) {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
      }
       else if (receivedBytes[0] == 0xAB && receivedBytes[1] == 0xCD) {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW); // Stop motor
      }
       else {
        Serial.println("Unknown command received.");
      }

      // Reset the counter for the next command
      byteCount = 0;
    }
  }
}
