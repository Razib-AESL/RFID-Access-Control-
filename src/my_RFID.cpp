#include <my_RFID.h>

String myTag; 
String content = "";
byte readCard[4];

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

void rfid_sensor_setup() { 
  SPI.begin();           // Initiate  SPI bus
  mfrc522.PCD_Init();    // Initiate MFRC522  
}

uint8_t read_rfid_tag() {
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return 0;
  }
  content = "";
  for ( uint8_t i = 0; i < 4; i++) {  // The MIFARE PICCs that we use have 4 byte UID
    readCard[i] = mfrc522.uid.uidByte[i];
    content.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  content.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
}

/*
void read_rfid_tag() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
  //  return "null";
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
  //  return "null";
    return;
  }
  //Show UID on serial monitor
  
///////////////  String Tag[4];
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    readCard[i] = mfrc522.uid.uidByte[i], HEX;
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  myTag = content.substring(1);
  for(int i = 0; i <4; i++){
  Serial.print(readCard[i]);
  }
  //Serial.print("UID tag :");
  //Serial.println(myTag);
 // return content.substring(1);  
}
*/