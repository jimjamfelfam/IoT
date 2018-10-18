void setup() {  
    // initialize inputs and outputs
    pinMode(4, OUTPUT);         //forward right
    pinMode(5, OUTPUT);         //forward left
    pinMode(0, OUTPUT);         //backward right
    pinMode(1, OUTPUT);         //backward left
    attachInterrupt(digitalPinToInterrupt(2), left_back, RISING);  
    attachInterrupt(digitalPinToInterrupt(3), right_back, RISING);
    long time_pdeg;
}

// main vehichle path
void loop() {
    forward(40);
    turnright(90);
    backward(50);
    turnleft(90)
    backward(40);
    turnright(90);
}

// triggered on the left bumper 
void left_back(){
    if(digitalRead(2)&&digitalRead(3)){
        backward(20);
    }
    else{
        digitalWrite(0,HIGH);
        digitalWrite(1,HIGH);
        delay(100);
        digitalWrite(0,LOW);
        delay(50);
        digitalWrite(1,LOW);
    }
}

// triggered on the right bumper 
void right_back(){
    if(digitalRead(2)&&digitalRead(3)){
        backward(20);
    }
    else{
        digitalWrite(1,HIGH);
        digitalWrite(0,HIGH);
        delay(100);
        digitalWrite(1,LOW);
        delay(50);
        digitalWrite(0,LOW);
    }
}

// vehicle moves backwards
void backwards(delay_time){
    digitalWrite(0,HIGH);
    digitalWrite(1,HIGH);
    delay(delay_time);
    digitalWrite(0,LOW);
    digitalWrite(1,LOW);
}

//vehichle moves forwards
void forwards(int delay_time){
    digitalWrite(4,HIGH);
    digitalWrite(5,HIGH);
    delay(delay_time);
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
}