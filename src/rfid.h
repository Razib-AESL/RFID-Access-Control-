#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>

#define SS_PIN    5
#define RST_PIN   17

//#define START_RFID    0
#define COMPARE_RFID  0
#define NEW_RFID      1 

#define buzzerPin 3
extern bool RFID_handler();