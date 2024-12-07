/*
Date:21.11.2024
Author: Axel Söderberg

This code is a test code for the Wireless weather station receiver. The purpose of this code 
is to test that the receiver board is able to receive the temperature data transmitted by the
transmitter board. This code is not the code to be used in the final project, but rather a quick 
test to see that the receiver board works as it should. 
*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RH_ASK.h>
#include <SPI.h> // Not used but needed for compatibility


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void show() { // Often used sequence - Function to simplify code
  display.display(); delay(2000); display.fillScreen(SSD1306_BLACK);
}
/*
PINS:
internal led PB2
RSSI input PC0
ask input PD7
Shutdown PD2
*/

// Create an instance of the RadioHead driver

RH_ASK driver(1000, 7, 9, 15); // (bitrate, RX pin, TX pin, PTT pin)

union FloatToBytes {
    float floatValue;
    byte byteArray[sizeof(float)];
};

void receiveFloat() {
    FloatToBytes converter;
    uint8_t buf[sizeof(float)];
    uint8_t buflen = sizeof(buf);
    
    // Receive the packet containing the float (4 bytes)
    if (driver.recv(buf, &buflen)) {
        // Copy received bytes into the converter byteArray
        memcpy(converter.byteArray, buf, sizeof(float));
        
        // Convert byte array back to float
        float receivedValue = converter.floatValue;

        //Serial.print("Received float: ");
        Serial.println(receivedValue, 2); // Print float with 5 decimal places for precision
    }
}

void setup() {
  Serial.begin(115200);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Old Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
    }
    // Initialize the RadioHead driver
    driver.init();
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);

 
}

void loop() {
    digitalWrite(2, LOW);
    receiveFloat();
    display.clearDisplay();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(28,25);
    display.println("Screen");
    display.setCursor(28,33);
    display.println("Primitives");
    display.display(); 
    randomSeed(analogRead(3));
    display.fillScreen(SSD1306_WHITE);              // Fill screen WHITE
    display.display();  
    display.fillScreen(SSD1306_BLACK);              // Fill screen BLACK
    display.display();
    
    delay(1000); // Wait for a second before receiving again
}