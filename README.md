# sanya-407
https://github.com/Fisher567/1/blob/main/sketch_nov21b.ino
#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10 
#define blue 5 
#define green 6
#define red 7
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key;
bool flag;
long last_time,last_time1;
byte hours,minute,day,month;

void setup() {
  Serial.begin(9600);
  SPI.begin(); 
  mfrc522.PCD_Init();
  for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);

}

void loop() {
  byte blockAddr=4;
  byte dataBlock[16];
  byte trailerBlock   = 7;
  int count;
  byte byte1;
  byte byte2;
  unsigned long id;
  byte buffer[18];
  byte size = sizeof(buffer);
  if ( ! mfrc522.PICC_IsNewCardPresent())
        return;
  if ( ! mfrc522.PICC_ReadCardSerial())
        return;
  mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));

  mfrc522.MIFARE_Read(5, buffer, &size);
  last_time=(long(buffer[3]) << 0) + (long (buffer[2]) << 8) + (long (buffer[1]) << 16 ) + (long (buffer[0]) << 24 );
  
  
  Serial.println("Reading data");
  mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  
  id=(long(buffer[3]) << 0) + (long (buffer[2]) << 8) + (long (buffer[1]) << 16 ) + (long (buffer[0]) << 24 );
  count=(buffer[7]<<8)+buffer[8];
  if (millis()-last_time<10000 ){
    digitalWrite(blue,HIGH);
    flag=1;
  }
  
  if (count && !flag){
  count--;
  
  for (int i=0;i++;i<7) 
    dataBlock[i]=buffer[i];
  dataBlock[7]=count>>8;
  dataBlock[8]=count& 0xFF;
  dataBlock[3]=id;
  dataBlock[2]=id>>8;
  dataBlock[1]=id>>16;
  dataBlock[0]=id>>24;
  
  mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);   
  Serial.println(id, DEC);
  Serial.println(count, DEC);
  digitalWrite(green,HIGH);
  last_time=millis();
  dataBlock[3]=last_time;
  dataBlock[2]=last_time>>8;
  dataBlock[1]=last_time>>16;
  dataBlock[0]=last_time>>24;
  mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);
  }
  if (!count && !flag){
    Serial.println(id, DEC);
    Serial.println(count, DEC);
    digitalWrite(red,HIGH);
  }
  
  mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    /*if (millis()-last_time>=1000 && flag){
    last_time=millis();
    flag=0;
  }*/
  delay(1000);
  digitalWrite(blue,LOW);          
  digitalWrite(green,LOW);
  digitalWrite(red,LOW);
}
/*void attach_card(byte *buffer, byte bufferSize) {
  int count;
  unsigned long id;
  Serial.println(id, DEC);
  Serial.print(count, DEC);
  
}*/
