/*
Date:21.11.2024
Author: Axel Söderberg

This code is a test code for the Wireless weather station receiver. The purpose of this code 
is to test that the receiver board is able to receive the temperature data transmitted by the
transmitter board. This code is not the code to be used in the final project, but rather a quick 
test to see that the receiver board works as it should. 
*/
#include <RH_ASK.h>
#include <SPI.h> // Not used but needed for compatibility

/*
PINS:
internal led PB2
RSSI input PC0
ask input PD7
Shutdown PD2
*/

// Create an instance of the RadioHead driver

RH_ASK driver(2000, 7, 9, 15); // (bitrate, RX pin, TX pin, PTT pin)

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

        Serial.print("Measured temperature: ");
        Serial.print(receivedValue, 2); // Print float with 5 decimal places for precision
        Serial.println("°C");
    }
}

void setup() {
    // Initialize the RadioHead driver
    driver.init();
    Serial.begin(9600);
    pinMode(2, OUTPUT);
    pinMode(4, OUTPUT); //reset for atmega328 driving oled
    digitalWrite(2, LOW);
    digitalWrite(4, LOW);
    delay(1000);
    digitalWrite(4, HIGH);

}

void loop() {
    digitalWrite(2, LOW);
    receiveFloat();
    
    delay(1000); // Wait for a second before receiving again
}
