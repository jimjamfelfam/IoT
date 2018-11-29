#include <Arduino.h>
#line 1 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\test\\test.ino"
#line 1 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\test\\test.ino"
#include <ESP8266WiFi.h>
#include <time.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

/* WiFi Settings */
const char* ssid     = "jamiee";
const char* password = "jimjamfelfam";

int timezone = -5 * 3600;
int dst = 0;

time_t this_second =0;
time_t last_second =0;

#line 22 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\test\\test.ino"
void setup();
#line 38 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\test\\test.ino"
void loop();
#line 47 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\test\\test.ino"
void displayinit();
#line 22 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\test\\test.ino"
void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear the buffer.
  display.clearDisplay();
  display.display();
 
  Serial.begin(115200);

  displayinit();
  
  // Clear the buffer.
  display.clearDisplay();
  display.display();
}

void loop() {
  display.clearDisplay();
  display.setTextSize(5);  
  display.setCursor(0,16);
  display.print(analogRead(0));
  display.display();
  delay(10);
}

void displayinit(){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0,0);
  display.println("Wifi connecting to");
  display.println( ssid );

  WiFi.mode(WIFI_STA);
  switch (WiFi.begin(ssid,password)){
    case 3:
      display.println("Connected");
      break;
    case 4:
      display.println("Connection Failed");
      break;
    case 6:
      display.println("Disconnected");
      break;
    default:
      display.println("Default");
      break;    
  } 
  display.println("Connecting");

  display.display();

  while( WiFi.status() == WL_DISCONNECTED){
      delay(500);
      display.print("."); 
      display.display();       
  }
  display.print("\n"); 
  switch(WiFi.status()){
    case 0:
      display.println("Idle");
      break;
    case 1:
      display.println("No SSID Avail");
      break;
    case 3:
      display.println("Wifi Connected!");
      display.print("IP:");
      display.println(WiFi.localIP() );
      break;
    case 4:
      display.println("Failed");
      break;
    case 6:
      display.println("Disonnected");
      break;
    default:
      display.println("Default");
      break;
  }
  display.display();

  delay(4000);
}
