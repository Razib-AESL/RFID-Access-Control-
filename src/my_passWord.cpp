#include <my_passWord.h>
/// addr of door password in flash memory (2009 - 2016

uint8_t password[pass_length];
uint8_t admin_password[pass_length];
uint8_t door_password[pass_length];
uint8_t new_password[pass_length];

uint8_t master_card[rfid_length];

bool admin_pass = true;
bool door_pass = true;
bool admin_set = true;
extern bool setting;
// extern bool setting_state = true;

bool rfid_master;
uint8_t read_master;

byte masterCard[rfid_length];
byte storedCard[4];
uint8_t successRead;

uint8_t master_read[4];
uint8_t master_read1[4];

uint8_t doorPass[8];
uint8_t adminPass[8];

void writeUint8IntoEEPROMfromuintArr(uint32_t addr, uint8_t writeVal[], uint16_t arr_ln);
void writeUint16IntoEEPROM(uint32_t addr, uint16_t writeVal);
void set_Admin_Password();
void set_Door_Password();
void set_rfid_Master();
bool check_Door_Password();
void check_admin_pass();

/////////////////////////////////////////////////ADMIN PASSWORD SET/////////////////////////////////////////////////

void set_Admin_Password() // Set the admin Password for Admin
{
  uint8_t j = 0;
  uint8_t k = 0;
  bool admin_cencel = true;

  while (admin_pass)
  {
    uint8_t admin_password1[pass_length];
    Serial.print("Create Admin Pass:");
    lcd.setCursor(0, 0);
    lcd.print("SET ADMIN PASS..");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    while (j < pass_length)
    { // Create new password
      uint8_t customKey = customKeypad.getKey();
      if (customKey)
      {
        if (customKey == 'B')
        {
          if (EEPROM.read(flag_admin_panel_set_ADDRESS) == 1)
          {
            admin_cencel = false;
            admin_pass = false;
            break;
          }
          else
          {
            admin_pass = true;
          }
        }
        else if (customKey != 'C')
        {
          admin_password[j] = customKey;
          Serial.print(customKey);
          lcd.setCursor(j, 1);
          lcd.print("* ");
          j++;
        }
        else if (customKey == 'C')
        {
          j = 0;
          lcd.setCursor(j, 1);
          lcd.print("                ");
        }
      }
    }

    j = 0;
    Serial.println();
    Serial.print("Confirm Admin Pass:"); // Confirm create new password
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CONFIRM PASSWORD");
    while (k < pass_length && admin_cencel)
    {
      uint8_t customKey = customKeypad.getKey();
      if (customKey)
      {
        if (customKey == 'B')
        {
          if (EEPROM.read(flag_admin_panel_set_ADDRESS) == 1)
          {
            admin_cencel = false;
            admin_pass = false;
            break;
          }
          else
          {
            admin_pass = true;
            break;
          }
        }
        else if (customKey != 'C')
        {
          admin_password1[k] = customKey;
          Serial.print(customKey);
          lcd.setCursor(k, 1);
          lcd.print("* ");
          k++;
        }
        else if (customKey == 'C')
        {
          k = 0;
          lcd.setCursor(k, 1);
          lcd.print("                ");
        }
      }
    }
    Serial.println();
    k = 0;

    if (!memcmp(admin_password1, admin_password, pass_length)) // if confirm write password on EEprom
    {
      Serial.println("Password set successful");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PASSWORD SET   ");
      lcd.setCursor(0, 1);
      lcd.print("SUCCESSFULLY...");
      for (k = 0; k < pass_length; k++)
      {
        Serial.print(admin_password[k]);
        Serial.println(",");
        admin_pass = false;
        door_pass = true;
      }
      writeUint8IntoEEPROMfromuintArr(ADMIN_PASS_START_ADDRESS, admin_password, pass_length);
      Serial.println();
      delay(2000);
      uint8_t pas_length = 8;
      while (pas_length != 0)
      {
        admin_password[pas_length--] = 0; // clear array for new data
      }
    }

    else if (admin_pass && admin_cencel)
    {
      Serial.println("Password set unsuccessful");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PASSWORD SET   ");
      lcd.setCursor(0, 1);
      lcd.print("UNSUCCESSFUL...");
      delay(2000);
      lcd.clear();
    }
  }
  admin_pass = true;
}

