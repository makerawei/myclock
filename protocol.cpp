#include <sys/_stdint.h>
#include "HardwareSerial.h"
#include "protocol.h"

static void parseUpdateImageReq(uint8_t *buf, CtrlPkg *pstCtrlPkg) {
  UpdateImageReq *req = &pstCtrlPkg->body.updateImageReq;
  req->width = buf[1];
  req->height = buf[2];
  req->x = buf[3];
  req->y = buf[4];
  req->buff = (uint16_t *)((char *)buf + 5);
}

static void parseUpdateGifReq(uint8_t *buf, CtrlPkg *pstCtrlPkg) {
  UpdateGifReq *req = &pstCtrlPkg->body.updateGifReq;
  req->width = buf[1];
  req->height = buf[2];
  req->x = buf[3];
  req->y = buf[4];
  req->frameCount = buf[5];
  req->durations = (uint16_t *)((char *)buf + 6);
  req->buff = (uint16_t *)((char *)buf + 6 + req->frameCount * sizeof(uint16_t));
}

static void parseDrawPixelReq(uint8_t *buf, CtrlPkg *pstCtrlPkg) {
  DrawPixelReq *req = &pstCtrlPkg->body.drawPixelReq;
  req->count = *(uint16_t *)(buf + 1);
  req->pixels = (Pixel *)(buf + 3);
}

static void parseSyncTimeReq(uint8_t *buf, CtrlPkg *pstCtrlPkg) {
  SyncTimeReq *req = &pstCtrlPkg->body.syncTimeReq;
  req->timestamp = ((unsigned long)buf[1] << 24) |
                   ((unsigned long)buf[2] << 16) |
                   ((unsigned long)buf[3] << 8)  |
                   ((unsigned long)buf[4]);
}

static void parseUpdateBrightnessReq(uint8_t *buf, CtrlPkg *pstCtrlPkg) {
  UpdateBrightnessReq *req = &pstCtrlPkg->body.updateBrightnessReq;
  req->brightness = *(uint8_t *)(buf + 1);
}

static void parseSetFontColorReq(uint8_t *buf, CtrlPkg *pstCtrlPkg) {
  SetFontColorReq *req = &pstCtrlPkg->body.setFontColorReq;
  req->background = *(uint8_t *)(buf + 1);
  req->color = (uint16_t)(buf[2] << 8) | (uint16_t)buf[3];
}

static void parseSwapBlueGreenReq(uint8_t *buf, CtrlPkg *pstCtrlPkg) {
  SwapBlueGreenReq *req = &pstCtrlPkg->body.swapBlueGreenReq;
  req->swap = *(uint8_t *)(buf + 1);
}

static void parseUpdateWifiReq(uint8_t *buf, CtrlPkg *pstCtrlPkg) {
  UpdateWifiReq *req = &pstCtrlPkg->body.updateWifiReq;
  char *p = (char *)buf;
  strncpy(req->ssid, p + 1, sizeof(req->ssid));
  req->ssid[sizeof(req->ssid) - 1] = '\0';
  strncpy(req->password, p + 1 + strlen(req->ssid) + 1, sizeof(req->password));
  req->password[sizeof(req->password) - 1] = '\0';
}

static void parseUpdateNightRangeTimeReq(uint8_t *buf, CtrlPkg *pstCtrlPkg) {
  UpdateNightRangeTimeReq *req = &pstCtrlPkg->body.updateNightRangeTimeReq;
  strncpy(req->rangeTime, (char *)(buf + 1), sizeof(req->rangeTime));
}

bool protocolUnpack(uint8_t *buf, CtrlPkg *pstCtrlPkg) {
  if (buf == NULL || pstCtrlPkg == NULL)
    return false;

  pstCtrlPkg->cmd = *buf;
  uint8_t *p = buf + 1;
  switch (pstCtrlPkg->cmd) {
  case CMD_UPDATE_IMAGE:
    parseUpdateImageReq(buf, pstCtrlPkg);
    break;
  case CMD_UPDATE_GIF:
    parseUpdateGifReq(buf, pstCtrlPkg);
    break;
  case CMD_DRAW_PIXEL:
    parseDrawPixelReq(buf, pstCtrlPkg);
    break;
  case CMD_SYNC_TIME:
    parseSyncTimeReq(buf, pstCtrlPkg);
    break;
  case CMD_UPDATE_BRIGHTNESS:
    parseUpdateBrightnessReq(buf, pstCtrlPkg);
    break;
  case CMD_SET_FONT_COLOR:
    parseSetFontColorReq(buf, pstCtrlPkg);
    break;
  case CMD_SWAP_BLUE_GREEN:
    parseSwapBlueGreenReq(buf, pstCtrlPkg);
    break;
  case CMD_UPDATE_WIFI:
    parseUpdateWifiReq(buf, pstCtrlPkg);
    break;
  case CMD_UPDATE_NIGHT_RANGE_TIME:
    parseUpdateNightRangeTimeReq(buf, pstCtrlPkg);
    break;
  case CMD_CLEAR_SCREEN:
  case CMD_ROTATE_SCREEN:
  case CMD_SAVE_IMAGE:
    break;
  default:
    break;
  }

  return true;
}
