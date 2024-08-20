// #include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <base64.h>

#define TRIG_PIN 18  // Trigger pin
#define ECHO_PIN 19  // Echo pin
#define EN_PIN 2     // Enable button as input
#define BLINK_PIN 4   // Blink pin
#define BUZZER 33   // Buzzer pin
#define SCL_PIN 22 // Define the SCL pin
#define SDA_PIN 21  // Define the SDA pin
#define green 25 // for leds
#define yellow 26
#define red 32

#define IN1 14 // dc motor driver pins
#define IN2 27
#define ENA 12

#define SCREEN_WIDTH 128 // lcd dimensions
#define SCREEN_HEIGHT 32

// Create an instance of the display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // lcd
Adafruit_ADXL345_Unified adxl345 = Adafruit_ADXL345_Unified(); // accelerometer

const char* ssid = "TAMSAR_LT1";
const char* password = "LT143BSD";

String url; 

void setup() {
  // myServo.attach(13);  // Attach to GPIO 13 or any other PWM-capable pin on ESP32
  // myServo2.attach(23);
  pinMode(EN_PIN, INPUT); 
  pinMode(BLINK_PIN, OUTPUT); 
  pinMode(BUZZER, OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red,OUTPUT);

  Wire.begin(SDA_PIN, SCL_PIN);  // Initialize I2C with SDA and SCL pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  ledcAttach(ENA, 5000, 8); // analog reading 

  WiFi.hostname("NodeMCU");
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi Not Connected");
    delay(500);
  }

  Serial.begin(115200); // baud rate

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(red, LOW);
  ledcWrite(ENA,0);

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
  digitalWrite(BLINK_PIN, LOW);
}

