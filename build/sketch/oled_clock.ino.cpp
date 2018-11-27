#include <Arduino.h>
#line 1 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_clock\\oled_clock.ino"
#line 1 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_clock\\oled_clock.ino"
#include <ESP8266WiFi.h>
#include <time.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

/* WiFi Settings */
const char* ssid     = "jamie";
const char* password = "jimjamfelfam";

int ledPin = 13;

int timezone = -5 * 3600;
int dst = 0;

time_t this_second =0;
time_t last_second =0;

#line 24 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_clock\\oled_clock.ino"
void setup();
#line 114 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_clock\\oled_clock.ino"
void loop();
#line 24 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_clock\\oled_clock.ino"
void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear the buffer.
  display.clearDisplay();
  display.display();
  
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);

  Serial.begin(115200);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0,0);
  display.println("Wifi connecting to ");
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
  display.println("\nConnecting");

  display.display();

  while( WiFi.status() == WL_DISCONNECTED){
      delay(500);
      display.print("."); 
      display.display();       
  }

  // Clear the buffer.
  display.clearDisplay();
  display.display();
  display.setCursor(0,0);
  
  
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
  }

  display.display();

  delay(1000);

  configTime(timezone, dst, "ca.pool.ntp.org", "time.nist.gov", "time1.google.com");
  display.println("Waiting for NTP...");

  while(!time(nullptr)){
    display.print("*");
    delay(1000);
  }

  display.println("Time response....OK"); 
  display.display();  
  delay(1000);

  display.clearDisplay();
  display.display();
}

void loop() {
  
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  
  Serial.print(p_tm->tm_mday);
  Serial.print("/");
  Serial.print(p_tm->tm_mon + 1);
  Serial.print("/");
  Serial.print(p_tm->tm_year + 1900);
  
  Serial.print(" ");
  
  Serial.print(p_tm->tm_hour);
  Serial.print(":");
  Serial.print(p_tm->tm_min);
  Serial.print(":");
  Serial.println(p_tm->tm_sec);
  
  // Clear the buffer.
  display.clearDisplay();
 
  display.setTextSize(2);
  display.setTextColor(WHITE);
  
  // hours : minutes
  display.setCursor(0,0);
  display.print(p_tm->tm_hour);
  display.print(":");
  if( p_tm->tm_min <10)
    display.print("0"); 
  display.print(p_tm->tm_min);

  // seconds
  display.setTextSize(1);
  display.setCursor(57,0);
  display.print(".");
  if( p_tm->tm_sec <10)
    display.print("0"); 
  display.print(p_tm->tm_sec);

  // day / month / year
  display.setTextSize(1);
  display.setCursor(80,0);
  display.print(p_tm->tm_mday);
  display.print(".");
  display.print(p_tm->tm_mon + 1);
  display.print(".");
  display.print(p_tm->tm_year -100);

  // connection status
  display.setTextSize(1);
  switch(WiFi.status()){
    case 3:
      display.setCursor(73,7);
      display.print("Connected");
      break;
    default:
      display.setCursor(55,7);
      display.print("Disconnected");
      break;
  }

  // analog read display
  display.setTextSize(3);
  display.setCursor(0,20);
  display.print(analogRead(A0));

  display.display();

  delay(1000); // update every 1 sec
}
