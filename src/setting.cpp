#include <main.h>

#define ADD_NEW_CARD 0
#define DELETE_NEW_CARD 1
#define CHANGE_MASTER_CARD 2
#define CHANGE_DOOR_PASS 3
#define CHANGE_ADMIN_PASS 4


extern bool check_Door_Password();
extern void check_Key_Press();
extern void set_Door_Password();
extern void set_Admin_Password();
extern void set_rfid_Master();
extern void read_EEprom();

void add_New_card();
void delete_Exist_Card();
void change_Door_pass();
void change_Admin_pass();
void change_Master_card();
bool check_New_Card_Validity(uint8_t *tagArr, uint8_t *scanArr);
bool scan_EEprom_for_Add_N_Delete_card();

bool setting = true;
uint8_t successRead1;
int state = 0;
bool setting_state = true;

extern uint8_t doorPass[8];
extern uint8_t adminPass[8];
extern byte storedCard[4];

byte new_Stored_Card[4];
uint8_t new_Card_Read;
uint8_t new_card_number = 0;

bool flag_delete_tag = false;
uint8_t read_addNewCard_arr[SIZE_ADD_NEW_CARD_MEMORY + SIZE_RFID_CARDIN_BYTE];

extern void writeUint8IntoEEPROMfromuintArr(uint32_t addr, uint8_t writeVal[], uint16_t arr_ln);
extern void writeEEprom_1byte(uint8_t address, uint value);

