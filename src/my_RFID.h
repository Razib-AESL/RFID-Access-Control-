#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5  //21
#define RST_PIN 17 //22

extern String myTag;
extern String content;
extern MFRC522 mfrc522;
extern byte readCard[4];
extern void rfid_sensor_setup();
//extern void read_rfid_tag();
extern uint8_t get_rfid_tag();