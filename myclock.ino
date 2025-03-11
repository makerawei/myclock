/*
 * Author: makerawei(创客阿玮）  
 * Date: 2025/03/11  
 */
#include "Org_01.h"
#include "FreeSerifBold9pt7b.h"
#include "sctp.h"
#include "protocol.h"
#include "resource.h"
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <SPIFFS.h>
#include <EEPROM.h>
#include <Ticker.h>
#include <ESP32Time.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>


#define PANEL_RES_X 64
#define PANEL_RES_Y 64
#define PANEL_CHAIN 1
#define SWAP_BLUE_GREEN 0
#define MAX_ROTATION 3
#define DEFAULT_IMAGE_FILENAME "/image0.bytes"
#define EEPROM_BASE_ADDRESS 0
#define EEPROM_MAGIC_CHAR 'X'
#define EEPROM_ROTATION_ADDRESS ((EEPROM_BASE_ADDRESS) + 1)
#define EEPROM_FONT_COLOR_ADDRESS ((EEPROM_ROTATION_ADDRESS) + 1)
#define EEPROM_SWAP_BLUE_GREEN_ADDRESS ((EEPROM_FONT_COLOR_ADDRESS) + 10)
#define EEPROM_WIFI_ADDRESS ((EEPROM_SWAP_BLUE_GREEN_ADDRESS) + 10)
#define EEPROM_NIGHT_MODE_RANGE_TIME_ADDRESS ((EEPROM_WIFI_ADDRESS) + 128)
#define DEFAULT_ROTATION 3
#define DEFAULT_BRIGHTNESS 100
#define NIGHT_MODE_BRIGHTNESS 20
#define NIGHT_MODE_FONT_COLOR 0xf800
#define NIGHT_MODE_TEXT_BG_HEIGHT 14
#define DEFAULT_TEXT_BG_HEIGHT 7
#define DEFAULT_FONT_COLOR 0xffff
#define DEFAULT_BACKGROUND_COLOR 0x0000
#define NTP_SYNC_INTERVAL 60  // 1min同步一次ntp服务器，获取准确时间（ESP32内置的RTC时钟会有误差，大概1天快15~20S左右）
#define NIGHT_MODE_RANGE_TIME "22:00-08:00"  // 夜间10点至次日早上8点为夜间模式时间段，夜间模式下自动调暗屏幕亮度，只显示时间到分钟级，不再显示背景图

#define WIFI_SSID_PASSWORD_DELIMITER "\t"
#define WIFI_MAX_CONNECT_TIMES 3

#define debugSerial Serial  // 定义调试打印的串口，不需要串口打印则注释这一行
#ifdef debugSerial
#define debugPrint(...) \
  { debugSerial.print(__VA_ARGS__); }
#define debugPrintln(...) \
  { debugSerial.println(__VA_ARGS__); }
#else
#define debugPrint(...) \
  {}
#define debugPrintln(...) \
  {}
#endif

// Module configuration
HUB75_I2S_CFG mxconfig(
	PANEL_RES_X,
	PANEL_RES_Y,
	PANEL_CHAIN
);

static MatrixPanel_I2S_DMA *dma_display = nullptr;
static CtrlPkg pkg PROGMEM;
static unsigned char lastDrawCmd = CMD_INVALID;
static int rotation = DEFAULT_ROTATION;
static uint16_t fontColor = DEFAULT_FONT_COLOR;
static uint16_t backgroundColor = DEFAULT_BACKGROUND_COLOR;
static uint8_t swapBlueGreen = SWAP_BLUE_GREEN;
static Ticker timerClock;
static Ticker timerNtp;
static ESP32Time rtc(8 * 3600);
static String timeString = "";
extern SCTPPacket p PROGMEM;
static bool isWiFiConnected = false;
static bool isNightMode = false; // 是否为夜间模式
static int nightModeBegin = 1320; // 晚上10点
static int nightModeEnd = 480;    // 早上8点

const char* ntpServer = "ntp.tencent.com";
// 时区偏移量（以秒为单位），例如：UTC+8为8*3600
const long utcOffsetInSeconds = 0;
// 创建WiFiUDP实例，NTP服务器使用UDP协议
static WiFiUDP ntpUDP;
static NTPClient timeClient(ntpUDP, ntpServer, utcOffsetInSeconds);


void printCenter(const char *buf, int y) {
  int16_t x1, y1;
  uint16_t w, h;
  dma_display->clearScreen();
  dma_display->getTextBounds(buf, 0, y, &x1, &y1, &w, &h);
  dma_display->setCursor(32 - (w / 2), y);
  dma_display->print(buf);
}

