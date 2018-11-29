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
      display.println("\nConnecting");
      break;
    default:
      display.println("Default");
      break;    
  }

  display.display();

  while(WiFi.status() == WL_DISCONNECTED){
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
    default:
      display.println("Default");
      break; 
  }

  display.display();

  delay(1000);

  configTime(timezone, dst, "time1.google.com", "ca.pool.ntp.org", "time.nist.gov");
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
  
  // serial print date
  Serial.print(p_tm->tm_mday);
  Serial.print("/");
  Serial.print(p_tm->tm_mon + 1);
  Serial.print("/");
  Serial.print(p_tm->tm_year + 1900);
  
  Serial.print(" ");
  
  //serial print time
  Serial.print(p_tm->tm_hour);
  Serial.print(":");
  Serial.print(p_tm->tm_min);
  Serial.print(":");
  Serial.println(p_tm->tm_sec);
  
  // Clear the buffer.
  display.clearDisplay();
  display.setTextColor(WHITE);
  
  // hours : minutes
  display.setTextSize(4);  
  display.setCursor(2,20);
  if( p_tm->tm_hour <10)
    display.print("0"); 
  display.print(p_tm->tm_hour);
  display.print(":");
  if( p_tm->tm_min <10)
    display.print("0"); 
  display.print(p_tm->tm_min);

  // seconds
  display.fillRect(3,54,2*(p_tm->tm_sec),4,WHITE);

  // day / month / year
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print(p_tm->tm_mday);
  display.print(".");
  display.print(p_tm->tm_mon + 1);
  display.print(".");
  display.print(p_tm->tm_year -100);

  // connection status
  display.setTextSize(1);
  switch(WiFi.status()){
    case 3:
      display.setCursor(73,0);
      display.print("Connected");
      break;
    default:
      display.setCursor(55,0);
      display.print("Disconnected");
      break;
  }

  display.display();

  // delay(1000); // update every 1 sec
}