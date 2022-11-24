#include <main.h>
#include <my_passWord.h>
#include <my_LCD.h>


uint8_t my_password[pass_length];
uint8_t admin_password[pass_length];
uint8_t door_password[pass_length];
uint8_t new_password[pass_length];

uint8_t master_card[rfid_length];

bool admin_pass = true;
bool door_pass = true;
bool rfid_master = true;
bool read_master = true;

uint8_t master_read[4];
uint8_t master_read1[4];
uint8_t tagsCount = 0;

void writeUint8IntoEEPROMfromuintArr(uint32_t addr,uint8_t writeVal[], uint16_t arr_ln);
void writeUint16IntoEEPROM(uint32_t addr,uint16_t writeVal);

int Cursor;

/////////////////////////////////////////////////ADMIN PASSWORD SET/////////////////////////////////////////////////

void set_Admin_Password()                            // Set the admin Password for Admin 
{
  uint8_t j = 0;
  uint8_t k = 0;  
  
  while(admin_pass)
  {
    uint8_t admin_password1[pass_length];
    lcd.setCursor(0,0);
    lcd.print("Create Admin Pass:");
    Serial.print("Create Admin Pass:");
    Cursor = 0;
    while (j < pass_length) {                 //Create new password
    uint8_t customKey = keypad.getKey(); 
      if (customKey) {
        admin_password1[j] = customKey;
        Serial.print("*");
        lcd.setCursor(Cursor,1);
        lcd.print("*");
        Cursor++;
        j++;
      }
    }
  delay(300);
  lcd.clear();
  j = 0;
  Serial.println();  
  Serial.print("Confirm Admin Pass:");
  lcd.setCursor(0,0);
  lcd.print("Confirm Admin Pass:");      // Confirm create new password
  Cursor = 0;
  while (k < pass_length)    {
     uint8_t customKey = keypad.getKey(); 
      if (customKey) {
        admin_password[k] = customKey;
        Serial.print("*");
        lcd.setCursor(Cursor,1);
        lcd.print("*");
        Cursor++;
        k++;
      }
    }
    delay(300);
    lcd.clear();
    Serial.println();
    k = 0;

    if (!memcmp(admin_password1, admin_password, pass_length))   // if confirm write password on EEprom
    {
        Serial.println("Password set successful");
        lcd.setCursor(0,0);
        lcd.print("PW Set successful");
        lcd.setCursor(0,1);
        lcd.print("");
        for(k = 0; k < pass_length; k ++)
        {
            Serial.print(admin_password[k]);
            Serial.println(",");
       //     EEPROM.write(k, admin_password[k]);
            admin_pass = false;
            door_pass = true;
        }
        //writeUint16IntoEEPROM(0,100);
        writeUint8IntoEEPROMfromuintArr(ADMIN_PASS_START_ADDRESS,admin_password,pass_length);
        Serial.println();
    }

    else 
        Serial.println("Password set unsuccessful");
        lcd.setCursor(0,0);
        lcd.print("PW Set unsuccessful");
        lcd.setCursor(0,1);
        lcd.print("");
        //pass = true;
  } 
}

///////////////////////////////////////////////DOOR PASSWORD SET////////////////////////////////////////////////////////////

void set_Door_Password()                            // Set the admin Password for Admin 
{
  uint8_t j = 0;
  uint8_t k = 0;  
  
  while(door_pass)
  {
    uint8_t door_password1[pass_length];
    Serial.print("Create door Pass:");
    lcd.setCursor(0,0);
    lcd.print("Create door Pass:"); 
    Cursor = 0;
    while (j < pass_length) {                 //Create new password
    uint8_t customKey = keypad.getKey(); 
      if (customKey) {
        door_password1[j] = customKey;
        Serial.print("*");
        lcd.setCursor(Cursor,1);
        lcd.print("*");
        Cursor++;
        j++;
      }
    }

  delay(300);
  lcd.clear();
  j = 0;
  Serial.println();  
  Serial.print("Confirm door Pass:");      // Confirm create new password
  lcd.setCursor(0,0);
  lcd.print("Confirm door Pass:"); 
  Cursor = 0;
  while (k < pass_length)    {
     uint8_t customKey = keypad.getKey(); 
      if (customKey) {
        door_password[k] = customKey;
        Serial.print("*");
        lcd.setCursor(Cursor,1);
        lcd.print("*");
        Cursor++;
        k++;
      }
    }

    delay(300);
    lcd.clear();
    Serial.println();
    k = 0;

    if (!memcmp(door_password1, door_password, pass_length))   // if confirm write password on EEprom
    {
        Serial.println("Password set successful");
        lcd.setCursor(0,0);
        lcd.print("PW Set successful");
        lcd.setCursor(0,1);
        lcd.print("");
        for(k = 0; k < pass_length; k ++)
        {
            Serial.print(door_password[k]);
         //   EEPROM.write(k, door_password[k]);
            door_pass = false;
        }
        Serial.println();
        writeUint8IntoEEPROMfromuintArr(DOOR_PASS_START_ADDRESS,door_password,pass_length);
        EEPROM.begin(500);
        EEPROM.write(flag_admin_panel_set_ADDRESS,1);
        EEPROM.commit();
        Serial.println(EEPROM.read(flag_admin_panel_set_ADDRESS));

    }

    else 
        Serial.println("Password set unsuccessful");
        lcd.setCursor(0,0);
        lcd.print("PW Set successful");
        lcd.setCursor(0,1);
        lcd.print("");
  } 
}

//////////////////////////////////////////////MASTER RFID SET///////////////////////////////////////////////
/*
void set_rfid_Master()
{
    
  Serial.print("Scan Your Master Tag");
  while (!rfid_master) {
    rfid_master = read_rfid_tag();
    if ( rfid_master == true) {
      master_read[tagsCount] = read_rfid_tag(); // Sets the master tag into position 0 in the array     
      tagsCount++;
    }
    Serial.println(master_read[0]);
  }
  rfid_master = false;
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void writeUint8IntoEEPROMfromuintArr(uint32_t addr,uint8_t writeVal[], uint16_t arr_ln)
{
  EEPROM.begin(500); //need to change
  for(int l=0; l<arr_ln; l++){
    EEPROM.write(addr, writeVal[l]);
    addr++;
    EEPROM.commit();
  }
  addr = addr - arr_ln;
  Serial.println("EEPROM Write done");
  for (size_t i = addr; i < addr+arr_ln; i++)
  {
  Serial.print(EEPROM.read(i),HEX);
  Serial.print(",");
  }
  Serial.println("");
}
uint32_t ReadEEprom_uint32(uint32_t addr){
  uint32_t value = 0;
    for (size_t i = addr; i <addr+4 ; i++)
    {
        Serial.print(EEPROM.read(i),HEX);
        Serial.print(",");
    }
    Serial.println("");
    return value;
}


void writeUint16IntoEEPROM(uint32_t addr,uint16_t writeVal)
{
  EEPROM.begin(500);
    uint8_t a = ((writeVal>>8) & 0xFF);
    Serial.println(a);
    EEPROM.write(addr, (a));
    addr++;
    EEPROM.commit();
    a = 0;
    a = (writeVal & 0xFF);
    Serial.println(a);
    EEPROM.write(addr, a);
    addr++;
    EEPROM.commit();
  Serial.println("EEPROM Write done");
  for (size_t i = 0; i < addr; i++)
  {
  Serial.print(EEPROM.read(i));
  Serial.print(",");
  }
}