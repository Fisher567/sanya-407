//---------------библиотеки и переменные--------------------------
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
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);

}

void loop() {
//-------------переменные---------------------------
  byte blockAddr=4;
  byte dataBlock[16];
  byte trailerBlock   = 7;
  int count;
  byte byte1;
  byte byte2;
  unsigned long id;
  byte buffer[18];
  byte size = sizeof(buffer);
//------------начало считывания----------------------
  if ( ! mfrc522.PICC_IsNewCardPresent())
        return;
  if ( ! mfrc522.PICC_ReadCardSerial())
        return;
  mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
//-----------------время применния карты------------------------------------------------------
  mfrc522.MIFARE_Read(5, buffer, &size);
  last_time=(long(buffer[3]) << 0) + (long (buffer[2]) << 8) + (long (buffer[1]) << 16 ) + (long (buffer[0]) << 24 );
  
  
  Serial.println("Reading data");
  //-------------id и кол-во поездок--------------------
  mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  
  id=(long(buffer[3]) << 0) + (long (buffer[2]) << 8) + (long (buffer[1]) << 16 ) + (long (buffer[0]) << 24 );
  count=(buffer[7]<<8)+buffer[8];
//---------не прошло время после прошлой оплаты--------
    if ( millis()-last_time<10000){
    digitalWrite(blue,HIGH);
    Serial.println(id, DEC);
    Serial.println(count, DEC);
  }
//---------------списание поездок-------------
  else{
  if (count>0){
  count--;
  for (int i=0;i++;i<7) 
    dataBlock[i]=buffer[i];
  dataBlock[8]=count& 0xFF;                   //разбор на байты
  dataBlock[7]=count>>8;
  dataBlock[3]=id;
  dataBlock[2]=id>>8;
  dataBlock[1]=id>>16;
  dataBlock[0]=id>>24;
  
  mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);   //запись поездок и id
  Serial.println(id, DEC);
  Serial.println(count, DEC);
  digitalWrite(green,HIGH);
  last_time=millis();                         //перезапись времени оплаты
  dataBlock[3]=last_time;
  dataBlock[2]=last_time>>8;
  dataBlock[1]=last_time>>16;
  dataBlock[0]=last_time>>24;
  mfrc522.MIFARE_Write(5, dataBlock, 16);     //запись времени
  }
//----------нет поездок---------------------
  else{
    Serial.println(id, DEC);
    Serial.println(count, DEC);
    digitalWrite(red,HIGH);
  }
  }
  mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();

  delay(1000);                                //задержка для выключения всех светодиодов
  digitalWrite(blue,LOW);          
  digitalWrite(green,LOW);
  digitalWrite(red,LOW);
}
