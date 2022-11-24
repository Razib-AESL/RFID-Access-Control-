#include <LiquidCrystal.h>

#define RS_PIN 21
#define EN_PIN 22
#define D4_PIN 16
#define D5_PIN  4
#define D6_PIN  2
#define D7_PIN 15

#define lcd_colm  16
#define lcd_row   2

// lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);
extern LiquidCrystal lcd;
extern void lcd_setup();
