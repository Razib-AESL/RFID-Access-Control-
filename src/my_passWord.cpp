#include <main.h>

#define pass_length 8
#define rfid_length 12

char password[pass_length];
char admin_password[pass_length];
char door_password[pass_length];
char new_password[pass_length];

char master_card[rfid_length];

bool admin_pass = true;
bool door_pass = true;
bool rfid_master = true;
bool read_master = true;
byte master_read[4];
byte master_read1[4];

/////////////////////////////////////////////////ADMIN PASSWORD SET/////////////////////////////////////////////////

void set_Admin_Password()                            // Set the admin Password for Admin 
{
  uint8_t j = 0;
  uint8_t k = 0;  
  
  while(admin_pass)
  {
    char admin_password1[pass_length];
    Serial.print("Create Admin Pass:");
    while (j < pass_length) {                 //Create new password
    char customKey = customKeypad.getKey(); 
      if (customKey) {
        admin_password1[j] = customKey;
        Serial.print("*");
        j++;
      }
    }

  j = 0;
  Serial.println();  
  Serial.print("Confirm Admin Pass:");      // Confirm create new password
  while (k < pass_length)    {
     char customKey = customKeypad.getKey(); 
      if (customKey) {
        admin_password[k] = customKey;
        Serial.print("*");
        k++;
      }
    }
    Serial.println();
    k = 0;

    if (!strncmp(admin_password1, admin_password, pass_length))   // if confirm write password on EEprom
    {
        Serial.println("Password set successful");
        for(k = 0; k < pass_length; k ++)
        {
            Serial.print(admin_password[k]);
          EEPROM.write(k, admin_password[k]);
            admin_pass = false;
            door_pass = true;
        }
        Serial.println();
    }

    else 
        Serial.println("Password set unsuccessful");
        //pass = true;
  } 
}

///////////////////////////////////////////////DOOR PASSWORD SET////////////////////////////////////////////////////////////

void set_Door_Password()                            // Set the admin Password for Admin 
{
  u_int8_t j = 0;
  u_int8_t k = 0;  
  
  while(door_pass)
  {
    char door_password1[pass_length];
    Serial.print("Create door Pass:");
    while (j < pass_length) {                 //Create new password
    char customKey = customKeypad.getKey(); 
      if (customKey) {
        door_password1[j] = customKey;
        Serial.print("*");
        j++;
      }
    }

  j = 0;
  Serial.println();  
  Serial.print("Confirm door Pass:");      // Confirm create new password
  while (k < pass_length)    {
     char customKey = customKeypad.getKey(); 
      if (customKey) {
        door_password[k] = customKey;
        Serial.print("*");
        k++;
      }
    }
    Serial.println();
    k = 0;

    if (!strncmp(door_password1, door_password, pass_length))   // if confirm write password on EEprom
    {
        Serial.println("Password set successful");
        for(k = 0; k < pass_length; k ++)
        {
            Serial.print(door_password[k]);
         //   EEPROM.write(k, door_password[k]);
            door_pass = false;
        }
        Serial.println();
    }

    else 
        Serial.println("Password set unsuccessful");
  } 
}

//////////////////////////////////////////////MASTER RFID SET///////////////////////////////////////////////

void set_rfid_Master()
{
    
  Serial.print("Scan Your Master Tag");
  while(rfid_master){
    if(read_master){
    read_rfid_tag();
       for(u_int8_t i = 0; i < 4; i++){
        master_read[i] = readCard[i];
         Serial.println(master_read[i]);
    }
    read_master = false;
    }
  else if (!read_master)
  {
    read_rfid_tag();
       for(u_int8_t i = 0; i < 4; i++){
        master_read1[i] = readCard[i];
         Serial.println(readCard[i]);
     }
    // if (!strncmp(master_read, master_read1, 4)) 
    }


  }
}