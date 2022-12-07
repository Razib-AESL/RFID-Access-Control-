#include <main.h>

#define ADD_NEW_CARD 0
#define DELETE_NEW_CARD 1
#define CHANGE_MASTER_CARD 2
#define CHANGE_DOOR_PASS 3
#define CHANGE_ADMIN_PASS 4

void add_New_card();
void delete_Exist_Card();
void change_Door_pass(String val);
void change_Admin_pass(String val);
void change_Master_card();
bool check_New_Card_Validity(uint8_t *tagArr, uint8_t *scanArr);
bool scan_EEprom_for_Add_N_Delete_card();
uint8_t read_addNewCard_arr[SIZE_ADD_NEW_CARD_MEMORY + SIZE_RFID_CARDIN_BYTE];

extern uint8_t doorPass[PASSWORD_LENGTH];
extern uint8_t adminPass[PASSWORD_LENGTH];
extern byte storedCard[RFID_LENGTH];
extern byte masterCard[RFID_LENGTH];
extern uint8_t set_Password[PASSWORD_LENGTH];
extern bool flag_Set_Password;
extern bool flag_Cencel_Password;
extern bool flag_Password_State;
extern bool storeRfid;
extern bool rfid_master;
bool new_Card = true;
bool new_Card_cencel = true;

byte new_Stored_Card[RFID_LENGTH];
uint8_t new_card_number = 0;

uint8_t successRead1;
int state = 0;
bool setting = true;
bool setting_state = true;
bool flag_delete_tag = false;

