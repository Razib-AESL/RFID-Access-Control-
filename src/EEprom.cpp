#include <main.h>

uint8_t doorPass[PASSWORD_LENGTH];
uint8_t adminPass[PASSWORD_LENGTH];
byte storedCard[RFID_LENGTH];


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

////////////////////////////////////////////////////WRITE ONE BYTE/////////////////////////////////////////////
void writeEEprom_1byte(uint8_t address, uint value)
{
  EEPROM.begin(500);
  EEPROM.write(address, value);
  EEPROM.commit();
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