void loop() {
  // Measure distance
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
  // measure 1st accel
  Serial.print("X: ");
  float x1 = event.acceleration.x;
  Serial.print(x1);
  Serial.print(" m/s^2\nY: ");
  float y1 = event.acceleration.y;
  Serial.print(y1);
  Serial.print(" m/s^2\nZ: ");
  float z1 = event.acceleration.z;
  Serial.print(z1);
  Serial.println(" m/s^2");
  Serial.println("==========");
  int enable = digitalRead(EN_PIN);
  delay(10);
  // measure 2nd accel
  adxl345.getEvent(&event);
  float x2 = event.acceleration.x;
  Serial.print("X: ");
  Serial.print(x2);
  Serial.print(" m/s^2\nY: ");
  float y2 = event.acceleration.y;
  Serial.print(y2);
  Serial.print(" m/s^2\nZ: ");
  float z2 = event.acceleration.z;
  Serial.print(z2);
  Serial.println(" m/s^2");

  float x = abs(x2-x1);
  float y = abs(y2-y1);
  float z = abs(z2-z1);
  Serial.println("||||||||||");
  Serial.print("x = ");
  Serial.println(x);
  Serial.print("y = ");
  Serial.println(y);
  Serial.print("z = ");
  Serial.println(z);
  
  delay(10);
  if (enable == HIGH) { // Switch ON
    Serial.println("Enable switch is ON");
    display.clearDisplay();
    display.setCursor(0,0);
    display.printf("Switch: ON\nDistance: %.2f cm\n", distance);
    if (distance == 0) {
      // skip
    } else if (distance > 0 && distance <= 5) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      ledcWrite(ENA, 255);
      // display.setCursor (100,0);
      // display.printf("RS 7")
    } else if (distance > 5 && distance <= 10) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      ledcWrite(ENA,240);
      // display.setCursor (100,0);
      // display.printf("RS 6.5")
    } else if (distance > 10 && distance <= 15) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      ledcWrite(ENA,210);
      // display.setCursor (100,0);
      // display.printf("RS 6")
    } else if (distance > 15 && distance <= 20) { 
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      ledcWrite(ENA,180);
      // display.setCursor (100,0);
      // display.printf("RS 5.5")
    } else if (distance > 20 && distance <= 25) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      ledcWrite(ENA, 160);
      // display.setCursor (100,0);
      // display.printf("RS 5")
    } else if (distance > 25 && distance <= 30){
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      ledcWrite(ENA,100);
      // display.setCursor (100,0);
      // display.printf("RS 4.5")
    } else { 
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      ledcWrite(ENA,50);
      // display.setCursor (100,0);
      // display.printf("RS 4")
    }
    display.display();    
  } else { // Switch OFF
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    ledcWrite(ENA,0);

    Serial.println("Enable switch is OFF");
    display.clearDisplay();
    display.setCursor(0,0);
    display.print(F("Switch: OFF"));
    display.display();
    //digitalWrite(BUZZER,LOW);
    
    digitalWrite(BLINK_PIN, HIGH);
    delay(1000);
    digitalWrite(BLINK_PIN, LOW);
    delay(1000);
  }
  //delay(1000);
  if ((x < 1) && (y < 1) && (z < 1)) {
    //display.clearDisplay();
    display.setCursor(100, 0);
    display.printf("RS 3");
    display.display(); 
    digitalWrite(BUZZER, LOW);
    Serial.println("Gempa sangat kecil, tidak perlu evakuasi");
    digitalWrite(green, HIGH);
    digitalWrite(yellow, LOW);
    digitalWrite(red, LOW);
    //send_wa("Gempa kecil, tidak perlu evakuasi");
  } else if ((x<1.5) && (y<1.5) &&(z<1.5)) {
    display.setCursor(100, 0);
    display.printf("RS 3.5");
    display.display(); 
    digitalWrite(BUZZER, LOW);
    Serial.println("Gempa sangat kecil, tidak perlu evakuasi");
    digitalWrite(green, HIGH);
    digitalWrite(yellow, LOW);
    digitalWrite(red, LOW);
    send_wa("Gempa sangat kecil, tidak perlu evakuasi");
    delay(10);
  } else {
    if ((x < 2) && (y < 2) && (z < 2)) {
      //display.clearDisplay();
      display.setCursor(100, 0);
      display.printf("RS 4");
      display.display();
      digitalWrite(BUZZER, LOW);
      Serial.println("Gempa kecil, tetap waspada");
      digitalWrite(green, LOW);
      digitalWrite(yellow, HIGH);
      digitalWrite(red, LOW);
      send_wa("Gempa kecil, tetap waspada");
      delay(10);
    } else if ((x < 3) && (y<3) && (z<3)){
      display.setCursor(100, 0);
      display.printf("RS 4.5");
      display.display();
      digitalWrite(BUZZER, LOW);
      Serial.println("Gempa sedang, berlindung");
      digitalWrite(green, LOW);
      digitalWrite(yellow, HIGH);
      digitalWrite(red, LOW);
      send_wa("Gempa sedang, berlindung");
      delay(10);
    } else {
      if ((x < 4) && (y < 4) && (z < 4)) {
        //display.clearDisplay();
        display.setCursor(100, 0);
        display.printf("RS 5");
        digitalWrite(BUZZER,HIGH);
        Serial.println("Gempa sedang, berlindung");
        digitalWrite(green, LOW);
        digitalWrite(yellow, HIGH);
        digitalWrite(red, LOW);
        send_wa("Gempa sedang, berlindung");
        delay(10);
      } 
      else { 
        if ((x < 6) && (y < 6) && (z < 6)) {
          //display.clearDisplay();
          display.setCursor(100, 0);
          display.printf("RS 6");
          digitalWrite(BUZZER,HIGH);
          Serial.println("Gempa kuat, segera evakuasi");
          digitalWrite(green, LOW);
          digitalWrite(yellow, LOW);
          digitalWrite(red, HIGH);
          send_wa("Gempa kuat, segera evakuasi");
          delay(10);
        } else { 
          digitalWrite(BUZZER,HIGH);
          digitalWrite(green, LOW);
          digitalWrite(yellow, LOW);
          digitalWrite(red, HIGH);
          Serial.println("Gempa sangat kuat");
          send_wa("Gempa kuat, segera evakuasi");
          delay(10);
        }
      }
      display.display();
    }
  }

  delay(500);  // Wait for half a second before next measurement
}

void send_wa(String pesan) {
  static unsigned long lastRequestTime = 0;
  unsigned long currentTime = millis();

  // Check if enough time has passed since the last request
  if (currentTime - lastRequestTime >= 60000) { // 1 minute delay
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String encodedMessage = urlencode(pesan);
      String url = "https://api.callmebot.com/whatsapp.php?phone=62895330508021&text=" + encodedMessage + "&apikey=2603405";
      http.begin(url);
      int httpCode = http.GET();
      if (httpCode > 0) {
        String response = http.getString();
        Serial.println("HTTP Code: " + String(httpCode));
        Serial.println("Response: " + response);
      } else {
        Serial.println("Error sending message: " + String(httpCode));
      }
      http.end();
      lastRequestTime = currentTime; // Update last request time
    } else {
      Serial.println("Error: Not connected to WiFi");
    }
  } else {
    Serial.println("Rate limit: waiting before next request");
  }
}

String urlencode(String str) {
  String encoded = "";
  char c;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == ' ') {
      encoded += "+";
    } else if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
      encoded += c;
    } else {
      encoded += "%";
      encoded += String(c >> 4, HEX);
      encoded += String(c & 0x0F, HEX);
    }
  }
  return encoded;
}
