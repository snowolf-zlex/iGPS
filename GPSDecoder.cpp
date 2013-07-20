
#include "GPSDecoder.h"

GPSDecoder::GPSDecoder() {
}

GPSDecoder::~GPSDecoder() {    
}


void GPSDecoder::decode(String sub) {
    if(sub.length()) {
        int split = sub.indexOf(',');
        String field = sub.substring(0,split);
        if(field.equals(GPRMC)) {
            processGPRMC(sub);
        } else if(field.equals(GPGGA)) {
            processGPGGA(sub);
        }
    }
}



String GPSDecoder::utcTime() {
    return _utcTime;
}


String GPSDecoder::utcDate() {
    return _utcDate;
}

bool GPSDecoder::isValid() {
    return _validStatus;
}


String GPSDecoder::longitude() {
    return _longitude;
}

String GPSDecoder::latitude() {
    return _latitude;
}

String GPSDecoder::altitude() {
    return _altitude;
}


/**
 * 字符串变浮点数
 */
double GPSDecoder::toDouble(String str){
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

// process GPRMC
void GPSDecoder::processGPRMC(String sub){
    String time = splitString(sub,',',1);
    String hour = time.substring(0, 2);
    String minute = time.substring(2, 4);
    String second = time.substring(4, 6);
    _hour = hour.toInt();
    _minute = minute.toInt();
    _second = second.toInt();
    _utcTime = hour + ':' + minute + ':' + second;
    
//    // Serial.println(time);
//    
//    int localHour = ((hour + LOCAL_AREA) % 24); // 8
//    String localHourStr = String(100 + localHour); // 108
//    localHourStr = localHourStr.substring(localHourStr.indexOf('1')+1, 3); // 08
//    
//    localTime = time;
//    localTime.replace(localTime.substring(0,2),localHourStr);
//    
//    // Serial.println(localTime);
//    
    String date = splitString(sub,',',9);
    String day = date.substring(0,2);
    String month = date.substring(2,4);
    String year = date.substring(4,6);
    _day = day.toInt();
    _month = month.toInt();
    _year = year.toInt();
    _utcDate = day + '/' + month + '/' + year;
//
//    // Serial.println(date);
//    
    String valid = splitString(sub,',',2);
    _validStatus = (valid.equals(VALID) ? true : false);
//
//    if(validStatus) {
        // Latitude
        String lat = splitString(sub,',',3);
        String degreeOfLat = lat.substring(0,2);
        String minuteOfLat = lat.substring(2,4);
        String secondOfLat = lat.substring(5,8);
        _degreeOfLatitude = degreeOfLat.toInt();
        _minuteOfLatitude = minuteOfLat.toInt();
        _secondOfLatitude = secondOfLat.toInt()*60/1000;
        
        _northingIndicator = splitString(sub,',',4).charAt(0);
        _latitude = degreeOfLat + CHAR_DEGREE + minuteOfLat + CHAR_MINUTE + secondOfLat + CHAR_SECOND + _northingIndicator;
//        // Serial.println(latString);
//        
        // Longitude
        String lon = splitString(sub,',',5);
        String degreeOfLon = lon.substring(0,3);
        String minuteOfLon = lon.substring(3,5);
        String secondOfLon = lon.substring(6,9);
        _degreeOfLongitude = degreeOfLon.toInt();
        _minuteOfLongitude = minuteOfLon.toInt();
        _secondOfLongitude = secondOfLon.toInt()*60/1000;
//
        _eastingIndicator = splitString(sub,',',6).charAt(0);
        _longitude = degreeOfLon + CHAR_DEGREE + minuteOfLon + CHAR_MINUTE + secondOfLon + CHAR_SECOND + _eastingIndicator;
//        // Serial.println(lonString);
//    }
}

// process GPGGA
void GPSDecoder::processGPGGA(String sub){
    boolean ggaStatus = splitString(sub,',',6).toInt();
    if(ggaStatus){
        _altitude = splitString(sub,',',9);
        _altitude += splitString(sub,',',10);
        //  Serial.println(altString);
    }
}

// Split String
String GPSDecoder::splitString(String s, char parser,int index) {
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
