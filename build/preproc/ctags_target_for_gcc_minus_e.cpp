# 1 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\test\\test.ino"
# 1 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\test\\test.ino"
# 2 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\test\\test.ino" 2
# 3 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\test\\test.ino" 2

# 5 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\test\\test.ino" 2
# 6 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\test\\test.ino" 2
# 7 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\test\\test.ino" 2
# 8 "d:\\OneDrive\\Documents\\IoT\\NodeMCU\\test\\test.ino" 2


Adafruit_SSD1306 display(-1);

/* WiFi Settings */
const char* ssid = "jamiee";
const char* password = "jimjamfelfam";

int timezone = -5 * 3600;
int dst = 0;

time_t this_second =0;
time_t last_second =0;

void setup() {
  display.begin(0x02 /*|< Gen. display voltage from 3.3V*/, 0x3C);

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
  display.setTextColor(1 /*|< Draw 'on' pixels*/);

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
