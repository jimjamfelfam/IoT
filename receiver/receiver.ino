#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int LED1 = 3;

void setup(void){
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1,HIGH);
}

void loop(void){
  if (radio.available()){
    radio.read(msg,1); 
    if(msg[0]==111){
      digitalWrite(LED1, HIGH);
    }
    if(msg[0]==222){
    digitalWrite(LED1,LOW);
    }
  }
  else{
    Serial.println("No radio available");
  }
}