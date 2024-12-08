/*
Date: 21.11.2024 
Author: Axel Söderberg

This code is a test code for the Wireless weather station transmitter. The purpose of this program is to
test that the transmitter board is able to read the temperature data from DS1820 temperature sensor and
transmit the data with the dedicated transmitter. This code is not the code to be used in the final 
project, but rather a quick test to see if the most vital things work as they should on the 
pcb. 
*/
#include <RH_ASK.h>
#include <SPI.h>  // Not used but needed for compatibility
#include <OneWire.h>
#include <DallasTemperature.h>

/*pins
ask transmit PB4 pin 4
ds1820 one wire PC2 pin 18 
internal led PB3 pin 3
*/
#define ONE_WIRE_BUS 18

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);


// Create an instance of the RadioHead driver
RH_ASK driver(2000, PA2, 4, PA3);  // (bitrate, RX pin, TX pin, PTT pin)

union FloatToBytes {
  float floatValue;
  byte byteArray[sizeof(float)];
};

// Function to send a float value
void sendFloat(float value) {
  FloatToBytes converter;
  converter.floatValue = value;

  // Send the entire float (4 bytes) as a single packet
  driver.send(converter.byteArray, sizeof(converter.byteArray));
  driver.waitPacketSent();  // Wait until the packet is sent
}

void setup() {
  // Initialize the RadioHead driver
  driver.init();
  sensors.begin();
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
}

void loop() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  sendFloat(tempC);
  digitalWrite(3, HIGH);
  delay(5);  // Wait for a second before sending again
  digitalWrite(3, LOW);
  delay(2000);
}
