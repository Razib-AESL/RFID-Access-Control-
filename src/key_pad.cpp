#include <Key_pad.h>
#include <my_passWord.h>


const int Enter_Button = BUTTON;
int buttonState = 0;
int StateNumber_passwordStates =0;
int flag_stag=0;
int CreateNewPasswordFlag=0;
extern int rfid_step;
int new_id_input = 0;
extern bool flag_read_password;
int Cursor1 = 0;
bool intial_passwant_print = true;

/* right now unnecessary 
unsigned long key_state_current_interrupt = 0;
unsigned long time_of_last_keypad_interrupt = 0;  
*/
volatile bool flag_for_key_state = false;
 

void IRAM_ATTR keypad_EnterButton();
void password_reset();
bool key_pad_scan();
extern bool new_RDID();

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pin_rows[ROW_NUM]      = {Row1, Row2, Row3, Row4}; 
byte pin_column[COLUMN_NUM] = {Column1, Column2, Column3, Column4};

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );


//if want to make password
String password; // change your password here
char key;
String input_key;

bool KeypadHandler();
void read_password();
void password_States();
void keypad_EnterButton();

void keypad_setup() {
  Serial.begin(115200);
  input_key.reserve(32); // maximum input characters is 33, change if needed
  //pinMode(Enter_Button, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(Enter_Button), isr_for_keypad, RISING);
}

//void IRAM_ATTR isr_for_keypad() {
void keypad_EnterButton() {
 // key_state_current_interrupt = millis();
 // if (key_state_current_interrupt - time_of_last_keypad_interrupt > 250)
 // {
	  flag_for_key_state = true;
    //time_of_last_keypad_interrupt = key_state_current_interrupt;
  //}
}

void password_States()
{
  switch (StateNumber_passwordStates)
  {
    case START:
    lcd.clear();
    if (flag_stag == 1 || input_key =="1"){
      StateNumber_passwordStates = CREATE_NEW_PASSWORD;
    }
    else if (input_key =="2"){
      StateNumber_passwordStates = ADD_NEW_RFID;
    }
    else{StateNumber_passwordStates = COMPARE_PASSWORD;}
    break;
//.....................................................
    case COMPARE_PASSWORD:
    Serial.println(input_key);
    if (password == input_key)
    {
      Serial.println("The password is correct, ACCESS GRANTED!");
      lcd.setCursor(0,0);
      lcd.print("Password correct");
      lcd.setCursor(0,1);
      lcd.print(" ACCESS GRANTED!");
      delay(1200);
      lcd.clear();
    }
    else
    {
      Serial.println("The password is incorrect, ACCESS DENIED!");
      lcd.setCursor(0,0);
      lcd.print("Password incorrect");
      lcd.setCursor(0,1);
      lcd.print(" ACCESS DENIED!");
      delay(1200);
      lcd.clear();
    }
    input_key = "";
    flag_for_key_state = false;
    StateNumber_passwordStates=START;
    break;
//....................................................
    case CREATE_NEW_PASSWORD:
    if (CreateNewPasswordFlag == 0){
      lcd.clear();
      Serial.println("Enter Your old password");
      lcd.setCursor(0,0);
      lcd.print("Enter old pass");
      lcd.setCursor(0,1);
      lcd.print("_");
      CreateNewPasswordFlag = 1;
      input_key="";
      flag_for_key_state = false;
      intial_passwant_print = false;
      break;
      }
    if (CreateNewPasswordFlag == 1){
      if (input_key == password)
      {
        lcd.clear();
        Serial.println("Enter new password");
        lcd.setCursor(0,0);
       lcd.print("Enter new pass");
       lcd.setCursor(0,1);
        lcd.print("_");
        CreateNewPasswordFlag = 2;
        input_key="";
        intial_passwant_print = false;
      }
      else{
        lcd.clear();
      Serial.println("invalid");
      lcd.setCursor(0,0);
       lcd.print("invalid");
       lcd.setCursor(0,1);
        lcd.print("_");
        delay(1000);
      lcd.clear();
      intial_passwant_print = true;
      //reset all
      input_key="";
      CreateNewPasswordFlag=0;
      flag_stag = 0;
      StateNumber_passwordStates=0;
      }
      flag_for_key_state = false; 
      
      break;
    }
    if (CreateNewPasswordFlag == 2){
      password ="";
      password = input_key;
      lcd.clear();
      Serial.println("New password is :  ");
             lcd.print("New pass set");
       lcd.setCursor(0,1);
        lcd.print("_");
        delay(1000);
      Serial.println(password);
      input_key="";
      CreateNewPasswordFlag=0;
      flag_stag = 0;
      StateNumber_passwordStates=0;
      flag_for_key_state = false;
      intial_passwant_print = true;
      break;
    }

//................................................
      case ADD_NEW_RFID:
      Serial.println("ADD NEW RFID......");
      lcd.clear();
      lcd.print("ADD NEW RFID");
      //lcd_print1(0,0,"ADD NEW RFID......");
      //lcd_print1(0,1,"ENTER PASSWORD");
      StateNumber_passwordStates = COMPARE_PASSWORD;
      new_id_input = 1;
      flag_for_key_state = false;
      rfid_step = NEW_RFID;
      intial_passwant_print = false;
      break;

  }
}
void read_password()
{
  char key_tap;
  EEPROM.begin(500);
  // ReadEEprom_uint32(208);
  for (int i = DOOR_PASS_START_ADDRESS; i < DOOR_PASS_START_ADDRESS+pass_length ; i++)
  {
    key_tap = EEPROM.read(i);
    password += key_tap;
  }
  flag_read_password = false;
  Serial.print(password);
  Serial.println();
}

bool KeypadHandler()
{
  if(intial_passwant_print){
      lcd.setCursor(0,0);
      lcd.print("Enter Password : ");
  }

  key = keypad.getKey();
  if (key)
  {
     //lcd.clear();
     lcd.setCursor(Cursor1,1);
     lcd.print("*");
     Cursor1++;
     Serial.println(key);
     
     switch (key)
     {
     case 'C':
      lcd.clear();
      Cursor1=0;
      input_key = "";/* code */
      break;

      case 'D':
      keypad_EnterButton();
      Cursor1=0;
      break;
     
     default: input_key += key;
      break;
     }
  }
  if(flag_for_key_state)
  {
    password_States();
  }
  return true;
}
