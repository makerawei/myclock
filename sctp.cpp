#include "HardwareSerial.h"
#include "sctp.h"

#define SCTP_START_BYTE_1 0xCC
const uint8_t START_PATTERN[] = {
    SCTP_START_BYTE_1}; // START_PARTTEN[] Should always be [0xCC]

static uint8_t dataIndex;
static uint8_t dataSize;
static uint8_t crcIndex = 0;

static RX_STATE_TYPE rxState = waitForStart;
SCTPPacket p PROGMEM;
static uint8_t crc;
static uint16_t pos = 0;

static bool inited = false;
static SCTPHandler sctpHandler;

void sctpInit(SCTPHandler handler) {
  if(inited) {
    return;
  } else {
    sctpHandler = handler;
  }
}


void sctpParseByte(uint8_t c) {
  // Serial.print("rxState=");
  // Serial.println(rxState);
  switch (rxState) {
  case waitForStart:
    if (c == START_PATTERN[pos]) {
      if (++pos == sizeof(START_PATTERN)) {
        rxState = waitForSeq;
        pos = 0;
        break;
      }
    } else {
      pos = 0;
    }
    break;

  case waitForSeq:
    p.seq = c;
    crc = c;
    rxState = waitForPort;
    break;

  case waitForPort:
    p.port = c;
    crc = (crc + c) % 0xFF;
    rxState = waitForSize1;
    break;

  case waitForSize1:
    p.size = c;
    crc = (crc + c) % 0xFF;
    rxState = waitForSize2;
    break;

  case waitForSize2:
    p.size |= (c << 8);
    crc = (crc + c) % 0xFF;
    Serial.print("size=");
    Serial.println(p.size);
    if(p.size < SCPT_MAX_DATA_SIZE) {
      rxState = waitForData;
    } else {
      rxState = waitForStart;
    }
    break;

  case waitForData:
    p.data[pos++] = c;
    crc = (crc + c) % 0xFF;
    // Serial.print("recv pos=");
    // Serial.println(pos);
    if (pos == p.size) {
      pos = 0;
      rxState = waitForCRC;
    }
    break;

  case waitForCRC:
    if(sctpHandler) {
      sctpHandler(&p);
    }
    rxState = waitForStart;
    break;

  default:
    // TODO: assert
    break;
  }
}

void sctpParseString(uint8_t *buff, unsigned int size) {
  for(int i = 0; i < size; i++) {
    sctpParseByte(*(buff + i));
  }
}
