#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <DHT.h>

/* WiFi Settings */
String ssid      = "Jamie";
String password  = "icelandwifi2016";

/* Azure IOT Hub Settings */
String mqtt_server = "dsjafelddf09.azure-devices.net";
String deviceName  = "CurrentMonitor1";
String deviceSAS   = "SharedAccessSignature sr=dsjafelddf09.azure-devices.net%2Fdevices%2FCurrentMonitor1&sig=OR7bX67Le87QOwFbeWjyEOOlaXt3o6cF48Pkmt3XHj0%3D&se=1565308891";
long interval           = 15000; //(ms) - 15 seconds between reports
/* Azure IOT Hub Settings */

#define DHTTYPE DHT11 // DHT11 or DHT22 
#define DHTPIN D1  // 

WiFiClientSecure espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

long lastMsg = 0;
int value = 0;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  LEDOff();
  dht.begin();
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server.c_str(), 8883);
  client.setCallback(callback);
}

void setup_wifi() {
  WiFi.begin(ssid.c_str(), password.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
}

void LEDOn() {
  digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
}

void LEDOff() {
  digitalWrite(BUILTIN_LED, HIGH);   // Turn the LED off (Note that HIGH is the voltage level
}

void callback(char* topic, byte* payload, unsigned int length) {
  LEDOn();
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++) {
    message+=(char)payload[i];
  }
  Serial.println(message);
 
  LEDOff();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    String hubUserString=mqtt_server;
    hubUserString+="/";
    hubUserString+=deviceName;
   
    if (client.connect(deviceName.c_str(), hubUserString.c_str(), deviceSAS.c_str())) {
      /*Serial.println("connected");*/
      String subscribestring="devices/";
      subscribestring+=deviceName;
      subscribestring+="/messages/devicebound/#";
      
      client.subscribe(subscribestring.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void publishTemprature() {
  LEDOn();
  float humidity    = 100;
  float temperature = 200;
//  float heatIndex   = dht.computeHeatIndex(temperature, humidity);
  
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  String json="{ \"DeviceId\":\"";
  json+=deviceName;
  json+="\", \"Temperature\":";
  json+=temperature;
  json+=", \"Humidity\": ";
  json+=humidity;
  json+=" }";

  String publishstring="devices/";
  publishstring+=deviceName;
  publishstring+="/messages/events/";

  Serial.println(json);
  client.publish(publishstring.c_str(), json.c_str());
  LEDOff();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > interval) {
    lastMsg = now;
    ++value;
    // Do the real work here
    publishTemprature();
  }
}

