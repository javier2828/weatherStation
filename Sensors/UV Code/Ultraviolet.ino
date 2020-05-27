// Watch video here: https://www.youtube.com/watch?v=jFP13uhgy04

/* 

Connection:

 Arduino           Ultraviolet (UV) sensor
 A0 (Analog 0)          SIG
 +5V                    VCC
 GND                    GND
 
*/

// UV index explanation: http://www2.epa.gov/sunwise/uv-index-scale
// UV index explanation: http://www.epa.gov/sunwise/doc/what_is_uvindex.html

int UV;

void setup()
{
  Serial.begin(9600);
} 
int GetUV(void)
{  
  float Vsig;
  int sensorValue;
  long  sum=0;
  
  for(int i=0;i<1024;i++)
  {  
      sensorValue=analogRead(A0);
      sum=sensorValue+sum;
      delay(2);
  }  
   
  sum = sum >> 10;
  Vsig = sum*4980.0/1023.0;
 
  if (Vsig < 50)
  {
      return 0;
  }
  if (Vsig > 50 && Vsig < 227) 
  {
      return 1;
  }
  if (Vsig > 227 && Vsig < 318)
  {
      return 2;
  }
  if (Vsig > 318 && Vsig < 408) 
  {
      return 3;
  }
  if (Vsig > 408 && Vsig < 503) 
  {
      return 4;
  }
  if (Vsig > 503 && Vsig < 606) 
  {
      return 5;
  }
  if (Vsig > 606 && Vsig < 696) 
  {
      return 6;
  }
  if (Vsig > 696 && Vsig < 795) 
  {
      return 7;
  }
  if (Vsig > 795 && Vsig < 881) 
  {
      return 8;
  }
  if (Vsig > 881 && Vsig < 976) 
  {
      return 9;
  }
  if (Vsig > 976 && Vsig < 1079) 
  { 
      return 10;
  }
  if (Vsig > 1079 && Vsig < 1170)
  {   
      return 11;
  }
  if (Vsig > 1170) 
  {
      return 12;
  }
}

void loop()
{
    UV=GetUV();
    Serial.print("UV Level = ");
    Serial.print(UV);    
    Serial.println();
    delay(2000);
}

