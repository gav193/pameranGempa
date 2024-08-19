#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#define TRIG_PIN 18  // Trigger pin
#define ECHO_PIN 19  // Echo pin
#define EN_PIN 2     // Enable button as input
#define BUZZ_PIN 4   // Buzzer enable
#define BUZZER 33
#define SCL_PIN 22 // Define the SCL pin
#define SDA_PIN 21  // Define the SDA pin

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// Create an instance of the display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_ADXL345_Unified adxl345 = Adafruit_ADXL345_Unified();

Servo myServo;  // Create a Servo object to control the servo
Servo myServo2;

void setup() {
  myServo.attach(13);  // Attach to GPIO 13 or any other PWM-capable pin on ESP32
  myServo2.attach(23);
  pinMode(EN_PIN, INPUT); // Enable button
  pinMode(BUZZ_PIN, OUTPUT); // Buzzer
  pinMode(BUZZER, OUTPUT);
  Wire.begin(SDA_PIN, SCL_PIN);  // Initialize I2C with SDA and SCL pins
  Serial.begin(115200);
  
  // Initialize the OLED display with the correct address
  if (!display.begin(SSD1306_PAGEADDR, 0x3C)) {  // Use 0x3C or 0x3D depending on your display
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Infinite loop
  }
  if (!adxl345.begin()) {
    Serial.println("Failed to find ADXL345 sensor");
    while (1);
  }
  adxl345.setRange(ADXL345_RANGE_16_G);
  Serial.println("ADXL345 sensor found!");
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  digitalWrite(TRIG_PIN, LOW);
  display.display();  // Show the initial splash screen
  delay(2000);        // Pause for 2 seconds

  display.clearDisplay();  // Clear the display buffer

  // Draw text
  display.setTextSize(1);         // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0,0);        // Start at top-left corner
  display.print(F("Hello, World!"));
  digitalWrite(BUZZER,LOW);
  display.display();  // Update the display with the new content
}

void loop() {
  // Measure distance
  digitalWrite(BUZZER,LOW);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Wait for the echo pin to go HIGH
  long duration = pulseIn(ECHO_PIN, HIGH);
  // Calculate the distance (in cm)
  float distance = (duration / 2.0) * 0.0344;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  sensors_event_t event;
  adxl345.getEvent(&event);

  //Serial.print("X: ");
  float x = event.acceleration.x;
  Serial.println(x);
  //Serial.print(" m/s^2\tY: ");
  float y = event.acceleration.y;
  Serial.println(y);
  //Serial.print(" m/s^2\tZ: ");
  float z = event.acceleration.z;
  //Serial.println(" m/s^2");
  Serial.println(z);
  Serial.println("==========");
  int enable = digitalRead(EN_PIN);

  delay(10);

  if (enable == HIGH) { // Switch ON
    Serial.println("Enable switch is ON");
    display.clearDisplay();
    display.setCursor(0,0);
    display.printf("Switch: ON\nDistance: %.2f cm\n", distance);
    //display.display();
    if (distance == 0) {
      // skip
    } else if (distance > 0 && distance <= 10) {
      //display.printf("RS 1.5\n");
      myServo.write(0);    // Move the servo to 0 degrees
      myServo2.write(10);
      delay(100);         // Wait for a second

      myServo.write(10);   // Move the servo to 20 degrees
      myServo2.write(0);
      delay(100);         // Wait for a second

      myServo.write(0);    // Move the servo back to 0 degrees
      myServo2.write(10);
      delay(100);        // Wait for a second
    } else if (distance > 10 && distance <= 20) {
      //display.printf("RS 3\n");
      myServo.write(0);    // Move the servo to 0 degrees
      
      myServo2.write(20);
      delay(100);         // Wait for a second

      myServo.write(20);   // Move the servo to 20 degrees
      myServo2.write(0);
      delay(100);        // Wait for a second

      myServo.write(0);    // Move the servo back to 0 degrees
      myServo2.write(20);
      delay(100);         // Wait for a second
    } else if (distance > 20 && distance <= 30) {
      //display.printf("RS 5\n");
      myServo.write(0);    // Move the servo to 0 degrees
      myServo2.write(30);
      delay(100);         // Wait for a second

      myServo.write(30);   // Move the servo to 20 degrees
      myServo2.write(0);
      delay(100);        // Wait for a second

      myServo.write(0);    // Move the servo back to 0 degrees
      myServo2.write(30);
      delay(100);        // Wait for a second
    } else {
      //display.printf("RS 7\n");
      delay(10);
      digitalWrite(BUZZER,HIGH);
      myServo.write(0);    // Move the servo to 0 degrees
      myServo2.write(40);
      delay(100);         // Wait for a second

      myServo.write(40);   // Move the servo to 20 degrees
      myServo2.write(0);
      delay(100);         // Wait for a second

      myServo.write(0);    // Move the servo back to 0 degrees
      myServo2.write(40);
      delay(100);       // Wait for a second
      digitalWrite(BUZZER,LOW);
    }
    display.display();    
  } else { // Switch OFF
    Serial.println("Enable switch is OFF");
    display.clearDisplay();
    display.setCursor(0,0);
    display.print(F("Switch: OFF"));
    display.display();
    //digitalWrite(BUZZER,LOW);
    digitalWrite(BUZZ_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZ_PIN, LOW);
    delay(1000);
  }
  delay(1000);
  if ( x > -0.2 && x < 0.2) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.printf("RS 1");
    display.display();
  } else {
    if (x > -0.4 && x < 0.4) {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.printf("RS 3");
      display.display();
    } else {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.printf("RS 5");
      display.display();
    }
  }

  delay(500);  // Wait for half a second before next measurement
}