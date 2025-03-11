#ifndef __SCTP_H__
#define __SCTP_H__
#include <Arduino.h>

#define SCTP_TX_QUEUE_SIZE 2
#define SCPT_MAX_DATA_SIZE 8600
#define MAX_NUM_OF_PORTS 2

#define IS_LEGAL_PORT(PORT) ((PORT) < (MAX_NUM_OF_PORTS))

enum PortType {
  PORT_CTRL = 0,
  PORT_ECHO = 1,
  PORT_ACK = 2,
};

typedef enum {
  waitForStart = 0,
  waitForSeq,
  waitForPort,
  waitForSize1,
  waitForSize2,
  waitForData,
  waitForCRC
} RX_STATE_TYPE;

typedef struct _SCTPPacket {
  uint8_t seq;
  uint8_t port;
  uint16_t size;
  uint8_t data[SCPT_MAX_DATA_SIZE];
} SCTPPacket;

typedef int (*SCTPHandler)(SCTPPacket *p);

void sctpInit(SCTPHandler handler);
void sctpParseByte(uint8_t c);
void sctpParseString(uint8_t *buff, unsigned int size);

#endif
