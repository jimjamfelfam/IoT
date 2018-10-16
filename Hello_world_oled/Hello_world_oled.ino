/* Hello World OLED Test */
#include "SSD1306Wire.h"
SSD1306Wire  display(0x3c, 5, 4); // Initialise the OLED display using Wire library
void setup() 
{
  Serial.begin(115200);
  display.init(); // Initialising the UI will init the display too.
  display.flipScreenVertically();
  
  display.clear();
  drawHelloWorld(); 
  display.display();
}

void drawHelloWorld() 
{
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Hello world");
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 10, "Hello world");
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 26, "Hello world");
}