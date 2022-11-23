#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 17

extern String myTag;
extern String content;
extern MFRC522 mfrc522;
extern byte readCard[4];
extern void rfid_sensor_setup();
//extern void read_rfid_tag();
extern uint8_t read_rfid_tag();