#include <Arduino.h>
#include <Keypad.h>
#include <my_lcd.h>
#include <rfid.h>

#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // four columns

#define BUTTON      2

//Pins
#define Row1      32
#define Row2      33
#define Row3      25
#define Row4      26
#define Column1   27
#define Column2   14
#define Column3   12
#define Column4   13


#define START                0
#define COMPARE_PASSWORD     1
#define CREATE_NEW_PASSWORD  2
#define RESET_PASSDWORD      3
#define ADD_NEW_RFID         4

extern String password;
extern void read_password(); 
extern bool KeypadHandler();