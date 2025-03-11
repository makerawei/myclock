#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <Arduino.h>
#define MAX_FRAME_COUNT 256

#pragma pack(1)

enum tagCmd {
  CMD_UPDATE_IMAGE = 0,
  CMD_DRAW_PIXEL = 1,
  CMD_CLEAR_SCREEN = 2,
  CMD_ROTATE_SCREEN = 3,
  CMD_SAVE_IMAGE = 4,
  CMD_UPDATE_GIF = 5,
  CMD_SYNC_TIME = 6,
  CMD_UPDATE_BRIGHTNESS = 7,
  CMD_SET_FONT_COLOR = 8,
  CMD_SWAP_BLUE_GREEN = 9,
  CMD_UPDATE_WIFI = 10,
  CMD_UPDATE_NIGHT_RANGE_TIME = 11,
  
  CMD_INVALID = 255
};

typedef struct {
  uint8_t width;
  uint8_t height;
  uint8_t x;
  uint8_t y;
  uint16_t *buff;
} UpdateImageReq;

typedef struct {
  uint8_t x;
  uint8_t y;
  uint16_t color; // color565
} Pixel;

typedef struct {
  uint16_t count;
  Pixel *pixels;
} DrawPixelReq;

typedef struct {
  uint8_t width;
  uint8_t height;
  uint8_t x;
  uint8_t y;
  uint8_t frameCount;
  uint16_t *durations;
  uint16_t *buff;
} UpdateGifReq;

typedef struct {
  unsigned long timestamp;
} SyncTimeReq;

typedef struct {
  uint8_t brightness;
} UpdateBrightnessReq;

typedef struct {
  uint8_t background;
  uint16_t color;
} SetFontColorReq;

typedef struct {
  uint8_t swap;
} SwapBlueGreenReq;

typedef struct {
  char ssid[32];
  char password[32];
} UpdateWifiReq;

typedef struct {
  char rangeTime[16];
} UpdateNightRangeTimeReq;

typedef struct {
  unsigned char cmd;
  union {
    UpdateImageReq updateImageReq;
    DrawPixelReq drawPixelReq;
    UpdateGifReq updateGifReq;
    SyncTimeReq syncTimeReq;
    UpdateBrightnessReq updateBrightnessReq;
    SetFontColorReq setFontColorReq;
    SwapBlueGreenReq swapBlueGreenReq;
    UpdateWifiReq updateWifiReq;
    UpdateNightRangeTimeReq updateNightRangeTimeReq;
  } body;

} CtrlPkg;

#pragma pack()

bool protocolUnpack(uint8_t *buf, CtrlPkg *pstCtrlPkg);

#endif
