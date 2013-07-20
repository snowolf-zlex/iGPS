#ifndef GPSDecoder_h
#define GPSDecoder_h

#include <Arduino.h>

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


//boolean LIGHT_STATUS = 0;

/**
 * GPS编码解析器
 */
class GPSDecoder {
  public:
    GPSDecoder(); // 构造
    
    ~GPSDecoder(); // 析构
    
    void decode(String str); // 解析
    
    bool isValid(); // 是否已定位
    
    String utcDate(); // 标准日期 dd/mm/yy
    
    String utcTime(); // 标准时间 hh:mm:ss
    
    String longitude(); // 经度 dddmm.mmmm
    
    String latitude(); // 纬度 ddmm.mmmm

    String altitude(); // 海拔
    
  private:      
    bool _validStatus; // T=Valid,F=Invalid 定位状态
    
    // 日期
    String _utcDate; // 标准日期 dd/mm/yy
    int _day;    // 日
    int _month;  // 月
    int _year;   // 年
    
    // 时间
    String _utcTime; // 标准时间 hh:mm:ss
    int _hour;   // 时
    int _minute; // 分
    int _second; // 秒
    int localTimeZone; // 本地时区 00 ～ +- 13
    
    // 纬度
    String _latitude; // 纬度 ddmm.mmmm
    int _degreeOfLatitude;  // 纬度——度
    int _minuteOfLatitude;  // 纬度——分
    int _secondOfLatitude;  // 纬度——秒
    char _northingIndicator; // 南北航向指示器 N=North,S=South
    
    // 经度
    String _longitude; // 经度 dddmm.mmmm
    int _degreeOfLongitude;  // 经度——度
    int _minuteOfLongitude;  // 经度——分
    int _secondOfLongitude;  // 经度——秒
    char _eastingIndicator; // 东西航向指示器 E=East,W=West
    
    // 海拔
    String _altitude; // 海拔
    char altitudeUnit; //  单位M		M=Meters
    
    String readLine(); // 读串口 行数据读取
    
    double toDouble(String str); // 字符串变浮点数
    
    void processGPRMC(String sub);
    
    void processGPGGA(String sub);
    
    String splitString(String s, char parser,int index);
    
};

#endif
