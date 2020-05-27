// Weather Station Project Group #1

//Necessary Libraries
#include <ThingSpeak.h>
#include <BridgeClient.h>
#include <DHT.h>
#include <Wire.h>
#include <LCD16x2.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define DHTPIN 8
#define DHTTYPE DHT11
#define TMPPIN A0
#define UVPIN A2
#define WDPIN A3
#define WSPIN 7

BridgeClient client;
LCD16x2 lcd1;
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bme;

long previous_time = 0;
long previous_time2 = 0;
int offset = 0;
volatile int full_revolutions = 0;
unsigned long timeold = 0;

void setup(){
  //Initialize libraries
  Serial.begin(9600);
  Bridge.begin();
  ThingSpeak.begin(client);
  Wire.begin();
  //dht.begin();
  bme.begin();
  analogReference(DEFAULT);
  pinMode(WSPIN, INPUT);
  attachInterrupt(4, magnet_detect, FALLING);//Initialize the intterrupt pin (Arduino digital pin 2);
  
}

long timeDelta(){
  //Change in the amount of time
  return millis() - previous_time;
}

long timeDelta2(){
  //Change in the amount of time
  return millis() - previous_time2;
}


void loop(){
  //Main loop
  displayButtons("","CR","SET","");
  if (millis()-previous_time2 >= 60000){//6s measure interval
    uploadData();
    previous_time2 = millis();
  }
  if (timeDelta >= 200){//200: delay for buttons
//    if (lcd1.readButtons() & 0x01) {
//      //System information button (SI)
//    }else
//      display_systemstatus();
    int buttons = lcd1.readButtons();
    if (buttons & 0x02) {
      //Current readings button (CR)
    }else{
      Serial.println("here first");
      display_datavalues("Working");
      display_current_data();
    }
    if (buttons & 0x04) {
      //Settings button (SET)
    }else{
      display_settings();
    }
    previous_time = millis();
  }
}

void uploadData() {
  Serial.println("UPLOAD");
  ThingSpeak.setField(1, String(get_temp()));
  ThingSpeak.setField(2, String(0));
  ThingSpeak.setField(3, String(get_hum()));
  ThingSpeak.setField(4, String(get_ws()));
  ThingSpeak.setField(5, get_wd());
  ThingSpeak.setField(6, get_uv());
  ThingSpeak.writeFields(231412, "X0YP8LMV5LCELAA7");
  
}

float get_temp() {
  float TempF = analogRead(TMPPIN) * 0.004882814;
  TempF = (TempF - 0.50) * 100;
  TempF = TempF * (9.0 / 5.0) + 32.0;
  //int TempF2 = (bme.readTemperature() * 1.8) + 32;
  //float avg = (TempF + TempF2) / 2;
  //avg = round(avg * 10) / 10.0;
  return TempF;
}

float get_pressure() {
  float P = 0;
  P = (bme.readPressure() / 1000);
  return P;
}

void magnet_detect()//This function is called whenever a magnet/interrupt is detected by the arduino
{
  full_revolutions++;
}

float get_ws() 
{
  
  float rpm = 0;
  float pi = 3.14159;
  float radius = 4.3; //4.3 inches
  float mph = 0;
  
  if (full_revolutions >= 5) 
  {
    rpm = 60*1000UL/(millis() - timeold)*full_revolutions;
    timeold = millis();
    full_revolutions = 0;
    mph = rpm*radius*2*pi*60/63360UL;
    return mph;
   }
   else
    return 0;
}

String get_wd() {
  int sensorValue = analogRead(WDPIN);
  int direction_degrees = abs((map(sensorValue, 0, 1023, 0, 359)-offset));
  if( direction_degrees >= 16 && direction_degrees <= 135)
    return "West";
  if( direction_degrees >= 136 && direction_degrees <= 225)
    return "South";
  if( direction_degrees >= 226 && direction_degrees <= 315)
    return "East";
  if( direction_degrees >= 316 || direction_degrees <= 15)
    return "North";
}

String get_uv() {
  float Vsig;
  int sensorValue;
  long  sum = 0;
  for (int i = 0; i < 1024; i++)
  {
    sensorValue = analogRead(UVPIN);
    sum = sensorValue + sum;
    delay(2);
  }
  sum = sum >> 10;
  Vsig = sum * 4980.0 / 1023.0;
  if (Vsig < 50)
    return "0";
  else if (Vsig > 50 && Vsig < 227)
    return "1";
  else if (Vsig > 227 && Vsig < 318)
    return "2";
  else if (Vsig > 318 && Vsig < 408)
    return "3";
  else if (Vsig > 408 && Vsig < 503)
    return "4";
  else if (Vsig > 503 && Vsig < 606)
    return "5";
  else if (Vsig > 606 && Vsig < 696)
    return "6";
  else if (Vsig > 696 && Vsig < 795)
    return "7";
  else if (Vsig > 795 && Vsig < 881)
    return "8";
  else if (Vsig > 881 && Vsig < 976)
    return "9";
  else if (Vsig > 976 && Vsig < 1079)
    return "10";
  else if (Vsig > 1079 && Vsig < 1170)
    return "11";
  else if (Vsig > 1170)
    return "12";
}

float get_hum() {
  float h = dht.readHumidity();
  return h;
}

void display_datavalues(String datastr) {
  //Displays the specified data string in the first row
  char chararray[16];
  datastr.toCharArray(chararray, 16);
  lcd1.lcdGoToXY(1, 1);
  lcd1.lcdWrite(chararray);
}

