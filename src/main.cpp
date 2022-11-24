#include <main.h>
//#include <my_passWord.cpp>


bool flag_read_password = true;

extern void set_Admin_Password();
extern void set_Door_Password();
//extern void set_rfid_Master();
void writeUint16IntoEEPROM(uint32_t addr,uint16_t writeVal);
extern bool KeypadHandler();


//extern void writeUint8IntoEEPROMfromuintArr(uint32_t addr,uint8_t writeVal[], uint16_t arr_ln);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
 // EEPROM.begin(0);
 lcd_setup();
 
 if(EEPROM.read(flag_admin_panel_set_ADDRESS) == 255){
   set_Admin_Password();
   set_Door_Password();
   flag_read_password = true;
   }
  
   
}

void loop() {

if(flag_read_password){
  read_password();
  lcd.clear();
}

   KeypadHandler();
   RFID_handler();
}