#include <my_RFID.h>
#include <my_LCD.h>
#include <my_Keypad.h>
#include <EEPROM.h>
#include <Arduino.h>

/////////////////////////////////////////////////////////////

#define BUZZER_PIN 3
#define PASSWORD_LENGTH 8
#define RFID_LENGTH 4

////////////////////////////////////////////////////////////


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

/////////////////////////////////////////////////////////////////////////////////////////
extern void lcd_setup();
extern void rfid_sensor_setup();

extern void writeUint8IntoEEPROMfromuintArr(uint32_t addr, uint8_t writeVal[], uint16_t arr_ln);
extern void writeEEprom_1byte(uint8_t address, uint value);
extern void read_EEprom_Admin_Pass();
extern void read_EEprom_Door_Pass();
extern void read_EEprom_Master_card();
extern void read_EEprom();

extern bool check_Door_Password();
extern void check_Key_Press();
extern void set_Door_Password();
//extern void set_Admin_Password();
extern void set_rfid_Master();
extern void set_Admin_password();


extern void set_Admin_password();
extern void compare_set_N_confirm_Pass(int flash_addr);
extern void create_Admin_password();
extern void create_Door_password();