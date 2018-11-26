#include <Arduino.h>
#line 1 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_hw\\oled_hw.ino"
#line 1 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_hw\\oled_hw.ino"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET LED_BUILTIN  //4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#line 13 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_hw\\oled_hw.ino"
void setup();
#line 35 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_hw\\oled_hw.ino"
void loop();
#line 13 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_hw\\oled_hw.ino"
void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
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

