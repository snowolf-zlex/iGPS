#include <GPSDecoder.h>
#include <LCD4884.h>
#include <SD.h>

GPSDecoder gps;

//#define _SS_MAX_RX_BUFF 256

//SD card attached to SPI bus as follows:
//#define SD_MOSI 11
//#define SD_MISO 12
//#define SD_CLK 13
#define SD_CS 4

File sdFile;

void setup() {
  Serial.begin(4800);
  Serial1.begin(9600);
  
  lcd.init();
  lcd.clear();
  
  if (!SD.begin(SD_CS)) {
    return;
  } 
 
  lcd.backlight(OFF);
}

void loop() {
  readGPS();
  lcdDisplay();
}

void lcdDisplay(){
    if(!gps.isValid()){
      lcd.backlight(OFF);
      return;
    }
    lcd.backlight(ON);
    //lcd.LCD_clear();  
    char dateBuf[8];
    char timeBuf[8];
    char latBuf[20];
    char lonBuf[20];
 
    gps.utcDate().toCharArray(dateBuf,15);
    lcd.write_string(0, 0, dateBuf);
    gps.utcTime().toCharArray(timeBuf,15);
    lcd.write_string(0, 1, timeBuf);

    gps.latitude().toCharArray(latBuf,15);
    lcd.write_string(0, 2, latBuf);
    gps.longitude().toCharArray(lonBuf,15);
    lcd.write_string(0, 3, lonBuf);
     
    // alt
    char altBuf[10]; 
    gps.altitude().toCharArray(altBuf,10);
    lcd.write_string(0, 4, altBuf);
}

void readGPS() {
   String line = readLine();
   if(line.length()) {
      sdFile = SD.open("gps.txt", FILE_WRITE);
      if (sdFile) {
        sdFile.println(line);
        sdFile.close();
      }
      gps.decode(line);
  }
}

//0x0D 0x0A CR LF \r \n
String readLine(){
  String line = "";
//  delay(50);
  while(Serial1.available()) {
      char c = char(Serial1.read());
      Serial.print(c); 
      delayMicroseconds(1500);
      if(c == '\r'){
       //do nothing
      }
      if(c != '\n'){
        line += c;
      } else {
         return line;
      }
  }
}
