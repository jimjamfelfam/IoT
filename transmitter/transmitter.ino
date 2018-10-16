#include  <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int interruptPin = 3;
long last_bounce;
long de_bounce = 40;
bool state = false;

void setup(void){
 pinMode(interruptPin, INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(interruptPin), debounce, RISING);  
 Serial.begin(115200);
 radio.begin();
 radio.openWritingPipe(pipe);
 msg[0] = 111;
}

void loop(){
    while (state){
        radio.write(msg, 1);
    }
}

void debounce(){
 if((millis()-last_bounce)>de_bounce){
    click();
    last_bounce=millis();
 }
 
}

void click(){
 Serial.println("Interrupt");
 if(state){
     state=false;
 }
 else{
     state=true;
 }
}