void onUpdateImageReq(UpdateImageReq *req) {
  debugPrintln("on update image req");
  debugPrint("width=");
  debugPrintln(req->width);
  debugPrint("height=");
  debugPrintln(req->height);
  debugPrint("x=");
  debugPrintln(req->x);
  debugPrint("y=");
  debugPrintln(req->y);
  /*
  char tmp[64] = {0};
  sprintf(tmp, "size=%d,w=%d,h=%d", size, req->width, req->height);
  printCenter(tmp, 32);
  */
  dma_display->clearScreen();
  dma_display->drawRGBBitmap(req->x, req->y, req->buff, req->width, req->height);
}

void onUpdateGifReq(UpdateGifReq *req) {
  debugPrintln("on update gif req");
  debugPrint("width=");
  debugPrintln(req->width);
  debugPrint("height=");
  debugPrintln(req->height);
  debugPrint("x=");
  debugPrintln(req->x);
  debugPrint("y=");
  debugPrintln(req->y);
  debugPrint("frameCount=");
  debugPrintln(req->frameCount);
  debugPrint("duration[0]=");
  debugPrintln(req->durations[0]);
  dma_display->clearScreen();
  dma_display->drawRGBBitmap(req->x, req->y, req->buff, req->width, req->height);
}

void onDrawPixelReq(DrawPixelReq *req) {
  /*
  // send buff for testing: CC 00 00 0B 00 01 02 00 01 06 E0 07 02 06 64 F8 00
  char tmp[64] = {0};
  sprintf(tmp, "count=%d, x=%d, y=%d, c=%d", req->count, req->pixels[0].x, req->pixels[0].y, req->pixels[0].color);
  printCenter(tmp, 32);
  */
  for(int i = 0; i < req->count; i++) {
    Pixel *pixel = req->pixels + i;
    dma_display->drawPixel(pixel->x, pixel->y, pixel->color);
  }
}

void onClearScreen() {
  dma_display->clearScreen();
}

void onRotateScreen() {
  rotation = (rotation + 1) % (MAX_ROTATION + 1);
  EEPROM.writeChar(EEPROM_BASE_ADDRESS, EEPROM_MAGIC_CHAR);
  EEPROM.writeUChar(EEPROM_ROTATION_ADDRESS, (uint8_t)rotation);
  EEPROM.commit();
  debugPrint("rotation=");
  debugPrintln(rotation);
  dma_display->setRotation(rotation); // 0,1,2,3
  if(lastDrawCmd == CMD_UPDATE_IMAGE) {
    onUpdateImageReq(&pkg.body.updateImageReq);
  }
}

int onSaveImage() {
  if(lastDrawCmd != CMD_UPDATE_IMAGE) {
    return -1;
  }
  File file = SPIFFS.open(DEFAULT_IMAGE_FILENAME, FILE_WRITE);
  if(!file) {
    debugPrintln("There was an error opening the file for writing");
    return -2;
  }
  UpdateImageReq *req = &pkg.body.updateImageReq;
  uint8_t tmp[1] = {CMD_UPDATE_IMAGE};
  file.write((uint8_t *)tmp, 1);
  file.write((uint8_t *)req, sizeof(pkg.body.updateImageReq) - sizeof(uint16_t *));
  file.write((uint8_t *)req->buff, req->height * req->width * sizeof(uint16_t));
    
  file.close();
  debugPrintln("save image success");
  return 0;
}

void onSyncTime(SyncTimeReq *req) {
  debugPrint("timestamp=");
  debugPrintln(req->timestamp);
  rtc.setTime(req->timestamp);
}

void onUpdateBrightness(UpdateBrightnessReq *req) {
  debugPrint("brightness=");
  debugPrintln(req->brightness);
  dma_display->setBrightness8(req->brightness);  
}

void onSetFontColor(SetFontColorReq *req) {
  debugPrint("set font color:");
  debugPrintln(req->color);
  if(req->background) {
    debugPrintln("set background color");
    backgroundColor = req->color;
    EEPROM.writeUShort(EEPROM_FONT_COLOR_ADDRESS + 1 + 2, backgroundColor);
    drawFontBackground(0, DEFAULT_TEXT_BG_HEIGHT, backgroundColor);
  } else {
    debugPrintln("set font color");
    EEPROM.writeUShort(EEPROM_FONT_COLOR_ADDRESS + 1, req->color);
    dma_display->setTextColor(req->color);
  }
  EEPROM.writeChar(EEPROM_FONT_COLOR_ADDRESS, EEPROM_MAGIC_CHAR);
  EEPROM.commit();
}

