#define led 5

int val;
const int analog_ip = A0;

void change(int per){
    // change led pmw duty cycle
    analogWrite(led,per/4);
}

void setup() {
  // initialize led pin
  pinMode(led,OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
    val=analogRead(analog_ip);
    change(val);
}
