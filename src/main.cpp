#include <main.h>

// extern void set_Admin_passwordword();
extern void set_Admin_password();
extern void set_Door_Password();
extern void set_rfid_Master();
extern void read_EEprom();
extern void my_Settings();
extern void LCD_print(uint8_t line1_start_pos, String line1_text, uint8_t line2_start_pos, String line2_text, double display_duration, bool is_clear);
extern bool rfid_master;
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  EEPROM.begin(500);
  pinMode(BUZZER_PIN, OUTPUT);
  lcd_setup();
  rfid_sensor_setup();

  LCD_print(0, "   WELCOME TO   ", 0, "-ACCESS CONTROL-", 2500, false);

  for (size_t i = 0; i < 510; i++)
  {
    Serial.print(EEPROM.read(i));
    Serial.print(" ");
      EEPROM.write(i, 255);
      EEPROM.commit();
  }

  if (EEPROM.read(flag_admin_panel_set_ADDRESS) == 255)
  {
    LCD_print(0, "DEVICE INITIAL..", 0, "CONFIGURATION !!", 2000, true);
    set_Admin_password();
    set_Door_Password();
    rfid_master = false;
    set_rfid_Master();
    EEPROM.write(flag_admin_panel_set_ADDRESS, 1);
    EEPROM.commit();
    LCD_print(0, "DEVICE CONFIGURE", 0, "SUCCESSFULLY !!!", 2000, true);
  }
  read_EEprom();
}

void loop()
{
  my_Settings();
}