void displayButtons(String But1, String But2, String But3, String But4) {
  //Displays specified buttons
  char chararray[4];
  
  But1.toCharArray(chararray,4);
  lcd1.lcdGoToXY(1, 2);
  lcd1.lcdWrite(chararray);
  But2.toCharArray(chararray,4);
  lcd1.lcdGoToXY(5, 2);
  lcd1.lcdWrite(chararray);
  But3.toCharArray(chararray,4);
  lcd1.lcdGoToXY(10, 2);
  lcd1.lcdWrite(chararray);
  But4.toCharArray(chararray,4);
  lcd1.lcdGoToXY(15, 2);
  lcd1.lcdWrite(chararray);
}

void displayScreen(String valuesstr, String menu) {
  //Displays desired menu buttons and desired string.
  lcd1.lcdClear();
  if (menu == "data")
    displayButtons("","<-","->","EX");
  else if (menu == "settings")
    displayButtons("Sel","","","EX");
  display_datavalues(valuesstr);
}

void display_current_data() {
  //Function that scrolls through the current sensor readings
  String temp_string;
  int data_index = 0;
  int data_count = 5;
  String current_data[data_count];
  bool isrunning = true;
  int prev_index = data_index;
  
  current_data[0] = String("Temp: " + String(get_temp()) + " F"); 
  current_data[1] = String("Hum: " + String(get_hum()) + " %");
  current_data[2] = String("UV: " + get_uv());
  current_data[3] = String("WD: " + get_wd()); 
  current_data[4] = String("WS: " + String(get_ws()) + " mph"); 
  //current_data[5] = String("Press: " + String(get_pressure()) + " kPa");

  lcd1.lcdClear();
  displayScreen(current_data[data_index], "data");
  while (isrunning) {
    int buttons = lcd1.readButtons();
    if ( buttons & 0x02) {
      //Back arrow
    } else {
      data_index--;
    }
    if (buttons & 0x04) {
      //Foward arrow
    } else {
      data_index ++;
    }
    if (buttons & 0x08) {
      //Exit button
    } else {
      isrunning = false;
      lcd1.lcdClear();
    }
    //TODO: Does this cause unnecessary refreshing?
    if (data_index >= data_count) {
      data_index = 0;
    } else if (data_index < 0) {
      data_index = data_count - 1;
    }
    if (prev_index != data_index){
      displayScreen(current_data[data_index], "data");
      prev_index = data_index;
    }
    
    delay(200);
  }
}

//void display_systemstatus() {
//  //Displays the system status
//  bool isrunning = true;
//  
//  lcd1.lcdClear();
//  lcd1.lcdGoToXY(1, 1);
//  lcd1.lcdWrite("System OK");//TODO: This is just a placeholder
//  displayButtons("","","","EX");
//  
//  while (isrunning) {
//    if (lcd1.readButtons() & 0x08) {
//      //Exit button
//    } else {
//      isrunning = false;
//    }
//    
//    delay(200);
//  }
//}

void display_settings() {
  //Displays the settings menu
  int settings_index = 0;
  int settings_count = 1;
  String settings[settings_count];
  bool isrunning = true;
  int prev_i = settings_index;
  
  settings[0] = "Calibrate North";
  //settings[1] = "WiFi";

  lcd1.lcdClear();
  displayScreen(settings[settings_index], "settings");
  
  while (isrunning) {
//    Loop detecting button clicks.
    if (lcd1.readButtons() & 0x01) {
      //Select button
    } else {
      //Check which setting was selected via the settings index
//      if (settings_index == 0) {
        //Calibrate north
      calibrateNorthSetting();
      isrunning = false;
      lcd1.lcdClear();
//      }
    }
//    if (lcd1.readButtons() & 0x02) {
//      //Back arrow
//    } else {
//      settings_index--;
//    }
//    if (lcd1.readButtons() & 0x04) {
//      //Foward arrow
//    } else {
//      settings_index ++;
//    }
    if (lcd1.readButtons() & 0x08) {
      //Exit button
    } else {
      isrunning = false;
      lcd1.lcdClear();
    }
    
//    if (settings_index >= settings_count) {
//        settings_index = 0;
//      } else if (settings_index < 0) {
//        settings_index = settings_count - 1;
//      } else {}
//    displayScreen(settings[settings_index], "settings");
    delay(200);
  }
}

void calibrateNorth() {
  int sensorValue = analogRead(WDPIN);
  int direction_degrees = map(sensorValue, 0, 1023, 0, 359);
  offset = direction_degrees;
}

void calibrateNorthSetting() {
  //Shows the calibrate north settings screen
  bool isrunning = true;
  int txt_length = 32;
  String text = "Align marker to north & hit SET";
  int start_i = 0;
  int stop_i = 15;
  char chararray[16];
  String disp_txt;
  
  while (isrunning) {
    lcd1.lcdClear();
    displayButtons("","SET","","EX");
    
    //Display the text via scrolling.
    lcd1.lcdGoToXY(1, 1);
    disp_txt = text.substring(start_i, stop_i);
    disp_txt.toCharArray(chararray, 16);
    lcd1.lcdWrite(chararray);
    if (stop_i >= txt_length) {
      start_i = 0;
      stop_i = 15;
    } else {
      start_i += 3;
      stop_i += 3;
    }
    
    if (lcd1.readButtons() & 0x02) {
      //Set button
    } else {
      calibrateNorth();
      isrunning = false;
      lcd1.lcdClear();
    }
    if (lcd1.readButtons() & 0x08) {
      //Exit button
    } else {
      isrunning = false;
      lcd1.lcdClear();
    }
    delay(700);
  }
}






