/*
 * GPS解析
 * 包括：
 * 时间、日期、经纬度、海拔
 */
#include <SoftwareSerial.h>
#include "LCD4884.h"

#define GPS_TX 12
#define GPS_RX 13

#define GPRMC "$GPRMC" // 推荐最小定位信息
#define GPVTG "$GPVTG" // 地面速度信息
#define GPGGA "$GPGGA" // 获取海拔高度
#define GPZDA "$GPZDA" // UTC时间/日期及本地时区偏移量

#define VALID "A" // A=Valid 有效
#define INVALID "V" // V=Invalid 无效

#define LOCAL_AREA 8 // 本地时区 TODO：使用$GPZDA替换

#define CHAR_DEGREE char(176) // 度
#define CHAR_MINUTE char(39)  // 分
#define CHAR_SECOND char(34)  // 秒

// joystick pin
#define KEY_PIN 0

// joystick number
enum KEY{
      LEFT_KEY, 
      CENTER_KEY, 
      DOWN_KEY, 
      RIGHT_KEY, 
      UP_KEY
};

#define NUM_KEYS 5

int ADC_KEY[5] = { 50, 200, 400, 600, 800 };

// GPS Serial
SoftwareSerial GPSSerial(GPS_TX, GPS_RX); // RX, TX

boolean LIGHT_STATUS = 0;

/*
* GPS Infomation
* $GPRMC,135142.00,V,,,,,,,130713,,,N*7A
* $GPVTG,,,,,,,,,N*30
* $GPGGA,135142.00,,,,,0,00,99.99,,,,,,*66
* $GPZDA,135142.00,13,07,2013,00,00*63
*/
struct GPS_INFO {
  String time; // UTC Time
  String localTime; // Local Time
  int hour;
  int minute;
  int second;
  boolean isValid; // A=Valid,V=Invalid
  String lat; // ddmm.mmmm Latitude 纬度
  int degreeOfLat;
  int minuteOfLat;
  int secondOfLat;
  String northingIndicator; // N=North,S=South 南北航向指示器
  String latString;
  String lon; // dddmm.mmmm Longitude 经度
  int degreeOfLon;
  int minuteOfLon;
  int secondOfLon;
  String eastingIndicator; // E=East,W=West 东西航向指示器
  String lonString;
  double kn; // knots Speed Over Ground 实际航速(海里)
  double kmph;// KM pre Hour 1knots = 1.85200km/h
  String cog; // °     Course Over Ground (true) 实际航迹向
  String date; // UTC Date 190203 ddmmyy     Universal time coordinated
  String day;
  String month;
  String year;
  double alt; // altitude
  String altString;
  String sogString; // Speed Over Ground Km/h
  String magneticVariationDegree; // ° Magnetic Variation 磁场变化的角度
  char magneticVariationDirection; //     E=East,W=West 磁场变化的方向
  String modeIndicator; // A A=Autonomous, D=Differential, E=Dead Reckoning, N=None
}gps;

void setup(){
  Serial.begin(4800);
  Serial.flush();
  GPSSerial.begin(9600);
  GPSSerial.flush();

  lcd.LCD_init();
  lcd.LCD_clear();
}

void loop(){ 
 
  if(readGPS()) {
    //lcd.LCD_clear();
    lcd.backlight(ON);
   
    char dateBuf[8];
    char timeBuf[8];
    char latBuf[20];
    char lonBuf[20];
  
    gps.date.toCharArray(dateBuf,15);
    lcd.LCD_write_string(0, 0, dateBuf, MENU_NORMAL);
    gps.time.toCharArray(timeBuf,15);
    lcd.LCD_write_string(0, 1, timeBuf, MENU_NORMAL);
    if(gps.isValid){
      gps.latString.toCharArray(latBuf,15);
      lcd.LCD_write_string(0, 2, latBuf, MENU_NORMAL);
      gps.lonString.toCharArray(lonBuf,15);
      lcd.LCD_write_string(0, 3, lonBuf, MENU_NORMAL);
    }
    // alt
    char altBuf[10];  
    gps.altString.toCharArray(altBuf,10);
    lcd.LCD_write_string(0, 4, altBuf, MENU_NORMAL);
    // sog
//    char sogBuf[10];  
//    gps.sogString.toCharArray(sogBuf,10);
//    lcd.LCD_write_string(0, 5, sogBuf, MENU_NORMAL);
  }
}

boolean readGPS() {
    String sub = readLine();
    if(sub.length()) {
//      Serial.println(sub);
//      Serial.println("-------------------------------------------");
      int split = sub.indexOf(',');    
      String field = sub.substring(0,split);
      if(field.equals(GPRMC)) {
        processGPRMC(sub);
      } else if(field.equals(GPGGA)) {
        processGPGGA(sub);
      }
      return true;
  }
  return false;
}

