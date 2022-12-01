#include <main.h>

extern void writeUint8IntoEEPROMfromuintArr(uint32_t addr, uint8_t writeVal[], uint16_t arr_ln);
extern void writeUint16IntoEEPROM(uint32_t addr, uint16_t writeVal);
extern void set_Admin_Password();
extern void set_Door_Password();
extern void set_rfid_Master();
extern void read_EEprom();
extern void lcd_setup();
extern void rfid_sensor_setup();
extern void my_Settings();

extern uint8_t successRead;
extern byte storedCard[4];
extern Keypad customKeypad;
extern bool door_pass;

void writeUint16IntoEEPROM(uint32_t addr, uint16_t writeVal);

void setup()
{
  // put your setup code here, to run once:
 // Serial.begin(115200);
  EEPROM.begin(500);
  pinMode(buzzerPin, OUTPUT);
  lcd_setup();
  rfid_sensor_setup();
  lcd.setCursor(0, 0);
  lcd.print("   WELCOME TO   ");
  lcd.setCursor(0, 1);
  lcd.print("-ACCESS CONTROL-");
  delay(2500);

  for (size_t i = 0; i < 510; i++)
  {
    Serial.print(EEPROM.read(i));
    Serial.print(" ");
    EEPROM.write(i, 255);
    EEPROM.commit();
  }

  if (EEPROM.read(flag_admin_panel_set_ADDRESS) == 255)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DEVICE INITIAL..");
    lcd.setCursor(0, 1);
    lcd.print("CONFIGURATION !!");
    delay(2000);
    lcd.clear();

    set_Admin_Password();
    set_Door_Password();
    set_rfid_Master();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DEVICE CONFIGURE");
    lcd.setCursor(0, 1);
    lcd.print("SUCCESSFULLY !!!");
    delay(2000);
    lcd.clear();
  }
  read_EEprom();
}

void loop()
{
  my_Settings();
}