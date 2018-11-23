#include  <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
int msg0[1];
int msg1[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int interruptPin = 3;
long last_bounce;
long de_bounce = 40;
bool state = true;
bool oldstate = state;

void setup(void){
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), debounce, RISING);  
    Serial.begin(115200);
    radio.begin();
    radio.openWritingPipe(pipe);
    msg0[0] = 111;
    msg1[0] = 222;
}

void loop(){
    if (state!=oldstate){
        if (state){
            for (int i=0; i <= 20; i++){
                radio.write(msg0,1);
                Serial.print(i);
                Serial.println(msg0[0]);
            }
        }
        else{
            for (int a=0; a <= 20; a++){
                radio.write(msg1,1);
                Serial.println(a);
                Serial.println(msg1[0]);
            }
        }
    oldstate=state;
    }
}

void debounce(){
    if((millis()-last_bounce)>de_bounce){
        click();
        last_bounce=millis();
    }
 
}

void click(){
//    Serial.println("Interrupt");
    if(state){
        state=false;
    }
    else{
        state=true;
    }
}