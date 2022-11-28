#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key;

void setup() {
    Serial.begin(9600); // Initialize serial communications with the PC
    SPI.begin();        // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522 card
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
}
void loop() {
 // unsigned long id = 123456789;
 //   byte date[] ={28,11,22};
  int coust;

    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;
        
    if (  mfrc522.PICC_ReadCardSerial()){
        
    byte blockAddr      = 4;
    byte trailerBlock   = 7;
    byte dataBlock[16];
    byte buffer[18];
    byte size = sizeof(buffer);
    /*
   dataBlock[3] = id;
   dataBlock[2] = id >> 8;
   dataBlock[1] = id >> 16;
   dataBlock[0] = id >> 24;
    
   dataBlock[4] = date[0];
   dataBlock[5] = date[1];
   dataBlock[6] = date[2];

   dataBlock[8] = coust ;
   dataBlock[7] = coust >> 8;
    */
    mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
 
   // Read data from the block
    mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  
    // Write data to the block
     memcpy(dataBlock,buffer,16);
     coust=(buffer[8] << 0) + (buffer[7] << 8);
     coust--;
     dataBlock[8] = coust ;
     dataBlock[7] = coust >> 8;
    mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);

  // Read data from the block
     mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    dump_byte_array(buffer, 16); Serial.println();
   
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    }
}
void dump_byte_array(byte *buffer, byte bufferSize) {  
    Serial.println((long(buffer[3]) << 0) + (long (buffer[2]) << 8) + (long (buffer[1]) << 16 ) + (long  (buffer[0]) << 24 ),DEC);
    Serial.println(String(buffer[4])+"."+ String(buffer[5])+"."+String(buffer[6]));
    Serial.println((buffer[8] << 0) + (buffer[7] << 8),DEC);
}
