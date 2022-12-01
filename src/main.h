#include <my_RFID.h>
#include <my_LCD.h>
#include <my_Keypad.h>
#include<EEPROM.h>
#include <Arduino.h>
#define buzzerPin 3


///////////////////////////////////////////////////////////
//0   - 50      //common variable
//101 - 120     //rfid
//200 - 220     //password
/// latest address of RFID in EEPROM = (1- 3)

/// addr of admin password in flash memory (2001 - 2008)
#define MAXIMUM_CARD_NUMBER                   20
#define SIZE_RFID_CARDIN_BYTE                 4
#define flag_admin_panel_set_ADDRESS          12

#define NUMBER_OF_RFID_TAG_ADDRESS            100
#define ID_CARD_START_ADDRESS                 101
#define SIZE_ADD_NEW_CARD_MEMORY              MAXIMUM_CARD_NUMBER * SIZE_RFID_CARDIN_BYTE
#define ADMIN_PASS_START_ADDRESS              200
#define DOOR_PASS_START_ADDRESS               208
#define MASTER_CARD_START_ADDRESS             51

