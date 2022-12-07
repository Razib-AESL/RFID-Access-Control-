#include <my_LCD.h>

LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

void lcd_setup(){
    lcd.begin(lcd_colm, lcd_row);
}

void LCD_print(uint8_t line1_start_pos,String line1_text,uint8_t line2_start_pos, String line2_text, double display_duration, bool is_clear){
  if(is_clear){
    lcd.clear();
  }
  lcd.setCursor(line1_start_pos, 0);
  lcd.print(line1_text);
  lcd.setCursor(line2_start_pos, 1);
  lcd.print(line2_text);
  if(display_duration >0){
    delay(display_duration);
    lcd.clear();
  }
}