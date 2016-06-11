
/*
2015.05.29 제1회SW Hackerton CodeBakery Main Device Source

Writer : KDW


*/

#include <MsTimer2.h>
#include <DHT11.h>
#include <SoftwareSerial.h>

#define HBDEBUG(i) i
//#define HBDEBUG(i)
#define rxPin 2
#define txPin 3
bool
heartbeatDetected(int IRSensorPin, int delay)
{
  static int maxValue = 0;
  static bool isPeak = false;
  int rawValue;
  bool result = false;
    
  rawValue = analogRead(IRSensorPin);
  // Separated because analogRead() may not return an int
  rawValue *= (1000/delay);
  /* HBDEBUG(Serial.print(isPeak); Serial.print("p, "));
  HBDEBUG(Serial.print(rawValue); Serial.print("r, "));
  HBDEBUG(Serial.print(maxValue); Serial.print("m, ")); */

  // If sensor shifts, then max is out of whack.
  // Just reset max to a new baseline.
  if (rawValue * 4L < maxValue) {
    maxValue = rawValue * 0.8;
    //HBDEBUG(Serial.print("RESET, "));
  }
  
  // Detect new peak
  if (rawValue > maxValue - (1000/delay)) {
    // Only change peak if we find a higher one.
    if (rawValue > maxValue) {
      maxValue = rawValue;
    }
    // Only return true once per peak.
    if (isPeak == false) {
      result = true;
      //Serial.print(result); Serial.print(",  *");
    }
    isPeak = true;
  } else if (rawValue < maxValue - (3000/delay)) {
    isPeak = false;
    // Decay max value to adjust to sensor shifting
    // Note that it may take a few seconds to re-detect
    // the signal when sensor is pushed on meatier part
    // of the finger. Another way would be to track how
    // long since last beat, and if over 1sec, reset
    // maxValue, or to use derivatives to remove DC bias.
    maxValue-=(1000/delay);
 }
  //HBDEBUG(Serial.print("\n"));
  return result;
}


////////////////////////////////////////////////////////////////////////
// Arduino main code
////////////////////////////////////////////////////////////////////////
int ledPin=13;
int analogPin=0;


const int delayMsec = 60; // 100msec per sample

// The main loop blips the LED and computes BPMs on serial port.

int BPM;

void getBPM() {
  static int beatMsec = 0;
    int heartRateBPM = 0;
    
    if (heartbeatDetected(analogPin, delayMsec)) {
      heartRateBPM = 60000 / beatMsec;
      digitalWrite(ledPin,HIGH);
      beatMsec = 0;
    } else {
      digitalWrite(ledPin,LOW);
    }
    // Note: I assume the sleep delay is way longer than the
    // number of cycles used to run the code hence the error
    // is negligible for math.
    
    beatMsec += delayMsec;
    if (heartRateBPM > 0) {
      BPM = heartRateBPM;
    }
}

//------

int pin=5;    
DHT11 dht11(pin);

SoftwareSerial swSerial(txPin, rxPin);
char data;


void setup()
{
   Serial.begin(9600);
   swSerial.begin(9600);
   Serial.println("Ready..");
 //  MsTimer2::set(60,getBPM);
 //  MsTimer2::start();
}

void loop()
{
  int err;
  float temp, humi;
  Serial.flush();
//  getBPM();
//  Serial.print(BPM);
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