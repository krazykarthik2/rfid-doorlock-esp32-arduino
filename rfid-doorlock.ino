#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   22    // Reset pin for MFRC522 - Module 1
#define SS_PIN    21    // Slave Select pin for MFRC522 - Module 1

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instanceO

void setup() {
  Serial.begin(9600); // Initialize serial communication
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 
  Serial.println("RFID Ready!");
}
#define adminData {0xAD,0x31,0x40,random(0xff),random(0xff)}
#define entryData {0xE0,0x74,0x50,random(0xff),random(0xff)}

byte adminDt []= {0xAD, 0x31, 0x40};
byte  entryDt []= {0xE0, 0x74, 0x50};

MFRC522::Uid lastReadUID ;
void loop() {
  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Print UID of the card
    readUID();
    if (isAdmin()) {

      if (digitalRead(WRITE_BTN) == HIGH) {
        writeUID(entryData);
        while (digitalRead(WRITE_BTN) == HIGH) {}

      }
      if (digitalRead(WRITE_ADMIN_BTN) == HIGH) {
        writeUID(adminData);
        while (digitalRead(WRITE_ADMIN_BTN) == HIGH) {}

      }
    }
    if(digitalRead(DELETE_BTN)==HIGH){
      
    trashUID();
    while(digitalRead(DELETE_BTN==HIGH){}
  }}

}
delay(100);
}
bool isAdmin() {
  for (byte i = 0; i < 3; i++) {
    if (uid.uidByte[i] != adminDt[i])
      return false;

  }
}
bool isEntry() {
  for (byte i = 0; i < 3; i++) {
    if (uid.uidByte[i] != entryDt[i])
      return false;

  }

}
void readUID() {
  Serial.print("UID:");
  lastReadUID = mfrc522.uid;
  printUID(lastReadUID);
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
void writeUID(byte [] data) {
  byte dataLength = sizeof(data);

  // Write data to the card
  MFRC522::StatusCode status = mfrc522.MIFARE_Write(4, data, dataLength); // Address 4

  // Check if data was written successfully
  if (status == MFRC522::STATUS_OK) {
    Serial.println("Module 2 - Data written successfully");
  } else {
    Serial.println("Module 2 - Error writing data");
  }

  delay(2000); // Delay before trying to write again
}
void printUID(MFRC522::Uid uid) {
  String content = "";
  byte letter;
  for (byte i = 0; i < uid.size; i++) {
    Serial.print(uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(uid.uidByte[i], HEX);



    content.concat(String(uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  Serial.println();
}
void trashUID(){
    writeUID({0x0});
  }