void onSwapBlueGreen(SwapBlueGreenReq *req) {
  debugPrint("swap blue and green:");
  debugPrintln(req->swap);
  EEPROM.writeChar(EEPROM_SWAP_BLUE_GREEN_ADDRESS, EEPROM_MAGIC_CHAR);
  EEPROM.writeUChar(EEPROM_SWAP_BLUE_GREEN_ADDRESS + 1, req->swap);
  EEPROM.commit();
}

void onUpdateWifi(UpdateWifiReq *req) {
  debugPrint("updateWifiSsid=");
  debugPrintln(req->ssid);
  debugPrint("updateWifiPassword=");
  debugPrintln(req->password);
  EEPROM.writeChar(EEPROM_WIFI_ADDRESS, EEPROM_MAGIC_CHAR);
  char tmp[64] = {0};
  snprintf(tmp, sizeof(tmp) - 1, "%s%s%s", req->ssid, WIFI_SSID_PASSWORD_DELIMITER, req->password);
  EEPROM.writeString(EEPROM_WIFI_ADDRESS + 1, tmp);
  EEPROM.commit();
  connectWifi(req->ssid, req->password);
}

void onUpdateNightRangeTime(UpdateNightRangeTimeReq *req) {
  debugPrint("updateNightRangeTime=");
  debugPrintln(req->rangeTime);
  if(parseNightRangeTime(req->rangeTime)) {
    EEPROM.writeChar(EEPROM_NIGHT_MODE_RANGE_TIME_ADDRESS, EEPROM_MAGIC_CHAR);
    EEPROM.writeString(EEPROM_NIGHT_MODE_RANGE_TIME_ADDRESS + 1, req->rangeTime);
    EEPROM.commit();
  }
}

static int myPacketHandler(SCTPPacket *packet) {
  debugPrintln("get sctp data");
  if(!packet) {
    return -1;
  }
  if(packet->port != PORT_CTRL) {
    debugPrintln("Invalid port");
    return -2;
  }
  
  int ret = 0;
  unsigned char lastCmd = pkg.cmd;
  protocolUnpack(packet->data, &pkg);
  debugPrint("pkg.cmd=");
  debugPrintln(pkg.cmd);
  switch(pkg.cmd) {
  case CMD_UPDATE_IMAGE:
    onUpdateImageReq(&pkg.body.updateImageReq);
    lastDrawCmd = lastCmd;
    break;
  case CMD_UPDATE_GIF:
    onUpdateGifReq(&pkg.body.updateGifReq);
    break;
  case CMD_DRAW_PIXEL:
    onDrawPixelReq(&pkg.body.drawPixelReq);
    break;
  case CMD_CLEAR_SCREEN:
    onClearScreen();
    break;
  case CMD_ROTATE_SCREEN:
    onRotateScreen();
    break;
  case CMD_SAVE_IMAGE:
    ret = onSaveImage();
    break;
  case CMD_SYNC_TIME:
    onSyncTime(&pkg.body.syncTimeReq);
    break;
  case CMD_UPDATE_BRIGHTNESS:
    onUpdateBrightness(&pkg.body.updateBrightnessReq);
    break;
  case CMD_SET_FONT_COLOR:
    onSetFontColor(&pkg.body.setFontColorReq);
    break;
  case CMD_SWAP_BLUE_GREEN:
    onSwapBlueGreen(&pkg.body.swapBlueGreenReq);
    break;
  case CMD_UPDATE_WIFI:
    onUpdateWifi(&pkg.body.updateWifiReq);
    break;
  case CMD_UPDATE_NIGHT_RANGE_TIME:
    onUpdateNightRangeTime(&pkg.body.updateNightRangeTimeReq);
    break;
  default:
    break;
  }
  
  return ret;  
}

void drawFontBackground(int8_t y, int8_t height, uint16_t color) {
  dma_display->fillRect(0, y, 64, height, color);
}

void printCenter(const char *buf, int y, int offset) {
  int16_t x1, y1;
  uint16_t w, h;
  dma_display->getTextBounds(buf, 0, y, &x1, &y1, &w, &h);
  dma_display->setCursor((PANEL_RES_X >> 1) - (w >> 1) + offset, y);
  dma_display->print(buf);
}

