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

void displayData(const char* label, float value, const char* unit) {
  display.clearDisplay();               // Clear the display
  display.setCursor(0, 10);             // Set cursor position
  display.setTextSize(2);               // Set text size for label
  display.print(label);                 // Print label
  display.setTextSize(3);               // Set text size for value
  display.setCursor(0, 30);
  display.print(value, 2);              // Display value with 2 decimal points
  display.print(" ");
  display.print(unit);                  // Display unit
  display.display();                    // Update display
}

void setup() {
  // Initialize SoftwareSerial
  softSerial.begin(9600); // Use the baud rate of the external device

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
  float data[4];

  // Check if data is available on SoftwareSerial
  if (softSerial.available() > 0) {
    // Read 4 float values sequentially
    for (int i = 0; i < 4; i++) {
      data[i] = softSerial.parseFloat();
    }

    // Display each value with a 10-second delay
    displayData("Temp:", data[0], "C");
    delay(10000);

    displayData("Press:", data[1], "hPa");
    delay(10000);

    displayData("Hum:", data[2], "%");
    delay(10000);

    displayData("Bat:", data[3], "V");
    delay(10000);
  }
}