extern void LCD_print(uint8_t line1_start_pos, String line1_text, uint8_t line2_start_pos, String line2_text, double display_duration, bool is_clear);
extern void create_Password(String val);
extern void read_RFID_card();

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////----------------------MY SETTINGS-----------------------////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void my_Settings()
{

  if (!setting)
  {
    switch (state)
    {
    case ADD_NEW_CARD:
      LCD_print(0, "ADD NEW CARD    ", 0, "D:OK #:NEXT B:EX", 0, false);
      check_Key_Press();
      break;

    case DELETE_NEW_CARD:
      LCD_print(0, "DELETE CARD     ", 0, "D:OK #:NEXT B:EX", 0, false);
      check_Key_Press();
      break;

    case CHANGE_MASTER_CARD:
      LCD_print(0, "CHNG MASTER CARD", 0, "D:OK #:NEXT B:EX", 0, false);
      check_Key_Press();
      break;
    case CHANGE_DOOR_PASS:
      LCD_print(0, "CHANGE DOOR PW  ", 0, "D:OK #:NEXT B:EX", 0, false);
      check_Key_Press();
      break;
    case CHANGE_ADMIN_PASS:
      LCD_print(0, "CHANGE ADMIN PW ", 0, "D:OK #:NEXT B:EX", 0, false);
      check_Key_Press();
      break;
    }
    delay(50);
  }

  else
  {
    if (setting)
    {
      LCD_print(0, "SCAN YOUR CARD..", 0, "ENTER PASSWORD..", 0, false);
      successRead1 = get_rfid_tag(); // sets successRead to 1 when we get read from reader otherwise 0

      check_Door_Password();

      // if successread == 1 then enter the loop
      if (successRead1 == 1) // the program will not go further while you are not getting a successful read
      {
        scan_EEprom_for_Add_N_Delete_card();
        if (check_New_Card_Validity(readCard, read_addNewCard_arr))
        {
          LCD_print(0, "AUTHORIZED ENTRY ", 0, "                 ", 0, false);
          digitalWrite(BUZZER_PIN, HIGH);
          delay(100);
          digitalWrite(BUZZER_PIN, LOW);
          delay(100);
          digitalWrite(BUZZER_PIN, HIGH);
          delay(100);
          digitalWrite(BUZZER_PIN, LOW);
          delay(1500);
        }
        else
        {
          LCD_print(0, "ENTRY DENIED !!! ", 0, "                 ", 0, false);

          digitalWrite(BUZZER_PIN, HIGH);
          delay(800);
          digitalWrite(BUZZER_PIN, LOW);
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
      LCD_print(0, "SCAN YOUR NEW...", 0, "RFID CARD !!!   ", 0, false);
      add_New_card();
    }
    else if (state == DELETE_NEW_CARD)
    {
      LCD_print(0, "SCAN YOUR OLD...", 0, "FOR DELETE  !!! ", 0, false);
      delete_Exist_Card();
    }

    else if (state == CHANGE_MASTER_CARD)
    {
      LCD_print(0, "SCAN YOUR OLD...", 0, "MASTER CARD !!! ", 0, false);
      change_Master_card();
    }

    else if (state == CHANGE_DOOR_PASS)
    {
      LCD_print(0, "PUT OLD PASSWORD", 0, "                ", 0, false);
      change_Door_pass("PUT OLD PASSWORD");
    }

    else if (state == CHANGE_ADMIN_PASS)
    {
      LCD_print(0, "PUT OLD PASSWORD", 0, "                ", 0, false);
      change_Admin_pass("PUT OLD PASSWORD");
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////-----------SCAN EEPROM FOR ADD AND DELETE CARD----------////////////////////
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
/////////////////---------------CHECK NEW CARD VALIDITY------------------////////////////////
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
/////////////////--------------------READ NEW CARD-----------------------////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void read_New_card()
{
  do
  {
    successRead1 = get_rfid_tag(); // sets successRead to 1 when we get read from reader otherwise 0
    uint8_t customKey = customKeypad.getKey();
    if (customKey == 'B')
    {
      new_Card_cencel = true;
      setting_state = true;
      break;
    }

    if (new_Card_cencel)
    {
      if (successRead1)
      {
        for (uint8_t j = 0; j < 4; j++) // Loop 4 times
        {
          new_Stored_Card[j] = readCard[j];
        }
        new_Card_cencel = false;
      }
    }
  } while (!successRead1); // Program will not go further while you not get a successful read
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////---------------------ADD NEW CARD-----------------------////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void add_New_card()
{
  read_New_card();

  if (!new_Card_cencel)
  {
    Serial.println("Confirm your New card");
    LCD_print(0, "CONFIRM YOUR NEW", 0, "SCAN CARD !!    ", 0, false);

    read_New_card(); // READ NEW RFID TAG SECOND TIME HERE //

    // CONFIRM NEW CARD AND WRITE CARD TO THE EEPROM //
    if (!memcmp(readCard, new_Stored_Card, 4) && !new_Card_cencel)
    {
      LCD_print(0, "YOUR SCAN CARD !!!", 0, "MATCHED.....    ", 2000, false);
      new_card_number = EEPROM.read(NUMBER_OF_RFID_TAG_ADDRESS);

      if (new_card_number == 255)
      {
        new_card_number = 0;
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
          LCD_print(0, "NEW CARD ADDED..", 0, "SUCCESSFULLY !! ", 2000, false);

          new_Card = false;
          memset(new_Stored_Card, 0, sizeof(new_Stored_Card)); // clear byte array for new data
        }

        else if (new_card_number > MAXIMUM_CARD_NUMBER)
        {
          Serial.println("ALREADY EXIST !! OR MEMORY FULL..!! ");
          LCD_print(0, "YOUR MEMORY FULL", 0, "PLZ DELETE CARD ", 2000, false);
        }
      }
      else
      {
        Serial.println("ALREADY EXIST  ");
        LCD_print(0, "ALREADY EXIST   ", 0, "                 ", 2000, false);
      }
      setting_state = true;
      state = ADD_NEW_CARD;
    }
    else if (!new_Card_cencel)
    {
      LCD_print(0, "YOUR SCAN CARD..", 0, "NOT MATCHED !!! ", 2000, false);
    }
  }
  new_Card = true;
  new_Card_cencel = true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////----------------------DELETE CARD-----------------------////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void delete_Exist_Card()
{

  read_New_card();

  if (!new_Card_cencel)
  {
    Serial.println("Confirm your New card");
    LCD_print(0, "SCAN CONFIRM FOR", 0, "DELETE CARD !!! ", 0, false);

    read_New_card(); // READ NEW RFID TAG SECOND TIME HERE //

    // CONFIRM NEW CARD AND WRITE CARD TO THE EEPROM
    if (!memcmp(readCard, new_Stored_Card, 4) && !new_Card_cencel)
    {
      LCD_print(0, "YOUR SCAN CARD !!!", 0, "MATCHED.....   ", 1500, false);
      flag_delete_tag = true;
      new_card_number = EEPROM.read(NUMBER_OF_RFID_TAG_ADDRESS);
      scan_EEprom_for_Add_N_Delete_card();
      Serial.println("scan complete");

      if (check_New_Card_Validity(readCard, read_addNewCard_arr))
      {
        Serial.println("not match");
        if (new_card_number <= MAXIMUM_CARD_NUMBER)
        {
          writeEEprom_1byte(NUMBER_OF_RFID_TAG_ADDRESS, new_card_number - 1);
          Serial.println("CARD DELETE.. SUCCESSFULLY");
          LCD_print(0, "DELETE YOUR CARD", 0, "SUCCESSFULLY !! ", 2000, false);
          new_Card = false;
          memset(new_Stored_Card, 0, sizeof(new_Stored_Card)); // clear byte array for new data
        }
      }
      else
      {
        Serial.println("YOUR CARD NOT EXIST  ");
        LCD_print(0, "YOUR CARD NOT...", 0, "EXIST  !!!!!    ", 2000, false);
      }
      setting_state = true;
      state = DELETE_NEW_CARD;
    }
    else if (!new_Card_cencel)
    {
      LCD_print(0, "YOUR SCAN CARD..", 0, "NOT MATCHED !!! ", 2000, false);
    }
  }
  new_Card = true;
  new_Card_cencel = true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
////////////////-------------------CHECK MASTER CARD----------------------///////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void change_Master_card()
{
  bool master = true;

  read_RFID_card(); // read rfid tag and store ID in a new masterCard array

  if (!memcmp(storedCard, masterCard, RFID_LENGTH))
  {
    LCD_print(0, "SCAN MASTER CARD", 0, "MATCHED  !!!    ", 2000, false);
    master = false;
    storeRfid = true;
    rfid_master = false;
    memset(masterCard, 0, sizeof(masterCard)); // clear byte array for new data
  }

  if (master && !storeRfid)
  {
    LCD_print(0, "SCAN MASTER CARD", 0, "NOT MATCHED  !!! ", 2000, false);
  }

  if (!master && storeRfid)
  {
    set_rfid_Master();
    read_EEprom();
  }
  setting_state = true;
  storeRfid = true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////-------------------CHANGE DOOR PASS----------------------///////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void change_Door_pass(String val)
{
  bool door = true;

  create_Password(val);

  if (!memcmp(set_Password, doorPass, PASSWORD_LENGTH)) // if password Matched
  {
    LCD_print(0, "PASSWORD MATCHED", 0, "!!!!!!!!         ", 2000, false);
    door = false;
    memset(set_Password, 0, sizeof(set_Password)); // clear array for new data
  }

  if (door && flag_Cencel_Password) // if password not Matched
  {
    LCD_print(0, "PASSWORD NOT    ", 0, "MATCHED  !!!    ", 2000, true);
  }

  else if (!door)
  {
    set_Door_Password();
    read_EEprom();
  }

  setting_state = true;
  flag_Cencel_Password = true;
  flag_Set_Password = true;
  flag_Password_State = true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////-------------------CHANGE ADMIN PASS----------------------//////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void change_Admin_pass(String val)
{
  bool admin = true;

  create_Password(val);

  if (!memcmp(set_Password, adminPass, PASSWORD_LENGTH)) // if password Matched
  {
    LCD_print(0, "PASSWORD MATCHED", 0, "!!!!!!!!         ", 2000, false);
    admin = false;
    memset(set_Password, 0, sizeof(set_Password)); // clear array for new data
  }

  if (admin && flag_Cencel_Password) // if password not Matched
  {
    LCD_print(0, "PASSWORD NOT....", 0, "MATCHED  !!!    ", 2000, false);
  }

  else if (!admin)
  {
    set_Admin_password();
    read_EEprom();
  }
  setting_state = true;
  flag_Cencel_Password = true;
  flag_Set_Password = true;
  flag_Password_State = true;
}
