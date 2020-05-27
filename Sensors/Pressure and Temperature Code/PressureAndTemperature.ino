

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11 
#define BMP_CS 10

Adafruit_BMP280 bme; // I2C
//Adafruit_BMP280 bme(BMP_CS); // hardware SPI
//Adafruit_BMP280 bme(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

float Temperature, Pressure;

void setup() 
{
  Serial.begin(9600);
 
}

float ReadTemperature(void)
{
  float T;
  T=(bme.readTemperature()*1.8)+32;
  return T;
}

float ReadPressure(void)
{
  float P;
  P=(bme.readPressure()/1000);
  return P;
}


void loop() {

    Temperature=ReadTemperature();
    Serial.print("Temperature = ");
    Serial.print(Temperature);    Serial.println(" *F");
    Pressure=ReadPressure();
    Serial.print("Pressure = ");
    Serial.print(Pressure);
    Serial.println(" kPa");
    Serial.println();
    delay(2000);
}
