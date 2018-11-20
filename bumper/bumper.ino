#include "Timer.h"
long time_pdeg;
long delay_time;
long mod =1000;
bool right_forward = LOW;
bool left_forward = LOW;
Timer l;
Timer r;

void setup() {  
    // initialize inputs and outputs
    pinMode(22, OUTPUT);         // 1a
    pinMode(23, OUTPUT);         // 2a
    pinMode(24, OUTPUT);         // 3a
    pinMode(25, OUTPUT);         // 4a
    pinMode(26, OUTPUT);         // 1,2 EN
    pinMode(27, OUTPUT);         // 3,4 EN
    pinMode(18, INPUT);
    pinMode(19, INPUT);
    attachInterrupt(digitalPinToInterrupt(18), left_bump, RISING);  // left bumper
    attachInterrupt(digitalPinToInterrupt(19), right_bump, RISING); // right bumper
}

// main 
void loop() {
    l.update();
    r.update();
}

// interrupts
void right_bump(){
    r.after(1*mod,r_stop);
    digitalWrite(22,HIGH);
    digitalWrite(23,LOW);
    digitalWrite(26,HIGH);
}
void left_bump(){
    l.after(1*mod,l_stop);
    digitalWrite(24,HIGH);
    digitalWrite(25,LOW);
    digitalWrite(27,HIGH);
}

// forward and backwards functions
void forward(){
    l.after(1*mod,l_stop);
    r.after(1*mod,r_stop);    
    digitalWrite(24,LOW);
    digitalWrite(25,HIGH);
    digitalWrite(27,HIGH);
    digitalWrite(22,LOW);
    digitalWrite(23,HIGH);
    digitalWrite(26,HIGH);        
}
void backward(){
    l.after(1*mod,l_stop);
    r.after(1*mod,r_stop);    
    digitalWrite(24,HIGH);
    digitalWrite(25,LOW);
    digitalWrite(27,HIGH);
    digitalWrite(22,HIGH);
    digitalWrite(23,LOW);
    digitalWrite(26,HIGH);        
}

// stop functions
void r_stop(){
    digitalWrite(22,LOW);
    digitalWrite(23,LOW);
    digitalWrite(26,LOW);
}
void l_stop(){
    digitalWrite(24,LOW);
    digitalWrite(25,LOW);
    digitalWrite(27,LOW);
}