///////////////////////////////////////////////DOOR PASSWORD SET////////////////////////////////////////////////////////////

void set_Door_Password() // Set the admin Password for Admin
{
  uint8_t j = 0;
  uint8_t k = 0;
  bool door_cencel = true;

  while (door_pass)
  {
    uint8_t door_password1[pass_length];
    Serial.print("Create door Pass:");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SET NEW DOOR PW ");
    while (j < pass_length)
    { // Create new password
      uint8_t customKey = customKeypad.getKey();
      if (customKey)
      {
        if (customKey == 'B')
        {
          if (EEPROM.read(flag_admin_panel_set_ADDRESS) == 1)
          {
            door_cencel = false;
            door_pass = false;
            break;
          }
          else
          {
            door_pass = true;
          }
        }
        else if (customKey != 'C')
        {
          door_password1[j] = customKey;
          Serial.print(customKey);
          lcd.setCursor(j, 1);
          lcd.print("* ");
          j++;
        }
        else if (customKey == 'C')
        {
          j = 0;
          lcd.setCursor(j, 1);
          lcd.print("                ");
        }
      }
    }

    j = 0;
    Serial.println();
    Serial.print("Confirm door Pass:"); // Confirm create new password
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CONFIRM PASSWORD");
    while (k < pass_length && door_cencel)
    {
      uint8_t customKey = customKeypad.getKey();
      if (customKey)
      {
        if (customKey == 'B')
        {
          if (EEPROM.read(flag_admin_panel_set_ADDRESS) == 1)
          {
            door_cencel = false;
            door_pass = false;
            break;
          }
          else
          {
            door_pass = true;
            break;
          }
        }
        else if (customKey != 'C')
        {
          door_password[k] = customKey;
          Serial.print(customKey);
          lcd.setCursor(k, 1);
          lcd.print("* ");
          k++;
        }
        else if (customKey == 'C')
        {
          k = 0;
          lcd.setCursor(k, 1);
          lcd.print("                ");
        }
      }
    }
    Serial.println();
    k = 0;

    if (!memcmp(door_password1, door_password, pass_length)) // if confirm write password on EEprom
    {
      Serial.println("Password set successful");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PASSWORD SET....");
      lcd.setCursor(0, 1);
      lcd.print("SUCCESSFULLY !!!");
      delay(2000);
      door_pass = false;
      Serial.println();
      writeUint8IntoEEPROMfromuintArr(DOOR_PASS_START_ADDRESS, door_password, pass_length);
      delay(2000);
      uint8_t pas_length = 8;
      while (pas_length != 0)
      {
        door_password[pas_length--] = 0; // clear array for new data
      }
    }

    else if (door_pass && door_cencel)
    {
      Serial.println("Password set unsuccessful");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PASSWORD SET....");
      lcd.setCursor(0, 1);
      lcd.print("UNSUCCESSFUL !!!");
      delay(2000);
      lcd.clear();
    }
  }
  door_pass = true;
}

//////////////////////////////////////////////MASTER RFID SET///////////////////////////////////////////////

