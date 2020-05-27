
volatile byte full_revolutions;
unsigned long timeold;
float rpm;
float pi = 3.14159;
float radius = 4.3; //4.3 inches
float mph;
float ipm; // inches per minute

void setup()
{
  Serial.begin(115200);
  pinMode(2, INPUT);
  attachInterrupt(0, magnet_detect, FALLING);//Initialize the intterrupt pin (Arduino digital pin 2);
  full_revolutions = 0;
  rpm = 0;
  timeold = 0;
  mph = 0;
  ipm = 0;
   
}
 
void loop()//Measure RPM
{
  if (full_revolutions >= 5) 
  {
    rpm = (float)60*1000UL/(millis() - timeold)*full_revolutions;
    timeold = millis();
    full_revolutions = 0;
    Serial.println(rpm,DEC);
    ipmToMph();
   }
  else if (millis() - timeold == 1000 && full_revolutions == 0) 
  { 
    rpm = 0;
    timeold = millis();
    full_revolutions = 0;
    Serial.println(rpm,DEC);
    ipmToMph();
  }
}
 
void magnet_detect()//This function is called whenever a magnet/interrupt is detected by the arduino
{
  full_revolutions++;
  Serial.println("detect");
}

void ipmToMph()
{
  ipm = rpm*radius*2*pi;
  mph = ipm*60/63360UL;
  Serial.println(mph,DEC);
}
 