// process GPRMC
void processGPRMC(String sub){
  String time = splitString(sub,',',1);
  String hour = time.substring(0, 2);
  String minute = time.substring(2, 4);
  String second = time.substring(4, 6);
  gps.hour = hour.toInt();
  gps.minute = minute.toInt();
  gps.second = second.toInt();       
  gps.time = hour + ':' + minute + ':' + second;      

// Serial.println(gps.time);
      
  int localHour = ((gps.hour + LOCAL_AREA) % 24); // 8
  String localHourStr = String(100 + localHour); // 108
  localHourStr = localHourStr.substring(localHourStr.indexOf('1')+1, 3); // 08

  gps.localTime = gps.time;
  gps.localTime.replace(gps.localTime.substring(0,2),localHourStr);

// Serial.println(gps.localTime); 

  String date = splitString(sub,',',9);       
  gps.day = date.substring(0,2);
  gps.month = date.substring(2,4);
  gps.year = date.substring(4,6);
  gps.date = gps.day + '/' + gps.month + '/' + gps.year;   
     
// Serial.println(gps.date);

  String valid = splitString(sub,',',2);
  gps.isValid = (valid.equals(VALID) ? true : false);
    
  if(gps.isValid) {  
    // Latitude
    gps.lat = splitString(sub,',',3);
    String degreeOfLat = gps.lat.substring(0,2);
    String minuteOfLat = gps.lat.substring(2,4);
    String secondOfLat = gps.lat.substring(5,8);
    gps.degreeOfLat = degreeOfLat.toInt();
    gps.minuteOfLat = minuteOfLat.toInt();
    gps.secondOfLat = secondOfLat.toInt()*60/1000;

    gps.northingIndicator = splitString(sub,',',4);
    gps.latString = degreeOfLat + CHAR_DEGREE + minuteOfLat + CHAR_MINUTE + gps.secondOfLat + CHAR_SECOND + gps.northingIndicator;
// Serial.println(gps.latString); 
  
    // Longitude
    gps.lon = splitString(sub,',',5);
    String degreeOfLon = gps.lon.substring(0,3);
    String minuteOfLon = gps.lon.substring(3,5);
    String secondOfLon = gps.lon.substring(6,9);
    gps.degreeOfLon = degreeOfLon.toInt();
    gps.minuteOfLon = minuteOfLon.toInt();
    gps.secondOfLon = secondOfLon.toInt()*60/1000;

    gps.eastingIndicator = splitString(sub,',',6);
    gps.lonString = degreeOfLon + CHAR_DEGREE + minuteOfLon + CHAR_MINUTE + gps.secondOfLon + CHAR_SECOND + gps.eastingIndicator;
// Serial.println(gps.lonString); 
  }
}

// process GPGGA
void processGPGGA(String sub){ 
  boolean ggaStatus = splitString(sub,',',6).toInt();
  if(ggaStatus){
    gps.altString = splitString(sub,',',9);
    gps.altString += splitString(sub,',',10);
  //  Serial.println(gps.altString);
  }
}

//0x0D 0x0A CR LF \r \n
String readLine(){
  String line = "";
  while(GPSSerial.available()) {
//      delay(1);
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

// Split String
String splitString(String s, char parser,int index) {
  String rs = '\0';
  int parserIndex = index;
  int parserCnt = 0;
  int fromIndex = 0, toIndex = -1;

  while(index >= parserCnt) {
    fromIndex = toIndex + 1;
    toIndex = s.indexOf(parser, fromIndex);

    if(index == parserCnt){
      if(toIndex == 0 || toIndex == -1){
        return rs;
      }
      return s.substring(fromIndex,toIndex);
    } else{
      parserCnt++;
    }
  }
  return rs;
}


/**
 * 字符串变浮点数
 */
double toDouble(String str){
  double d = 0.0;
  int index = str.indexOf('.');
  
  if(index){
    int i = str.substring(0, (index+1)).toInt();
    String s = str.substring((index+1), str.length());
    int f = s.toInt();
    
    // int
    d += i;
    
    double t = 0.0 + f;
    // float
    for(int j = 0; j < s.length(); j++) {
      t = t/10;      
    }
    
    if(i > 0 ){
      d += t;
    } else {
      d += (-1) * t; 
    }
    
  }
  return d;
}

/*
 *         String s = "";
        switch(a) {
          case LEFT_KEY:
            s = "LEFT";
            break;               
          case CENTER_KEY:
            s = "CENTER";
            break;               
          case DOWN_KEY:
            s = "DOWN";
            break;               
          case RIGHT_KEY:
            s = "RIGHT";
            break;               
          case UP_KEY:
            s = "UP";
            break;               
          default: 
            s = "UNPRESSD";
            break;
        }
 */
// Convert ADC value to key number
int LCD4884::getKey(void) {
      int input = analogRead(KEY_PIN);

	for (int k = 0; k < NUM_KEYS; k++) {
		if (input < ADC_KEY[k]) {
			return k;
		}
	}
	return -1;
}
