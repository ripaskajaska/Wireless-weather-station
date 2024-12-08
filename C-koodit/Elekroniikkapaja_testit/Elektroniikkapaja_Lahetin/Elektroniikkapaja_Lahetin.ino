//Include libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <RH_ASK.h>
#include <SPI.h> 
#include <Adafruit_BMP280.h>
#include <Wire.h>

//pin definition
#define ONE_WIRE_BUS (PC2)
#define BMP_SCK  (PB7)
#define BMP_MISO (PB6)
#define BMP_MOSI (PB7)
#define BMP_CS   (PB2)
#define address 0x40 //SEN0546 I2C address

//global variables
float temp = 0.0;
float pressure = 0.0;
float humidity = 0.0; 
float battery_voltage = 0.0;
uint8_t buf[4] = {0};
uint16_t rawHumidity;

//function prototypes
void transmit_data_packet()
void measure_barometric_pressure(float* pressure_ptr);
void measure_relative_humidity(float* humidity_ptr);
void measure_temperature(float* temp_ptr);
void measure_battery_voltage(float* battery_voltage);
uint8_t readReg(uint8_t reg, void* pBuf, size_t size);
//void measure_brightness();


//Initialize dallas temp ds1820
OneWire oneWire(ONE_WIRE_BUS);        // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature. 

//initialize BMP280
Adafruit_BMP280 bmp(BMP_CS); // hardware SPI

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);   //battery level
  pinMode(A1, INPUT);   //LDR
  pinMode(PB1, OUTPUT); //CS LDR
  pinMode(PB2, OUTPUT); //CS BMP280
  pinMode(PB3, OUTPUT); //Int led
  pinMode(PB4, OUTPUT); //transmit ASK data pin
  pinMode(PC3, OUTPUT); //CS battery level

  digitalWrite(PC3, HIGH);

   //settings    KATSO TARVITSEEKO TÄTÄ!!!!!
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  //initialize sensors
  sensors.begin();
  bmp.begin();
}

void loop() {

  //measure temperature
  measure_temperature(&temp);

  //measrue barometric pressure
  measure_barometric_pressure(&pressure);

  //measure relative humidity
  measure_relative_humidity(&humidity);
}

void measure_temperature(float* temp_ptr){
   sensors.requestTemperatures(); // Send the command to get temperatures
  * temp_ptr = sensors.getTempCByIndex(0);
}

void measure_barometric_pressure(float* pressure_ptr){
  bmp.takeForcedMeasurement(); // Send the command to get barometric pressure data
  * pressure_ptr = bmp.readPressure();
}

uint8_t readReg(uint8_t reg, void* pBuf, size_t size) {
  if (pBuf == NULL) {
    Serial.println("pBuf ERROR!! : null pointer");
    return 0;
  }
  uint8_t* _pBuf = (uint8_t*)pBuf;
  Wire.beginTransmission(address);
  Wire.write(&reg, 1);
  if (Wire.endTransmission() != 0) {
    return 0;
  }
  delay(20); // Small delay for sensor response
  Wire.requestFrom(address, (uint8_t)size);
  for (uint16_t i = 0; i < size; i++) {
    _pBuf[i] = Wire.read();
  }
  return size;
}

void measure_relative_humidity(float* humidity_ptr){
   if (hum == NULL) {
    Serial.println("Error: null pointer passed to measureHumidity");
    return;
  }

  uint8_t buf[4] = {0};
  uint16_t rawHumidity;

  // Read humidity register (0x00 is assumed as register for simplicity)
  if (readReg(0x00, buf, 4)) {
    rawHumidity = buf[2] << 8 | buf[3];
    *hum = ((float)rawHumidity / 65535.0) * 100; // Update the value at the pointer
  } 
}

void measure_battery_voltage(float* battery_voltage){
  digitalWrite(PC3, LOW); //cs enable for reading votlage
  * battery_voltage = analogRead(A0);
  digitalWrite(PC3, HIGH);
}

