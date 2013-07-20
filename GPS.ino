#include <GPSDecoder.h>
#include <SoftwareSerial.h>

#define GPS_TX 12
#define GPS_RX 13

// GPS Serial
SoftwareSerial GPSSerial(GPS_TX, GPS_RX); // RX, TX

// GPSLib gps(&GPSSerial);
GPSDecoder gps;


void setup() {
  Serial.begin(4800);
  GPSSerial.begin(9600);
}

void loop() {
//  Serial.println(gps.toDouble("-123.405"));
String line = readLine();
if(line.length()<=0){
 return; 
}
//Serial.print("line: ");
Serial.println(line);
Serial.println("---------------------------------");

gps.decode(line);
  Serial.println(gps.utcDate());
  Serial.println(gps.utcTime());
  Serial.println(gps.longitude());
  Serial.println(gps.latitude());
  Serial.println(gps.altitude());
  Serial.println(gps.isValid());
Serial.println("--------------+++++++++++------------");
}

String readLine(){
  String line = "";
  while(GPSSerial.available()) {
      char c = char(GPSSerial.read());
      Serial.print(c);
      if(c == '\r'){
        // do nothing
        continue;
      }
      if(c != '\n'){
        line += c;
      } else {
         return line;
      }
  }
}
