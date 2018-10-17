void setup() {  
    // initialize inputs and outputs
    pinMode(0, INPUT);
    pinMode(1, INPUT);
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
}

void loop() {
    // read left and right
    if(digitalRead(0)){
        digitalWrite(2, HIGH);
        digitalWrite(2, LOW);
    }
    if(digitalRead(1)){
        digitalWrite(3, HIGH);
        digitalWrite(3, LOW);    
    }
}