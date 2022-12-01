#include <my_RFID.h>

String myTag; 
String content = "";
byte readCard[4];

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

void rfid_sensor_setup() { 
  SPI.begin();           // Initiate  SPI bus
  mfrc522.PCD_Init();    // Initiate MFRC522  
}


uint8_t get_rfid_tag() {
  // Getting ready for Reading Tags
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new Tag placed to RFID reader continue
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a Tag placed get Serial and continue
    return 0;
  }
  // There are Mifare Tags which have 4 byte or 7 byte UID care if you use 7 byte Tag
  // I think we should assume every Tag as they have 4 byte UID
  // Until we support 7 byte Tags
  for ( uint8_t i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522.uid.uidByte[i];
  }
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
}
