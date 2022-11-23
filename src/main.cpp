#include <Arduino.h>
#include <main.h>

extern void set_Admin_Password();
extern void set_Door_Password();
extern void set_rfid_Master();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
 // lcd_setup();
 // rfid_sensor_setup();
   set_Admin_Password();
   set_Door_Password();
   set_rfid_Master();
}

void loop() {
  // put your main code here, to run repeatedly:
 /* 
  read_rfid_tag();
  lcd.setCursor(0,0);
  lcd.print("UID: ");
  lcd.print(myTag);
  */
 // char customKey = customKeypad.getKey();
 Serial.println("Loop");
 
}