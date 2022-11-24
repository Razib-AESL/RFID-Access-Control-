#include <my_LCD.h>

LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

void lcd_setup(){
    lcd.begin(lcd_colm, lcd_row);
}