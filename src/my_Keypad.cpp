#include <my_Keypad.h>


byte rowPins[ROWS] = {row1, row2, row3, row4}; //connect to the row pinouts of the keypad

#if COLS < 4
    byte colPins[COLS] = {clm1, clm2, clm3}; //connect to the column pinouts of the keypad
char hexaKeys[ROWS][COLS] = {
      {'1','2','3'},
      {'4','5','6'},
      {'7','8','9'},
      {'*','0','#'}
};
#else 
    byte colPins[COLS] = {clm1, clm2, clm3, clm4}; //connect to the column pinouts of the keypad
    char hexaKeys[ROWS][COLS] = {
          {'1','2','3','A'},
          {'4','5','6','B'},
          {'7','8','9','C'},
          {'*','0','#','D'}
        };
#endif

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

