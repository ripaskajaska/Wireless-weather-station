#include <OneWire.h>
#include <RH_ASK.h>
#include <DallasTemperature.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <SPI.h>

// Pin definitions for ATmega644A
#define ONE_WIRE_BUS 18         // For Dallas DS1820
#define BMP_CS PB2               // Use PB4 as Chip Select for BMP280
#define ADDRESS 0x40             // SEN0546 I2C address

// Global variables
float temp = 0.0;
float pressure = 0.0;
float humidity = 0.0;
float battery_voltage = 0.0;
uint8_t buf[4] = {0};
float floatValueArray[4] = {0.0};
uint16_t rawHumidity;

// Initialize Dallas Temp DS1820
OneWire oneWire(ONE_WIRE_BUS);        // OneWire instance
DallasTemperature sensors(&oneWire); // Dallas Temperature instance

// Initialize BMP280
Adafruit_BMP280 bmp(BMP_CS); // Hardware SPI

// Create an instance of the RadioHead driver
RH_ASK driver(2000, PA2, 4, PA3);  // (bitrate, RX pin, TX pin, PTT pin)

// Function to send a float array

// Function prototypes
void measure_temperature(float* temp_ptr);
void measure_barometric_pressure(float* pressure_ptr);
void measure_relative_humidity(float* humidity_ptr);
void measure_battery_voltage(float* battery_voltage_ptr);
uint8_t readReg(uint8_t reg, void* pBuf, size_t size);

void sendFloatArray(const float* values, size_t count) {
  driver.send((uint8_t*)values, count * sizeof(float));
  driver.waitPacketSent();
}

void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);
  while (!Serial);

  // Initialize I2C
  Wire.begin(); // This uses PC0 (SCL) and PC1 (SDA) by default on ATmega644A

  // Initialize sensors
  sensors.begin();

  if (!bmp.begin(BMP_CS)) { // Initialize BMP280 with SPI
    Serial.println("Error: BMP280 not found!");
    while (1); // Halt if BMP280 initialization fails
  }

  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);

  // Indicate setup completion
  Serial.println("Setup completed. Starting measurements...");
  pinMode(BMP_CS, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
}

void loop() {
  // Measure temperature
  measure_temperature(&temp);
  if (temp == -127.0) {
    Serial.println("Error: Failed to read DS1820 temperature sensor. Retrying...");
  } else {
    Serial.println("lämpö mitattu");
  }
  floatValueArray[0] = temp;

  // Measure barometric pressure
  measure_barometric_pressure(&pressure);
  Serial.println("paine mitattu");
  floatValueArray[1] = pressure;


  // Measure relative humidity
  measure_relative_humidity(&humidity);
  Serial.println("kosteus mitattu");
  floatValueArray[2] = humidity;

  // Measure battery voltage
  measure_battery_voltage(&battery_voltage);
  Serial.println("akun jännite mitattu");
  floatValueArray[3] = battery_voltage;

  // Print results to Serial Monitor
  Serial.println("Sensor Readings:");
  Serial.print("Temperature (C): ");
  Serial.println(temp);
  Serial.print("Pressure (Pa): ");
  Serial.println(pressure);
  Serial.print("Humidity (%): ");
  Serial.println(humidity);
  Serial.print("Battery Voltage (V): ");
  Serial.println(battery_voltage);

  sendFloatArray(&floatValueArray, 4);
  digitalWrite(3, HIGH);
  delay(5);  // Wait for a second before sending again
  digitalWrite(3, LOW);
  delay(2000);
}

// Function to measure temperature
void measure_temperature(float* temp_ptr) {
  sensors.requestTemperatures();
  *temp_ptr = sensors.getTempCByIndex(0);
  // Retry logic for invalid readings
  if (*temp_ptr == -127.0) {
    delay(200); // Short delay before retry
    sensors.requestTemperatures();
    *temp_ptr = sensors.getTempCByIndex(0);
  }
}

// Function to measure barometric pressure
void measure_barometric_pressure(float* pressure_ptr) {
  bmp.takeForcedMeasurement();
  *pressure_ptr = bmp.readPressure();
}

// Function to measure relative humidity
void measure_relative_humidity(float* humidity_ptr) {
  if (humidity_ptr == NULL) {
    Serial.println("Error: Null pointer passed to measure_relative_humidity");
    return;
  }

  uint8_t buf[4] = {0};
  if (readReg(0x00, buf, 4)) { // Adjust register address as per sensor datasheet
    uint16_t rawHumidity = (buf[2] << 8) | buf[3];
    *humidity_ptr = ((float)rawHumidity / 65535.0) * 100.0;
  } else {
    Serial.println("Error: Failed to read humidity data");
  }
}

// Function to measure battery voltage
void measure_battery_voltage(float* battery_voltage_ptr) {
  digitalWrite(19, LOW);          // Enable CS for battery level
  delay(10);
  *battery_voltage_ptr = analogRead(A0) * (((5.0+0.03) / 1023.0)); // skaalaa tämä vielä
  delay(10);
  digitalWrite(19, HIGH);         // Disable CS
}

// Function to read a register via I2C
uint8_t readReg(uint8_t reg, void* pBuf, size_t size) {
  if (pBuf == NULL) {
    Serial.println("Error: Null pointer passed to readReg");
    return 0;
  }

  Wire.beginTransmission(ADDRESS);
  Wire.write(reg);
  if (Wire.endTransmission() != 0) {
    return 0;
  }

  delay(20); // Allow sensor response time
  Wire.requestFrom((uint8_t)ADDRESS, (uint8_t)size);
  for (size_t i = 0; i < size; i++) {
    ((uint8_t*)pBuf)[i] = Wire.read();
  }
  return size;
}
