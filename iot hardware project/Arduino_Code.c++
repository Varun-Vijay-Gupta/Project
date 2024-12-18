

#include <Wire.h>  // Required for camera communication

// Pin Definitions for OV7670 Camera
#define OV7670_VSYNC 6  // Pin connected to VSYNC
#define OV7670_HREF  9  // Pin connected to HREF
#define OV7670_PCLK  10  // Pin connected to PCLK
#define OV7670_RST   11  // Pin connected to RST
#define OV7670_SIOC  12  // Pin connected to SIOC
#define OV7670_SIOD  A4  // Pin connected to SIOD
#define OV7670_D0    A0  // Pin connected to D0
#define OV7670_D1    A1  // Pin connected to D1
#define OV7670_D2    A2  // Pin connected to D2
#define OV7670_D3    A3  // Pin connected to D3
#define OV7670_D4    A5  // Pin connected to D4

// Pin definitions for the IR sensor array
const int irLeftPin = 2;    // Pin connected to the left sensor
const int irCenterPin = 3;  // Pin connected to the center sensor
const int irRightPin = 4;   // Pin connected to the right sensor

// Pin definitions for the ultrasonic sensor
#define TRIG_PIN 7  // Trigger pin for the ultrasonic sensor
#define ECHO_PIN 8  // Echo pin for the ultrasonic sensor

void setup() {
  Serial.begin(115200); // Start serial communication
  Wire.begin();
  
  // Reset and initialize camera
  pinMode(OV7670_RST, OUTPUT);
  digitalWrite(OV7670_RST, LOW);
  delay(1000);
  digitalWrite(OV7670_RST, HIGH);
  
  // Set up the ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT); // Set TRIG pin as output
  pinMode(ECHO_PIN, INPUT);  // Set ECHO pin as input

  // Set the IR sensor pins as input
  pinMode(irLeftPin, INPUT);
  pinMode(irCenterPin, INPUT);
  pinMode(irRightPin, INPUT);
}

void loop() {
  // Measure distance using the ultrasonic sensor
  long distance = measureDistance();

  // Read the values from the IR sensors
  int leftSensorValue = digitalRead(irLeftPin);
  int centerSensorValue = digitalRead(irCenterPin);
  int rightSensorValue = digitalRead(irRightPin);

  // Check if any IR sensor detects an object
  if (leftSensorValue == HIGH || centerSensorValue == HIGH || rightSensorValue == HIGH) {
    // Object detected, print distance and detection message
    Serial.println("Object detected.");
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    // Check if the distance is less than 20 cm
    if (distance < 20) {
      captureImage();  // Capture image if an object is closer than 20 cm
      delay(5000);     // Delay to avoid capturing multiple images quickly
      return;         // Exit loop after capturing image
    }
  } else {
    // If no object is detected, print a message
    Serial.println("No object detected.");
  }

  // Small delay to avoid spamming the serial monitor
  delay(500);
}

// Function to measure distance using ultrasonic sensor
long measureDistance() {
  long duration;

  // Clear the TRIG_PIN
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Set the TRIG_PIN HIGH for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the ECHO_PIN and get the duration of the pulse
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance in cm
  long distance = duration * 0.034 / 2;

  return distance; // Return the distance
}

void captureImage() {
  Serial.println("Capturing image...");
  int pixelCount = 0;

  // Loop through rows
  for (int row = 0; row < 120; row++) {
    // Loop through columns
    for (int col = 0; col < 160; col++) {
      // Read pixel data from camera pins
      int pixel_value = digitalRead(OV7670_D0); // Read single bit for grayscale
      Serial.print(pixel_value); // Send pixel data over serial

      pixelCount++; // Increment pixel count
      
      if (col < 159) { // Avoid adding a comma after the last pixel
        Serial.print(","); // Separate pixel data by commas
      }
    }
    Serial.println(); // New line after each row of pixels
  }

  // Print total pixels sent for debugging
  Serial.print("Total pixels sent: ");
  Serial.println(pixelCount); // Should print 19200 if correct

  // Check if the pixel count matches expected value
  if (pixelCount != 19200) {
    Serial.println("Warning: Pixel count does not match expected value.");
  }

  Serial.println("Image data sent.");
}