void printIcon(Icon *icon) {
  int i = 0;
  for(i = 0; i < icon->size; i++) {
    int x = icon->points[i].x + icon->x;
    int y = icon->points[i].y + icon->y;
    dma_display->drawPixel(x, y, icon->color);
  }
}

void updateWifiStatusIcon() {
  wifiIcon.color = isWiFiConnected ? 0x07e0 : 0xf800;
  printIcon(&wifiIcon);
}


void drawBackgroundImage() {
  if(!SPIFFS.begin(true)) {
    debugPrintln("An Error has occurred while mounting SPIFFS");
    return;
  }
  if(SPIFFS.exists(DEFAULT_IMAGE_FILENAME)) {
    debugPrintln("image file exists");
    File file = SPIFFS.open(DEFAULT_IMAGE_FILENAME, FILE_READ);
    if(file) {
      debugPrint("file size:");
      debugPrintln(file.size());
      file.read((uint8_t *)&p.data, sizeof(p.data));
      debugPrint("read cmd from file=");
      debugPrintln((uint8_t)p.data[0]);
      file.close();
      p.port = PORT_CTRL;
      myPacketHandler(&p);
    }
  } else {
    debugPrintln("image file not exists");
    dma_display->drawRGBBitmap(0, 0, MYCLOCK_DEFAULT_IMAGE, 64, 64);
  }
}


void updateTime() {
  String time = rtc.getTime();
  if(time == timeString) {
    return;
  }
  timeString = time;
  
  int tm, h, m = 0;
  sscanf(time.c_str(), "%d:%d", &h, &m);
  tm = h * 60 + m;
  
  bool preIsNightMode = isNightMode;
  if(nightModeBegin <= nightModeEnd) {
    isNightMode = tm >= nightModeBegin && tm < nightModeEnd;
  } else {
    isNightMode = tm >= nightModeBegin || tm < nightModeEnd;
  }
  
  if(!isNightMode) { // 白天模式
    if(preIsNightMode) {
      debugPrintln("change from night mode to day mode");
      dma_display->setFont(&Org_01);
      dma_display->setBrightness8(DEFAULT_BRIGHTNESS);
      dma_display->setTextColor(fontColor);
      drawBackgroundImage();
    }
    drawFontBackground(0, DEFAULT_TEXT_BG_HEIGHT, backgroundColor);
    printCenter(time.c_str(), 5, 0);
    updateWifiStatusIcon();
  } else { // 夜间模式
    if(!preIsNightMode) {
      debugPrintln("change from day mode to night mode");
      dma_display->clearScreen();
      dma_display->setFont(&FreeSerifBold9pt7b);
      dma_display->setTextColor(NIGHT_MODE_FONT_COLOR);
      dma_display->setBrightness8(NIGHT_MODE_BRIGHTNESS);
    }
    drawFontBackground((PANEL_RES_Y >> 1) - 12, NIGHT_MODE_TEXT_BG_HEIGHT, 0x0000);
    // 夜间模式只显示小时和分钟 hh:mm
    char tmp[7] = {0};
    snprintf(tmp, sizeof(tmp) - 1, "%s", time.c_str());
    printCenter(tmp, PANEL_RES_Y >> 1, 0);
  }
}

void syncTimeFromNtp() {
  debugPrintln("syncTimeFromNtp");
  if(timeClient.forceUpdate()) {
    long timestamp = timeClient.getEpochTime();
    rtc.setTime(timestamp);
    debugPrintln("sync time from ntp server success");
    debugPrint("ntp timestamp=");
    debugPrintln(timestamp);
  } else {
    debugPrintln("get time from ntp server failed");
    if(WiFi.status() != WL_CONNECTED) {
      debugPrintln("wifi disconnected");
      isWiFiConnected = false;
    }
  }
}

void onConnectingWifiStatus()
{
  printCenter("Connecting", 8, 0);
  printCenter("WiFi...", 20, 0);
  dma_display->drawBitmap(16, 24, CONNECTING_WIFI, 32, 32, 0x2459);
}


bool parseNightRangeTime(const char *rangeTime) {
  int h1, m1, h2, m2;
  if(sscanf(rangeTime, "%d:%d-%d:%d", &h1, &m1, &h2, &m2) != 4) {
    debugPrint("Invalid night mode range time:");
    debugPrintln(rangeTime);
    return false;
  }
  nightModeBegin = h1 * 60 + m1;
  nightModeEnd = h2 * 60 + m2;
  debugPrint("night mode begin-end munites=");
  debugPrint(nightModeBegin);
  debugPrint(",");
  debugPrintln(nightModeEnd);
  return true;
}