void my_Settings()
{

  if (!setting)
  {
    switch (state)
    {
    case ADD_NEW_CARD:
      lcd.setCursor(0, 0);
      lcd.print("ADD NEW CARD    ");
      lcd.setCursor(0, 1);
      lcd.print("D:OK #:NEXT B:EX");
      check_Key_Press();
      break;

    case DELETE_NEW_CARD:
      lcd.setCursor(0, 0);
      lcd.print("DELETE CARD     ");
      lcd.setCursor(0, 1);
      lcd.print("D:OK #:NEXT B:EX");
      check_Key_Press();
      break;

    case CHANGE_MASTER_CARD:
      lcd.setCursor(0, 0);
      lcd.print("CHNG MASTER CARD");
      lcd.setCursor(0, 1);
      lcd.print("D:OK #:NEXT B:EX");
      check_Key_Press();
      break;
    case CHANGE_DOOR_PASS:
      lcd.setCursor(0, 0);
      lcd.print("CHANGE DOOR PW  ");
      lcd.setCursor(0, 1);
      lcd.print("D:OK #:NEXT B:EX");
      check_Key_Press();
      break;
    case CHANGE_ADMIN_PASS:
      lcd.setCursor(0, 0);
      lcd.print("CHANGE ADMIN PW ");
      lcd.setCursor(0, 1);
      lcd.print("D:OK #:NEXT B:EX");
      check_Key_Press();
      break;
    }
    delay(50);
  }

  else
  {
    if (setting)
    {
      lcd.setCursor(0, 0);
      lcd.print("SCAN YOUR CARD..");
      lcd.setCursor(0, 1);
      lcd.print("ENTER PASSWORD..");
      successRead1 = get_rfid_tag(); // sets successRead to 1 when we get read from reader otherwise 0

      check_Door_Password();

      // if successread == 1 then enter the loop
      if (successRead1 == 1) // the program will not go further while you are not getting a successful read
      {
        scan_EEprom_for_Add_N_Delete_card();
        if (check_New_Card_Validity(readCard, read_addNewCard_arr))
        {
          lcd.setCursor(0, 0);
          lcd.print("AUTHORIZED ENTRY ");
          lcd.setCursor(0, 1);
          lcd.print("                 ");
          digitalWrite(buzzerPin, HIGH);
          delay(100);
          digitalWrite(buzzerPin, LOW);
          delay(100);
          digitalWrite(buzzerPin, HIGH);
          delay(100);
          digitalWrite(buzzerPin, LOW);          
          delay(1500);
          
        }
        else
        {
          lcd.setCursor(0, 0);
          lcd.print("ENTRY DENIED !!! ");
          lcd.setCursor(0, 1);
          lcd.print("                 ");
          digitalWrite(buzzerPin, HIGH);
          delay(800);
          digitalWrite(buzzerPin, LOW);
          delay(100);
        }
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////-------------------CHECK KEY PRESS----------------------////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void check_Key_Press()
{

  char customKey = customKeypad.getKey();
  if (customKey == '#')
  {
    state = state + 1;
    if (state > 4)
      state = 0;
  }

  else if (customKey == 'B')
  {
    state = 0;
    setting = true;
  }

  else if (customKey == 'D')
  {
    lcd.clear();
    setting_state = false;
  }

  if (!setting_state)
  {
    if (state == ADD_NEW_CARD)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SCAN YOUR NEW...");
      lcd.setCursor(0, 1);
      lcd.print("RFID CARD !!!   ");
      add_New_card();
    }
    else if (state == DELETE_NEW_CARD)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SCAN YOUR CARD..");
      lcd.setCursor(0, 1);
      lcd.print("FOR DELETE  !!! ");
      delete_Exist_Card();
    }

    else if (state == CHANGE_MASTER_CARD)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SCAN YOUR OLD...");
      lcd.setCursor(0, 1);
      lcd.print("MASTER CARD !!! ");
      change_Master_card();
    }

    else if (state == CHANGE_DOOR_PASS)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PUT OLD PASSWORD");
      change_Door_pass();
    }

    else if (state == CHANGE_ADMIN_PASS)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PUT OLD PASSWORD");
      change_Admin_pass();
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////SCAN EEprom FOR ADD AND DELETE CARD/////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

bool scan_EEprom_for_Add_N_Delete_card()
{
  for (size_t i = 0; i < SIZE_ADD_NEW_CARD_MEMORY; i++)
  {
    read_addNewCard_arr[i] = EEPROM.read(ID_CARD_START_ADDRESS + i);
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////CHECK NEW CARD VALIDITY/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

bool check_New_Card_Validity(uint8_t *tagArr, uint8_t *scanArr)
{
  EEPROM.begin(500);
  int flag_get_ID = 0;
  int i = 0;
  bool flag_rewriteFlash = false;
  while (i < SIZE_ADD_NEW_CARD_MEMORY)
  {
    flag_get_ID = 0;
    for (size_t j = 0; j < SIZE_RFID_CARDIN_BYTE; j++)
    {
      if ((tagArr[j] == scanArr[i + j]) || flag_rewriteFlash)
      {
        Serial.print(tagArr[j]);
        flag_get_ID++;
        // for delete
        if (flag_delete_tag)
        {
          flag_rewriteFlash = true;
          Serial.println("flag_delete_tag");
          EEPROM.write((ID_CARD_START_ADDRESS + i + j), scanArr[i + j + SIZE_RFID_CARDIN_BYTE]);
          EEPROM.commit();
        }
      }
    }
    i = i + 4;
    // if matched
    if ((flag_get_ID == 4) && !flag_delete_tag)
    {
      return true;
    }
  }
  flag_delete_tag = false;
  if (flag_rewriteFlash)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////---------------------ADD NEW CARD-----------------------////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void add_New_card()
{
  uint8_t rfid_length = 4;
  bool new_Card = true;
  bool new_Card_cencel = true;

  /* READ NEW RFID TAG FIRST TIME HERE*/
  do
  {
    successRead1 = get_rfid_tag(); // sets successRead to 1 when we get read from reader otherwise 0
    if (successRead1)
    {
      for (uint8_t j = 0; j < 4; j++) // Loop 4 times
      {
        new_Stored_Card[j] = readCard[j];
      }
      uint8_t pas_length = 4;
      while (pas_length != 0)
      {
        readCard[pas_length--] = 0; // clear array for new data
      }
    }
    uint8_t customKey = customKeypad.getKey();
    if (customKey == 'B')
    {
      new_Card_cencel = false;
      setting_state = true;
      break;
    }
  }

  while (!successRead1); // Program will not go further while you not get a successful read

  /* READ NEW RFID TAG SECOND TIME HERE*/
  if (new_Card_cencel)
  {
    Serial.println("Confirm your New card");
    lcd.setCursor(0, 0);
    lcd.print("CONFIRM YOUR NEW");
    lcd.setCursor(0, 1);
    lcd.print("SCAN CARD !!    ");
    do
    {
      successRead1 = get_rfid_tag(); // sets successRead to 1 when we get read from reader otherwise 0
      uint8_t customKey = customKeypad.getKey();
      if (customKey == 'B')
      {
        new_Card_cencel = false;
        setting_state = true;
        break;
      }
    }

    while (!successRead1); // Program will not go further while you not get a successful read

    /* CONFIRM NEW CARD AND WRITE CARD TO THE EEPROM*/
    if (!memcmp(readCard, new_Stored_Card, 4))
    {
      lcd.setCursor(0, 0);
      lcd.print("YOUR SCAN CARD !!!");
      lcd.setCursor(0, 1);
      lcd.print("MATCHED.....   ");
      delay(1000);
      new_card_number = EEPROM.read(NUMBER_OF_RFID_TAG_ADDRESS);
      if(new_card_number == 255){
        new_card_number=0;
      }
      scan_EEprom_for_Add_N_Delete_card();
      if (!check_New_Card_Validity(readCard, read_addNewCard_arr))
      {
        Serial.println("not match");
        if (new_card_number <= MAXIMUM_CARD_NUMBER)
        {
          writeUint8IntoEEPROMfromuintArr(ID_CARD_START_ADDRESS + (new_card_number * SIZE_RFID_CARDIN_BYTE), new_Stored_Card, SIZE_RFID_CARDIN_BYTE);
          writeEEprom_1byte(NUMBER_OF_RFID_TAG_ADDRESS, new_card_number + 1);
          Serial.println("NEW CARD ADDED.. SUCCESSFULLY");
          lcd.setCursor(0, 0);
          lcd.print("NEW CARD ADDED..");
          lcd.setCursor(0, 1);
          lcd.print("SUCCESSFULLY !! ");
          delay(2000);

          new_Card = false;
          setting_state = true;
          uint8_t pas_length = 4;
          while (pas_length != 0)
          {
            new_Stored_Card[pas_length--] = 0; // clear array for new data
          }
        }

        else if (new_card_number > MAXIMUM_CARD_NUMBER)
        {
          Serial.println("ALREADY EXIST !! OR MEMORY FULL..!! ");
          lcd.setCursor(0, 0);
          lcd.print("YOUR MEMORY FULL");
          lcd.setCursor(0, 1);
          lcd.print("PLZ DELETE CARD ");
          delay(2000);
        }
      }
      else
      {
        Serial.println("ALREADY EXIST  ");
        lcd.setCursor(0, 0);
        lcd.print("ALREADY EXIST !!");
        lcd.setCursor(0, 1);
        lcd.print("                ");
        setting_state = true;
        delay(2000);
      }
      state = ADD_NEW_CARD;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////----------------------DELETE CARD-----------------------////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void delete_Exist_Card()
{

  uint8_t rfid_length = 4;
  bool delete_Card = true;
  bool delete_Card_cencel = true;

  /* READ NEW RFID TAG FIRST TIME HERE*/
  do
  {
    successRead1 = get_rfid_tag(); // sets successRead to 1 when we get read from reader otherwise 0
    if (successRead1)
    {
      for (uint8_t j = 0; j < 4; j++) // Loop 4 times
      {
        new_Stored_Card[j] = readCard[j];
      }
      uint8_t pas_length = 4;
      while (pas_length != 0)
      {
        readCard[pas_length--] = 0; // clear array for new data
      }
    }
    uint8_t customKey = customKeypad.getKey();
    if (customKey == 'B')
    {
      delete_Card_cencel = false;
      setting_state = true;
      break;
    }
  }

  while (!successRead1); // Program will not go further while you not get a successful read

  /* READ NEW RFID TAG SECOND TIME HERE*/
  if (delete_Card_cencel)
  {
    Serial.println("Confirm your New card");
    lcd.setCursor(0, 0);
    lcd.print("SCAN CONFIRM FOR");
    lcd.setCursor(0, 1);
    lcd.print("DELETE CARD !!! ");
    do
    {
      successRead1 = get_rfid_tag(); // sets successRead to 1 when we get read from reader otherwise 0
      uint8_t customKey = customKeypad.getKey();
      if (customKey == 'B')
      {
        delete_Card_cencel = false;
        setting_state = true;
        break;
      }
    }

    while (!successRead1); // Program will not go further while you not get a successful read

    /* CONFIRM NEW CARD AND WRITE CARD TO THE EEPROM*/
    if (!memcmp(readCard, new_Stored_Card, 4))
    {
      lcd.setCursor(0, 0);
      lcd.print("YOUR SCAN CARD !!!");
      lcd.setCursor(0, 1);
      lcd.print("MATCHED.....   ");
      delay(1000);
      flag_delete_tag = true;
      new_card_number = EEPROM.read(NUMBER_OF_RFID_TAG_ADDRESS);
      scan_EEprom_for_Add_N_Delete_card();
      Serial.println("scan complete");

      if (check_New_Card_Validity(readCard, read_addNewCard_arr))
      {
        Serial.println("not match");
        if (new_card_number <= MAXIMUM_CARD_NUMBER)
        {
          // writeUint8IntoEEPROMfromuintArr(ID_CARD_START_ADDRESS + (new_card_number * SIZE_RFID_CARDIN_BYTE), new_Stored_Card, SIZE_RFID_CARDIN_BYTE);
          writeEEprom_1byte(NUMBER_OF_RFID_TAG_ADDRESS, new_card_number - 1);
          Serial.println("CARD DELETE.. SUCCESSFULLY");
          lcd.setCursor(0, 0);
          lcd.print("DELETE YOUR CARD");
          lcd.setCursor(0, 1);
          lcd.print("SUCCESSFULLY !! ");
          delay(2000);

          delete_Card = false;
          setting_state = true;
          uint8_t pas_length = 4;
          while (pas_length != 0)
          {
            new_Stored_Card[pas_length--] = 0; // clear array for new data
          }
        }
      }
      else
      {
        Serial.println("YOUR CARD NOT EXIST  ");
        lcd.setCursor(0, 0);
        lcd.print("YOUR CARD NOT...");
        lcd.setCursor(0, 1);
        lcd.print("EXIST  !!!!!    ");
        delay(2000);
      }
      setting_state = true;
      state = DELETE_NEW_CARD;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////
////////////////-------------------CHECK MASTER CARD----------------------///////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void change_Master_card()
{
  uint8_t rfid_length = 4;
  byte masterCard[rfid_length];
  bool master = true;
  bool master_cencel = true;

  do
  {
    successRead1 = get_rfid_tag(); // sets successRead to 1 when we get read from reader otherwise 0
    uint8_t customKey = customKeypad.getKey();
    if (customKey == 'B')
    {
      setting_state = true;
      master_cencel = false;
      break;
    }
    if (successRead1)
    {
      for (uint8_t j = 0; j < 4; j++) // Loop 4 times
      {
        masterCard[j] = readCard[j];
      }
    }
  }

  while (!successRead1); // Program will not go further while you not get a successful read

  if (!memcmp(storedCard, masterCard, 4))
  {
    lcd.setCursor(0, 0);
    lcd.print("MASTER CARD !!!");
    lcd.setCursor(0, 1);
    lcd.print("MATCHED.....   ");
    delay(2000);
    master = false;
    uint8_t pas_length = 4;
    while (pas_length != 0)
    {
      storedCard[pas_length--] = 0; // clear array for new data
    }
  }

  if (master && master_cencel)
  {
    lcd.setCursor(0, 0);
    lcd.print("MASTER CARD NOT ");
    lcd.setCursor(0, 1);
    lcd.print("MATCHED.....    ");
    delay(2000);
    setting_state = true;
  }
  else if (!master)
  {
    set_rfid_Master();
    setting_state = true;
    read_EEprom();
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////-------------------CHANGE DOOR PASS----------------------///////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void change_Door_pass()
{
  bool door = true;
  bool door_cencel = true;
  uint8_t j = 0;
  uint8_t pass_length = 8;
  uint8_t door_password1[pass_length];
  while (j < pass_length)
  {
    uint8_t customKey = customKeypad.getKey();
    if (customKey)
    {
      if (customKey == 'B')
      {
        door_cencel = false;
        setting_state = true;
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
    lcd.print("PASSWORD MATCHED ");
    lcd.setCursor(0, 1);
    lcd.print("!!!!!!!!!        ");
    delay(2000);
    door = false;
    uint8_t pas_length = 8;
    while (pas_length != 0)
    {
      door_password1[pas_length--] = 0; // clear array for new data
    }
  }

  if (door && door_cencel) // if password not Matched
  {
    lcd.setCursor(0, 0);
    lcd.print("PASSWORD NOT    ");
    lcd.setCursor(0, 1);
    lcd.print("MATCHED.....    ");
    delay(2000);
    lcd.clear();
  }

  else if (!door)
  {
    set_Door_Password();
    setting_state = true;
    read_EEprom();
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////-------------------CHANGE ADMIN PASS----------------------//////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void change_Admin_pass()
{
  bool admin = true;
  bool admin_cencel = true;
  uint8_t j = 0;
  uint8_t pass_length = 8;
  uint8_t admin_password1[pass_length];
  while (j < pass_length) // Create new password
  {
    uint8_t customKey = customKeypad.getKey();
    if (customKey)
    {
      if (customKey == 'B')
      {
        admin_cencel = false;
        setting_state = true;
        break;
      }
      else if (customKey != 'C')
      {
        admin_password1[j] = customKey;
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
  if (!memcmp(admin_password1, adminPass, pass_length)) // if password Matched
  {
    lcd.setCursor(0, 0);
    lcd.print("PASSWORD MATCHED ");
    lcd.setCursor(0, 1);
    lcd.print("!!!!!!!!         ");
    delay(2000);
    admin = false;
    uint8_t pas_length = 8;
    while (pas_length != 0)
    {
      admin_password1[pas_length--] = 0; // clear array for new data
    }
  }

  if (admin && admin_cencel)
  { // if password not Matched
    lcd.setCursor(0, 0);
    lcd.print("PASSWORD NOT    ");
    lcd.setCursor(0, 1);
    lcd.print("MATCHED.....    ");
    delay(2000);
    lcd.clear();
  }

  else if (!admin)
  {
    set_Admin_Password();
    setting_state = true;
    read_EEprom();
  }
}
