#include <main.h>
extern void set_Admin_Password();
extern void set_Door_Password();
extern void set_rfid_Master();
void writeUint16IntoEEPROM(uint32_t addr,uint16_t writeVal);

int statevar = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
 
 for (size_t i = 0; i < 510; i++)
 {
   Serial.print(EEPROM.read(i));
   Serial.print(" ");
 }
 if(EEPROM.read(flag_admin_panel_set_ADDRESS) == 255){
   set_Admin_Password();
   set_Door_Password();
   }
   
}

void loop() {

}