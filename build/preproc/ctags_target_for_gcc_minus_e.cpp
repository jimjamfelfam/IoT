# 1 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_hw\\oled_hw.ino"
# 1 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_hw\\oled_hw.ino"
# 2 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_hw\\oled_hw.ino" 2
# 3 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_hw\\oled_hw.ino" 2
# 4 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_hw\\oled_hw.ino" 2
# 5 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_hw\\oled_hw.ino" 2


Adafruit_SSD1306 display(16 /*4*/);





void setup() {
  display.begin(0x02 /*|< Gen. display voltage from 3.3V*/, 0x3C);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(1 /*|< Draw 'on' pixels*/);
  display.setCursor(0,0);
  display.println("Hello World");
  display.println("From:Jamie Feldman");
  display.println("TEST TEST TEST TEST");
  display.println("TEST TEST TEST TEST");
  display.println("TEST TEST TEST TEST");
  display.println("TEST TEST TEST TEST");
  display.println("TEST TEST TEST TEST");
  display.println("TEST TEST TEST TEST");
  display.display();

}

void loop() {
  // put your main code here, to run repeatedly:

}
