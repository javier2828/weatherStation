/*  Arduino Wind Direction Module
 *  Boris Dostinov
 */

//Pins used
const int analogInPin = A0;  

int sensorValue = 0;
int direction_degrees;

void setup() {
  // initialize serial communications at 115200 bps:
  Serial.begin(115200);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);

  direction_degrees = map(sensorValue, 0, 1023, 0, 359);
  direction_degrees = direction_degrees + 90; //90 degree offset

  if( direction_degrees >= 23 && direction_degrees <= 67)
    Serial.print("North East");
  else if( direction_degrees >= 68 && direction_degrees <= 112)
    Serial.print("East");
  else if( direction_degrees >= 113 && direction_degrees <= 157)
    Serial.print("South East");
  else if( direction_degrees >= 158 && direction_degrees <= 202)
    Serial.print("South");
  else if( direction_degrees >= 203 && direction_degrees <= 247)
    Serial.print(South West");
  else if( direction_degrees >= 248 && direction_degrees <= 292)
    Serial.print("West");
  else if( direction_degrees >= 293 && direction_degrees <= 22)
    Serial.print("North");
    
  }
}
