#include <main.h>

uint8_t set_Password[PASSWORD_LENGTH];
uint8_t confirm_Password[PASSWORD_LENGTH];
uint8_t door_password[PASSWORD_LENGTH];
uint8_t master_card[RFID_LENGTH];
byte masterCard[RFID_LENGTH];
extern uint8_t doorPass[PASSWORD_LENGTH];
extern uint8_t adminPass[PASSWORD_LENGTH];
extern byte storedCard[RFID_LENGTH];

bool flag_Password_State = true;
bool flag_Set_Password = true;
bool flag_Cencel_Password = true;

bool door_pass = true;
bool admin_set = true;
bool rfid_master = true;
extern bool setting;
uint8_t successRead;
uint8_t Cursor = 0;
bool storeRfid = true;

uint8_t initial_setup = 0;
bool initialConfig = true;

void read_RFID_card();
void check_admin_pass(String val);
extern void LCD_print(uint8_t line1_start_pos, String line1_text, uint8_t line2_start_pos, String line2_text, double display_duration, bool is_clear);

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////CREATE PASSWORD ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void create_Password(String val)
{
  // Create new password
  while (Cursor < PASSWORD_LENGTH)
  {
    uint8_t customKey = customKeypad.getKey();
    if (customKey)
    {
      if (customKey == 'B')
      {
        if (EEPROM.read(flag_admin_panel_set_ADDRESS) == 1) // check initial setup
        {
          flag_Cencel_Password = false;
          flag_Password_State = false;
          break;
        }
        else
        {
          if (flag_Set_Password)
          {
            flag_Password_State = true;
          }
          else if (!flag_Set_Password)
          {
            flag_Password_State = true;
            break;
          }
        }
      }

      else if (customKey != 'C')
      {
        if (flag_Set_Password) // set password
        {
          set_Password[Cursor] = customKey;
        }
        else if (!flag_Set_Password) // confirm password
        {
          confirm_Password[Cursor] = customKey;
        }
        Serial.print(customKey);
        LCD_print(0, val, Cursor, "* ", 0, false);
        Cursor++;
      }

      else if (customKey == 'C') // clear password
      {
        Cursor = 0;
        LCD_print(0, val, Cursor, "                ", 0, false);
      }
    }
  }
  Cursor = 0;
  flag_Set_Password = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////COMPARE SET & CONFIRM PASSWORD /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void compare_set_N_confirm_Pass(int flash_addr)
{
  if (!memcmp(confirm_Password, set_Password, PASSWORD_LENGTH)) // if confirm write password on EEprom
  {
    Serial.println("Password set successful");
    LCD_print(0, "PASSWORD SET    ", 0, "SUCCESSFULLY... ", 2000, false);
    flag_Password_State = false;
    writeUint8IntoEEPROMfromuintArr(flash_addr, set_Password, PASSWORD_LENGTH);
    memset(set_Password, 0, sizeof(set_Password)); // clear array for new data   
  }

  else if (flag_Password_State && flag_Cencel_Password)
  {
    Serial.println("Password set unsuccessful");
    LCD_print(0, "PASSWORD SET    ", 0, "UNSUCCESSFUL... ", 2000, true);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////CREATE ADMIN PASSWORD/////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void create_Admin_password()
{
  if (flag_Set_Password)
  {
    Serial.print("Create Admin Pass:");
    LCD_print(0, "SET ADMIN PASS..", 0, "                ", 0, false);
    create_Password("SET ADMIN PASS..");
  }
  else if (!flag_Set_Password)
  {
    Serial.println("Confirm Admin Pass:");
    LCD_print(0, "CONFIRM PASSWORD", 0, "                ", 0, false);
    create_Password("CONFIRM PASSWORD");
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////ADMIN PASSWORD SET//////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void set_Admin_password()
{
  while (flag_Password_State)
  {
    create_Admin_password(); // Enter First Time For Set Password

    if (flag_Password_State)
    {
      flag_Set_Password = false;
      create_Admin_password(); // Confrim Set Password
      compare_set_N_confirm_Pass(ADMIN_PASS_START_ADDRESS);
    }
  }
  flag_Password_State = true;  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////CREATE DOOR PASSWORD /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void create_Door_password()
{
  if (flag_Set_Password)
  {
    Serial.print("Create Door Pass:");
    LCD_print(0, "SET NEW DOOR PW ", 0, "                ", 0, false);
    create_Password("SET NEW DOOR PW ");
  }
  else if (!flag_Set_Password)
  {
    Serial.println("Confirm Door Pass:");
    LCD_print(0, "CONFIRM PASSWORD", 0, "                ", 0, false);
    create_Password("CONFIRM PASSWORD");
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////DOOR PASSWORD SET///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void set_Door_Password()
{
  while (flag_Password_State)
  {
    create_Door_password(); // Enter First Time For Set Password

    if (flag_Password_State)
    {
      flag_Set_Password = false;
      create_Door_password(); // Confrim Set Password
      compare_set_N_confirm_Pass(DOOR_PASS_START_ADDRESS);
    }
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////READ RFID SET////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void read_RFID_card()
{
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

    if (storeRfid) // store ID if storeRfid true
    {
      if (successRead)
      {
        for (uint8_t j = 0; j < RFID_LENGTH; j++) // Loop 4 times
        {
          masterCard[j] = readCard[j];
          Serial.println(masterCard[j]);
        }

        storeRfid = false;
      }
    }
  }

  while (!successRead); // Program will not go further while you not get a successful read
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////MASTER RFID SET///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void set_rfid_Master()
{
  // uint8_t master_Card[4];
  while (!rfid_master)
  {
    Serial.println("Scan your Master card");
    LCD_print(0, "SCAN NEW MASTER ", 0, "CARD !!!!!      ", 0, false);

    read_RFID_card(); // read rfid tag and store ID in a new masterCard array

    if (!rfid_master)
    {
      Serial.println("Confirm your Master card");
      LCD_print(0, "CONFIRM YOUR....", 0, "MASTER CARD !!! ", 0, false);

      read_RFID_card(); // read rfid tag and not store ID in a new masterCard array
      storeRfid = true;

      if (!rfid_master)
      {
        if (!memcmp(readCard, masterCard, RFID_LENGTH)) // if new scan card match then set it master here
        {
          writeUint8IntoEEPROMfromuintArr(MASTER_CARD_START_ADDRESS, masterCard, RFID_LENGTH);
          Serial.println("Master card same");
          LCD_print(0, "MASTER CARD SET ", 0, "SUCCESSFULLY !!!", 2000, true);
          rfid_master = true;
          memset(masterCard, 0, sizeof(masterCard)); // clear byte array for new data
         }

        else
        {
          Serial.println("Not Matched");
          LCD_print(0, "SCAN CARD NOT...", 0, "MATCHED  !!!!!  ", 2000, true);
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////CHECK DOOR PASSWORD////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

bool check_Door_Password()
{
  uint8_t customKey = customKeypad.getKey();

  if (customKey)
  {
    if (customKey == 'A' && Cursor == 0) // If Press A
    {
      LCD_print(0, "ENTER ADMIN PASS", 0, "", 0, true);
      admin_set = false;
      Cursor = 0;
    }
    if (!admin_set) // check admin pass for settings
    {
      check_admin_pass("ENTER ADMIN PASS");
      admin_set = true;
      return true;
    }
    else if (customKey != 'A' && customKey != 'B' && customKey != 'C' && customKey != 'D' && Cursor == 0)
    {
      LCD_print(0, "ENTER DOOR PASS", Cursor, "* ", 0, true);
      Serial.print(customKey);
      set_Password[Cursor] = customKey;

      Cursor = Cursor + 1;

      create_Password("ENTER DOOR PASS"); // store password in a set_Password array

      if (!memcmp(set_Password, doorPass, PASSWORD_LENGTH)) // if password Matched
      {

        LCD_print(0, "AUTHORIZED ENTRY", 0, "!!!!!!!         ", 1500, false);
        memset(set_Password, 0, sizeof(set_Password)); // clear array for new data
      }
      else // if password not Matched
      {
        if (flag_Cencel_Password)
        {
          LCD_print(0, "ENTRY DENIED !!!! ", 0, "                 ", 1500, false);
        }
      }
    }
  }
  return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////CHECK ADMIN PASS//////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void check_admin_pass(String val)
{

  create_Password(val);

  if (!memcmp(set_Password, adminPass, PASSWORD_LENGTH)) // if confirm write password on EEprom
  {
    LCD_print(0, "   WELCOME TO   ", 0, "-ADMIN SETTINGS-", 2000, true);
    setting = false;
    memset(set_Password, 0, sizeof(set_Password)); // clear array for new data
  }

  else
  {
    if (setting && flag_Cencel_Password && flag_Password_State)
    {
      LCD_print(0, "WRONG PASSWORD..", 0, "TRY AGAIN !!!!! ", 2000, true);
    }
  }
  flag_Password_State = true;
}
