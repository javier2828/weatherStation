/*
Arduino Hall Effect Sensor Project
by Arvind Sanjeev
Please check out  http://diyhacking.com for the tutorial of this project.
DIY Hacking
*/


// volatile byte half_revolutions;
volatile byte full_revolutions;

 unsigned int rpm;
 unsigned long timeold;

 void setup()
 {
   Serial.begin(115200);
   pinMode(2, INPUT);
   attachInterrupt(0, magnet_detect, FALLING);//Initialize the intterrupt pin (Arduino digital pin 2);
   full_revolutions = 0;
   rpm = 0;
   timeold = 0;
 }
 void loop()//Measure RPM
 {
   if (full_revolutions >= 20) { 
     //rpm = 30*1000/(millis() - timeold)*half_revolutions;
     rpm = 60*1000/(millis() - timeold)*full_revolutions;
     timeold = millis();
     full_revolutions = 0;
     Serial.println(rpm,DEC);
   }
 }
 void magnet_detect()//This function is called whenever a magnet/interrupt is detected by the arduino
 {
   full_revolutions++;
   Serial.println("detect");
 }

 
