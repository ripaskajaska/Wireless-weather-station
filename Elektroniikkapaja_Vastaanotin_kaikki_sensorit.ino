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

void receiveFloats() {
    float buf[4];
    uint8_t buflen = sizeof(buf);
    
    // Receive the packet containing the float (4 bytes)
    if (driver.recv((uint8_t*)buf, &buflen)) {
        
        // get values of buffer and serial print them out
        Serial.print("Temperature: ");
        Serial.print(buf[0], 2);
        Serial.println("°C");

        Serial.print("Pressure: ");
        Serial.print(buf[1], 2);
        Serial.println(" hPa");

        Serial.print("Humidity: ");
        Serial.print(buf[2], 2);
        Serial.println("%");

        Serial.print("Battery voltage: ");
        Serial.print(buf[3], 2);
        Serial.println(" V");
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
    receiveFloats();
    delay(1000); // Wait for a second before receiving again
}