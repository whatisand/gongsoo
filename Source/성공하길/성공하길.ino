/*
2015.05.29 제1회SW Hackerton CodeBakery Main Device Source

Writer : KDW


*/


#include <DHT11.h>
#include <SoftwareSerial.h>


#define rxPin 2
#define txPin 3
#define HBDEBUG(i) i


int pin=5;    
DHT11 dht11(pin); 




SoftwareSerial swSerial(txPin, rxPin);
char data;

const int delayMsec = 60;

void setup()
{
   Serial.begin(9600);
   swSerial.begin(9600);
   Serial.println("Ready..");
}

void loop()
{
  int err;
  float temp, humi;
  Serial.flush();
 
  Serial.print(verti);
  Serial.print(" horizen: ");
  Serial.print(hori);
  Serial.println();
  
  */
  if((err=dht11.read(humi, temp))==0)
  {
    
    Serial.print("temperature:");
    Serial.print(temp);
    Serial.print(" humidity:");
    Serial.print(humi);
    Serial.println();
  
}
  else
  {
    Serial.println();
    Serial.print("Error No :");
    Serial.print(err);
    Serial.println();    
  }
 
      swSerial.print((int)temp);
      swSerial.print("E");
    
      swSerial.print((int)humi);
     swSerial.print("E");
     
     swSerial.println();
    
   Serial.println();
  
  delay(1000);

  
  while(swSerial.available()) {
      data = swSerial.read();
      if(data == 1) break;
      Serial.print(data);
      delay(1);
   }
  

  
}
