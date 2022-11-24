
#define pass_length 4
#define rfid_length 12


///////////////////////////////////////////////////////////
//0-50      //common variable
//51 - 2000 //rfid
//2001-2050 //password
/// latest address of RFID in EEPROM = (1- 3)

/// addr of admin password in flash memory (2001 - 2008)
#define NEXT_RFID_TAG_STORING_START_ADDREESS  1
#define ADMIN_PASS_START_ADDRESS              200
#define DOOR_PASS_START_ADDRESS               208
/// addr of door password in flash memory (2009 - 2016