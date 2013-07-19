#ifndef GPSLib_h
#define GPSLib_h

#include <Arduino.h>

/**
 * GPS库文件接口定义
 */
class GPSLib {
  public:
    boolean isValid(); // 是否已定位
    
    String utcDate(); // 标准日期 dd/mm/yy
    
    String utcTime(); // 标准时间 hh:mm:ss
    
    double longitude(); // 经度 dddmm.mmmm 
    
    double latitude(); // 纬度 ddmm.mmmm 

    double altitude(); // 海拔
    
  private:    
    boolean validStatus; // T=Valid,F=Invalid 定位状态
    
    // 日期
    String utcDate; // 标准日期 ddmmyy
    int day;    // 日
    int month;  // 月
    int year;   // 年
    
    // 时间
    String utcTime; // 标准时间 hhmmss.sss
    int hour;   // 时    
    int minute; // 分
    int second; // 秒
    int localTimeZone; // 本地时区 00 ～ +- 13
    
    // 纬度
    double latitude; // 纬度 ddmm.mmmm
    int degreeOfLatitude;  // 纬度——度
    int minuteOfLatitude;  // 纬度——分
    int secondOfLatitude;  // 纬度——秒
    char northingIndicator; // 南北航向指示器 N=North,S=South
    
    // 经度
    double longitude; // 经度 dddmm.mmmm
    int degreeOfLongitude;  // 经度——度
    int minuteOfLongitude;  // 经度——分
    int secondOfLongitude;  // 经度——秒
    char eastingIndicator; // 东西航向指示器 E=East,W=West
    
    // 海拔
    double altitude; // 海拔
    char altitudeUnit; //	单位M		M=Meters
};

#endif
