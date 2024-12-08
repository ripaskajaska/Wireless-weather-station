#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define SoftwareSerial pins
#define RX_PIN 10 // Pin connected to the TX of the external device
#define TX_PIN 11 // Pin connected to the RX of the external device


// Initialize SoftwareSerial
SoftwareSerial softSerial(RX_PIN, TX_PIN);

void setup() {
  // Initialize SoftwareSerial
  softSerial.begin(9600); // Use the baud rate of the external device
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Use the correct I2C address
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Waiting for data..."));
  display.display();
}

void loop() {
  // Check if data is available on SoftwareSerial
  if (softSerial.available() > 0) {
    // Read the float value
    float temperature = softSerial.parseFloat();
    // Display the temperature on the OLED screen
    display.clearDisplay();               // Clear the display
    display.setCursor(20, 10);              // Reset cursor position
    display.setTextSize(2);               // Set text size for temperature
    display.print(F("Temp: "));           // Label
    display.setTextSize(3);               // Set text size for temperature
    display.setCursor(20, 30);
    display.print(temperature, 2);        // Display temperature with 2 decimal points
    display.display();                    // Display updated content
    if (softSerial.available() > 0) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(5);
      float buf_clean = softSerial.parseFloat();
      digitalWrite(LED_BUILTIN, LOW);
    }

    
  }
   
}

