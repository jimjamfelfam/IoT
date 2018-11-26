# 1 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_clock\\oled_clock.ino"
# 1 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_clock\\oled_clock.ino"
# 2 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_clock\\oled_clock.ino" 2
# 3 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_clock\\oled_clock.ino" 2

# 5 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_clock\\oled_clock.ino" 2
# 6 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_clock\\oled_clock.ino" 2
# 7 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_clock\\oled_clock.ino" 2
# 8 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\oled_clock\\oled_clock.ino" 2


Adafruit_SSD1306 display(-1);

/* WiFi Settings */



int ledPin = 13;

int timezone = 7 * 3600;
int dst = 0;

void setup() {
  display.begin(0x02 /*|< Gen. display voltage from 3.3V*/, 0x3C);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  pinMode(ledPin,0x01);
  digitalWrite(ledPin,0x0);

  Serial.begin(115200);

  display.setTextSize(1);
  display.setTextColor(1 /*|< Draw 'on' pixels*/);

  display.setCursor(0,0);
  display.println("Wifi connecting to ");
  display.println( "Jamie" );

  WiFi.mode(WIFI_STA);
  WiFi.begin("Jamie","jimjamfelfam");

  display.println("\nConnecting");

  display.display();

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      display.print(".");
      display.display();
  }

  // Clear the buffer.
  display.clearDisplay();
  display.display();
  display.setCursor(0,0);

  display.println("Wifi Connected!");
  display.print("IP:");
  display.println(WiFi.localIP() );

  display.display();

  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  display.println("\nWaiting for NTP...");

  while(!time(nullptr)){
     Serial.print("*");

     delay(1000);
  }
  display.println("\nTime response....OK");
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

  display.setTextSize(3);
  display.setTextColor(1 /*|< Draw 'on' pixels*/);

  display.setCursor(0,0);
  display.print(p_tm->tm_hour);
  display.print(":");
  if( p_tm->tm_min <10)
    display.print("0");
  display.print(p_tm->tm_min);

  display.setTextSize(2);
  display.setCursor(90,5);
  display.print(".");
  if( p_tm->tm_sec <10)
    display.print("0");
  display.print(p_tm->tm_sec);

  display.setTextSize(1);
  display.setCursor(0,40);
  display.print(p_tm->tm_mday);
  display.print("/");
  display.print(p_tm->tm_mon + 1);
  display.print("/");
  display.print(p_tm->tm_year + 1900);

  display.display();

  delay(1000); // update every 1 sec

}