void connectWifi(const char *ssid, const char *password) {
  WiFi.begin(ssid, password);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < WIFI_MAX_CONNECT_TIMES) {
    delay(1000);
    debugPrintln("connecting WiFi...");
  }
  if(WiFi.status() == WL_CONNECTED) {
    debugPrintln("wifi connect success");
    isWiFiConnected = true;
    timeClient.begin();
    if(timeClient.forceUpdate()) {
      rtc.setTime(timeClient.getEpochTime());
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  if(!EEPROM.begin(1000)) {
    debugPrintln("EEPROM init failed");
    return;
  }
  
  rtc.setTime(23, 54, 03, 4, 1, 2017);  // initial time is my daughter's birthday;)
    
  char magic = (char)EEPROM.readChar(EEPROM_BASE_ADDRESS);
  debugPrint("magic=");
  debugPrintln(magic);
  if(magic == EEPROM_MAGIC_CHAR) {
    debugPrintln("read config from EEPROM");
    rotation = (int)EEPROM.readUChar(EEPROM_ROTATION_ADDRESS);
  } else {
    debugPrintln("EEPROM magic char not found");
  }
  magic = (char)EEPROM.readChar(EEPROM_FONT_COLOR_ADDRESS);
  if(magic == EEPROM_MAGIC_CHAR) {
    fontColor = EEPROM.readUShort(EEPROM_FONT_COLOR_ADDRESS + 1);
    backgroundColor = EEPROM.readUShort(EEPROM_FONT_COLOR_ADDRESS + 1 + 2);
    debugPrint("EEPROM font color=");
    debugPrintln(fontColor);
    debugPrint("EEPROM background color=");
    debugPrintln(backgroundColor);
  }
  
  magic = (char)EEPROM.readChar(EEPROM_NIGHT_MODE_RANGE_TIME_ADDRESS);
  if(magic == EEPROM_MAGIC_CHAR) {
    String rangeTime = EEPROM.readString(EEPROM_NIGHT_MODE_RANGE_TIME_ADDRESS + 1);
    parseNightRangeTime(rangeTime.c_str());
  }
  
  magic = (char)EEPROM.readChar(EEPROM_SWAP_BLUE_GREEN_ADDRESS);
  if(magic == EEPROM_MAGIC_CHAR) {
    swapBlueGreen = EEPROM.readUChar(EEPROM_SWAP_BLUE_GREEN_ADDRESS + 1);
  }
  
  if(swapBlueGreen) {
    mxconfig.gpio.b1 = 26;
    mxconfig.gpio.b2 = 12;
    mxconfig.gpio.g1 = 27;
    mxconfig.gpio.g2 = 13;
  }
  mxconfig.gpio.e = 18;
  mxconfig.clkphase = false;
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(DEFAULT_BRIGHTNESS); //0-255
  dma_display->setRotation(rotation); // 0,1,2,3
  dma_display->setFont(&Org_01);
  dma_display->setTextColor(fontColor);
  
  magic = (char)EEPROM.readChar(EEPROM_WIFI_ADDRESS);
  if(magic == EEPROM_MAGIC_CHAR) {
    String wifi = EEPROM.readString(EEPROM_WIFI_ADDRESS + 1);
    const char *ssid = strtok((char *)wifi.c_str(), WIFI_SSID_PASSWORD_DELIMITER);
    if(ssid) {
      const char *password = (char *)strtok(NULL, WIFI_SSID_PASSWORD_DELIMITER);
      if(password) {
        onConnectingWifiStatus();
        debugPrintln("read wifi settings from EEPROM");
        debugPrint("ssid=");
        debugPrintln(ssid);
        debugPrint("password=");
        debugPrintln(password);
        connectWifi(ssid, password);
      }
    }
  }
  
  drawBackgroundImage();
  
  sctpInit(myPacketHandler);

  timerClock.attach(0.5, updateTime);
  if(WiFi.status() == WL_CONNECTED) {
    timerNtp.attach(NTP_SYNC_INTERVAL, syncTimeFromNtp);
  }
  
  Serial.println("system start success");
}


void loop() {
  int n = Serial.available();
  if(n > 0) {
    uint8_t buff[256] = {0};
    if(n > sizeof(buff)) {
      n = sizeof(buff);
    }
    size_t size = Serial.readBytes(buff, n);
    sctpParseString(buff, size);
    /*
    debugPrint("get data size=");
    debugPrintln(size);
    */
  }
}
