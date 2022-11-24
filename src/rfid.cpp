#include<rfid.h>
#include <my_lcd.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
String show_RFID;
int rfid_step;
uint8_t mac_bytes;
int mac_siz=0;
uint32_t RfId;
int total_card;
//uint32_t total_rfid[4];
int shift1;
bool new_rdif_flag = false;
uint32_t ReadEEprom_uint32(uint32_t addr);

uint32_t ID1 = {0xA05F5F7A};
uint32_t ID2 = {0xB018987E};

void writeUint32IntoEEPROM(uint32_t addr,uint32_t writeVal);

void rfid_sensor_setup() { 
  SPI.begin();           // Initiate  SPI bus
  mfrc522.PCD_Init();    // Initiate MFRC522  
  digitalWrite(buzzerPin, LOW);
  EEPROM.begin(100);
}

String get_RFID_tag() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
  //  lcd_print1(0, 1, "Touch RFID card");
  //  Serial.print("no card1 :");
    return "null";
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
  //  Serial.print("no card2 :");
    return "null";
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  //byte letter;
  //byte mac_siz = mfrc522.uid.size;
  shift1 = 24;
  RfId = (RfId & 0x00000000);

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
    
    RfId = ((((mfrc522.uid.uidByte[i] << shift1)) & 0xFFFFFFFF) | RfId);
    shift1 = shift1-8;
    Serial.print(mfrc522.uid.uidByte[i],HEX);
    //Serial.println(sizeof(mfrc522.uid.uidByte[i]));
  } 
  //Serial.println(RfId,HEX);
  //Serial.println(sizeof(RfId));
  content.toUpperCase();
  //RfId = content.substring(1);
  //Serial.print(sizeof(content.substring(1)));
  return content.substring(1); 
}


void RFID_States(){
    switch(rfid_step)
    {
    /*case START_RFID:

      break;*/
    case COMPARE_RFID:
      if (ID1 == RfId)
      {
   //     lcd_print1(0, 0, "ID: ");
   //     lcd_print2(show_RFID);
        digitalWrite(buzzerPin, HIGH);
        Serial.println("Authorized Access");
   //     lcd_print1(0, 1, "Authorized Access ");
        delay(150);
        digitalWrite(buzzerPin, LOW);
   //     lcd_clear();
      }
      else
      {
        digitalWrite(buzzerPin, HIGH);
        Serial.println("Wrong card......ACCESS DENIED!");
    //    lcd_print1(0, 1, "Access denied!!!  ");
        delay(1500);
        digitalWrite(buzzerPin, LOW);
    //    lcd_clear();
      }
      rfid_step = COMPARE_RFID; 
      break;

    case NEW_RFID:
    /*if (ID2 == RfId){
      lcd_print1(0, 0, "ID: ");
      lcd_print2(show_RFID);
        Serial.println("Card already exist");
        digitalWrite(buzzerPin, HIGH);
        lcd_print1(0, 1, "Card already exist ");
        delay(150);
        digitalWrite(buzzerPin, LOW);
    }
    else{*/
    if (new_rdif_flag){
     //   lcd_print1(0, 0, "ID: ");
      //  lcd_print2(show_RFID);
        digitalWrite(buzzerPin, HIGH);
        Serial.println("New Card Added");
        //add new card in flash 
     //   lcd_print1(0, 1, "New Card Added");
        //ReadEEprom_uint32(0);

        writeUint32IntoEEPROM(0, RfId);
        delay(150);
        digitalWrite(buzzerPin, LOW);
    }
    //}
    new_rdif_flag = false;
    rfid_step = COMPARE_RFID;
    break;
    }
}

void writeUint32IntoEEPROM(uint32_t addr,uint32_t writeVal)
{
  EEPROM.begin(1000); //need to change 
  shift1=24;
  for(int l=0; l<4; l++){
    uint8_t a = ((writeVal>>shift1) & 0xFF);
    //Serial.println(a);
    EEPROM.write(addr, (a));
    addr++;
    EEPROM.commit();
    a = 0;
    shift1 = shift1-8;
  }
  Serial.println("EEPROM Write done");
  for (size_t i = 0; i < addr; i++)
  {
  Serial.print(EEPROM.read(i),HEX);
  Serial.print(",");
  }
  Serial.println("");
}
//0-50      //common variable
//51 - 2000 //rfid
//2001-2050 //password

/// latest address of RFID in EEPROM = (0- 3)
/// addr of admin password in EEPROM = (4-7)
/// addr of door password in EEPROM = (8-11)

/// addr of admin password in flash memory (2001 - 2008)
/// addr of door password in flash memory (2009 - 2016)

/*
void writeUint8IntoEEPROMfromuintArr(uint32_t addr,uint8_t writeVal[], uint16_t arr_ln)
{
  EEPROM.begin(arr_ln); //need to change 
  for(int l=0; l<arr_ln; l++){
    EEPROM.write(addr, writeVal[l]);
    addr++;
    EEPROM.commit();
  }
  Serial.println("EEPROM Write done");
  for (size_t i = addr; i < addr+arr_ln; i++)
  {
  Serial.print(EEPROM.read(i),HEX);
  Serial.print(",");
  }
  Serial.println("");
}
*/
/*
bool save_data_in_flash(){//12
  Serial.print(mac_siz);
  mac_siz = sizeof(show_RFID); 
  int start_add = 0;
  for (int j; j<mac_siz; j++){
  EEPROM.write(start_add, RfId);
  EEPROM.commit();
  EEPROM.read(start_add);
  start_add++;
  }
  total_card++;
  return true;
}
*/

/*String check_RFID(){
  for (int a=0; a<mac_siz; a++)
  {

  }
} */

bool RFID_handler()
{
  show_RFID = get_RFID_tag();
  // Serial.println(show_RFID);
  if (show_RFID != "null")
  {
    Serial.println(show_RFID);
    if (ID2 == RfId)
    {
      rfid_step = NEW_RFID;
      new_rdif_flag = true;
      return true;
    }
    RFID_States();
  }

  return true;
}