void set_rfid_Master()
{
  uint8_t master_Card[4];
  while (!rfid_master)
  {
    Serial.println("Scan your Master card");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SCAN NEW MASTER");
    lcd.setCursor(0, 1);
    lcd.print("CARD !!!!!      ");
    do
    {
      successRead = get_rfid_tag(); // sets successRead to 1 when we get read from reader otherwise 0
      uint8_t customKey = customKeypad.getKey();
      if (customKey == 'B')
      {
        if (EEPROM.read(flag_admin_panel_set_ADDRESS) == 1)
        {
          rfid_master = true;
          break;
        }
        else
        {
          rfid_master = false;
        }
      }
      if (successRead)
      {
        for (uint8_t j = 0; j < 4; j++) // Loop 4 times
        {
          masterCard[j] = readCard[j];
        }
      }
    }

    while (!successRead); // Program will not go further while you not get a successful read

    if (!rfid_master)
    {

      Serial.println("Confirm your Master card");
      lcd.setCursor(0, 0);
      lcd.print("CONFIRM YOUR....");
      lcd.setCursor(0, 1);
      lcd.print("MASTER CARD !!! ");

      do
      {
        Serial.println("Confirm your Master card");
        successRead = get_rfid_tag(); // sets successRead to 1 when we get read from reader otherwise 0
        uint8_t customKey = customKeypad.getKey();
        if (customKey == 'B')
        {
          if (EEPROM.read(flag_admin_panel_set_ADDRESS) == 1)
          {
            rfid_master = true;
            break;
          }
          else
          {
            rfid_master = false;
          }
        }
      }

      while (!successRead); // Program will not go further while you not get a successful read

      if (!rfid_master)
      {
        if (!memcmp(readCard, masterCard, 4))
        {

          writeUint8IntoEEPROMfromuintArr(MASTER_CARD_START_ADDRESS, masterCard, rfid_length);
          EEPROM.begin(500);
          EEPROM.write(flag_admin_panel_set_ADDRESS, 1);
          EEPROM.commit();

          Serial.println("Master card same");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("MASTER CARD SET ");
          lcd.setCursor(0, 1);
          lcd.print("SUCCESSFULLY !!!");
          delay(2000);
          rfid_master = true;
        }

        else
        {
          Serial.println("Not Matched");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("SCAN CARD NOT...");
          lcd.setCursor(0, 1);
          lcd.print("MATCHED  !!!!!  ");
          delay(2000);
          lcd.clear();
        }
      }
    }
  }
  rfid_master = false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void writeUint8IntoEEPROMfromuintArr(uint32_t addr, uint8_t writeVal[], uint16_t arr_ln)
{
  EEPROM.begin(500); // need to change
  for (int l = 0; l < arr_ln; l++)
  {
    EEPROM.write(addr, writeVal[l]);
    addr++;
    EEPROM.commit();
  }
  addr = addr - arr_ln;
  Serial.println("EEPROM Write done");
  for (size_t i = addr; i < addr + arr_ln; i++)
  {
    Serial.print(EEPROM.read(i), HEX);
    Serial.print(",");
  }
  Serial.println("");
}

void writeEEprom_1byte(uint8_t address, uint value)
{
  EEPROM.begin(500);
  EEPROM.write(address, value);
  EEPROM.commit();
}

uint32_t ReadEEprom_uint32(uint32_t addr)
{
  uint32_t value = 0;
  for (size_t i = addr; i < addr + 4; i++)
  {
    Serial.print(EEPROM.read(i), HEX);
    Serial.print(",");
  }
  Serial.println("");
  return value;
}

void writeUint16IntoEEPROM(uint32_t addr, uint16_t writeVal)
{
  EEPROM.begin(500);
  uint8_t a = ((writeVal >> 8) & 0xFF);
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
/////////////////////////////////////////////READ MASTER TAG FROM EEPROM////////////////////////////////////////

void read_EEprom_Master_card()
{
  for (uint8_t i = 0; i < 4; i++)
  {
    storedCard[i] = EEPROM.read(i + MASTER_CARD_START_ADDRESS);
    Serial.println(storedCard[i]);
  }
}
//////////////////////////////////////////////READ DOOR PASS FROM EEPROM/////////////////////////////////////////
void read_EEprom_Door_Pass()
{
  for (uint8_t i = 0; i < 8; i++)
  {
    doorPass[i] = EEPROM.read(i + DOOR_PASS_START_ADDRESS);
    Serial.println(doorPass[i]);
  }
}

//////////////////////////////////////////////READ ADMIN PASS FROM EEPROM/////////////////////////////////////////
void read_EEprom_Admin_Pass()
{
  for (uint8_t i = 0; i < 8; i++)
  {
    adminPass[i] = EEPROM.read(i + ADMIN_PASS_START_ADDRESS);
    Serial.println(adminPass[i]);
  }
}

//////////////////////////////////////////////////READ EEPROM///////////////////////////////////
void read_EEprom()
{
  read_EEprom_Master_card();
  read_EEprom_Door_Pass();
  read_EEprom_Admin_Pass();
}

///////////////////////////////////////////////CHECK DOOR PASSWORD////////////////////////////////////////////////
bool check_Door_Password()
{
  uint8_t j = 0;
  bool door_cencel = true;
  uint8_t door_password1[pass_length];
  uint8_t customKey = customKeypad.getKey();

  if (customKey)
  {
    if (customKey == 'A' && j == 0)
    { // If Press A
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ENTER ADMIN PASS");
      admin_set = false;
      j = 0;
    }
    if (!admin_set)
    { // check admin pass for settings
      check_admin_pass();
      admin_set = true;
      return true;
    }
    else if (customKey != 'A' && customKey != 'B' && customKey != 'C' && customKey != 'D' && j == 0)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ENTER DOOR PASS");
      Serial.print(customKey);
      door_password1[j] = customKey;
      lcd.setCursor(j, 1);
      lcd.print("* ");
      j = j + 1;

      while (j < pass_length)
      {
        uint8_t customKey = customKeypad.getKey();
        if (customKey)
        {
          if (customKey == 'B')
          {
            door_cencel = false;
            break;
          }
          else if (customKey != 'C')
          {
            door_password1[j] = customKey;
            Serial.print(customKey);
            lcd.setCursor(j, 1);
            lcd.print("* ");
            j++;
          }
          else if (customKey == 'C')
          {
            j = 0;
            lcd.setCursor(j, 1);
            lcd.print("                ");
          }
        }
      }

      j = 0;
      if (!memcmp(door_password1, doorPass, pass_length)) // if password Matched
      {
        lcd.setCursor(0, 0);
        lcd.print("AUTHORIZED ENTRY");
        lcd.setCursor(0, 1);
        lcd.print("!!!!!!!         ");
        delay(1500);
        uint8_t pas_length = 8;
        while (pas_length != 0)
        {
          door_password1[pas_length--] = 0; // clear array for new data
        }
      }
      else
      { // if password not Matched
        if (door_cencel)
          lcd.setCursor(0, 0);
        lcd.print("ENTRY DENIED !!!! ");
        lcd.setCursor(0, 1);
        lcd.print("                 ");
        delay(1500);
      }
    }
  }
  return false;
}

///////////////////////////////////////////////CHECK ADMIN PASS/////////////////////////////////////////

void check_admin_pass()
{
  uint8_t k = 0;
  uint8_t pas_length = 8;
  bool admin_cencel = true;
  while (k < pass_length)
  {
    uint8_t customKey = customKeypad.getKey();
    if (customKey)
    {
      if (customKey == 'B')
      {
        admin_cencel = false;
        break;
      }
      else if (customKey != 'C')
      {
        admin_password[k] = customKey;
        Serial.print(customKey);
        lcd.setCursor(k, 1);
        lcd.print("* ");
        k++;
      }
      else if (customKey == 'C')
      {
        k = 0;
        lcd.setCursor(k, 1);
        lcd.print("                ");
      }
    }
  }
  Serial.println();
  k = 0;

  if (!memcmp(admin_password, adminPass, pass_length)) // if confirm write password on EEprom
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   WELCOME TO   ");
    lcd.setCursor(0, 1);
    lcd.print("-ADMIN SETTINGS-");
    delay(2000);
    setting = false;
    lcd.clear();

    uint8_t pas_length = 8;
    while (pas_length != 0)
    {
      admin_password[pas_length--] = 0; // clear array for new data
    }
  }

  else
  {
    if (setting && admin_cencel)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("WRONG PASSWORD..");
      lcd.setCursor(0, 1);
      lcd.print("TRY AGAIN !!!!! ");
      delay(2000);
      lcd.clear();
    }
  }
}
