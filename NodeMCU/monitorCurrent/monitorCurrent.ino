/*
 monitorCurrent 
 Connects with the Azure IoT hub and sends readings for a non invasice current trasformer
 Designed to be used with solar panels for monitoring energy production of resedential adresses

 add from Arduino library.  You may need http://arduino.esp8266.com/stable/package_esp8266com_index.json
 be sure to adjust packet size, stated here: https://kevinsaye.wordpress.com/2016/09/22/connecting-an-esp8266-to-azure-iot-remote-monitoring-solution/

 3.2  added limits on connectToIoT and hard coded the SAS token
 3.3  added web site for interaction
 3.4  testing OTA updates
 3.5  testing OTA updates
 3.6  testing Calibrate
 3.7  testing stability (uptime)
 */
#include <ArduinoJson.h>
#include <ESP8266httpUpdate.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiAP.h>
#include "EmonLib.h"
#include "FS.h"

/* Server Setup*/
WiFiClientSecure espClient;
PubSubClient client(espClient);
ESP8266HTTPUpdate OTA;
HTTPClient http;
EnergyMonitor emon1;
ESP8266WebServer server (80);

/* Device settings */ 
String web_UserName   = "admin";
String web_Password   = "admin";
String devicePassword = "GG0IaUO2UqaejsEkdtzlwBbG9kab/RZKoE4K+SeEKe4=";

/* WiFi Settings */
String ssid      = "Just Hold on Wifi's at Home";
String password  = "Drakecauseidetthewifi";

/* Azure IOT Hub Settings */
String serverName  = "jafelhub.azure-devices.net";
String deviceName  = "currentMonitor";
String deviceSAS   = "SharedAccessSignature sr=jafel-hub.azure-devices.net%2Fdevices%2FcurrentMonitor&sig=N%2FRHus5SkH3U04JSrNfkrJyynNskeRbSrqIA1DLI3Vg%3D&se=1574623280";
long interval      = 15000; //(ms) - 15 seconds between reports

/* Calibration Settings */
float calibration     = .00020;         // default value for calibration
float calibrationchg  = .00005;         // for each calibration cycle, how much to move the calibration
float calibrationmtc  = .00005;         // how close the calibration should get
float minimumReading  = .00;            // if less than 'minimumReading' amps, we don't care.  sensors are not exact

/* Package Setigns*/
float sketchVersion   = 4.0;            // just a version of this sketch.   Used for when you OTA and to ensure the correct version is running
boolean readSettings  = false;
String Header         = "<html><head><meta http-equiv='refresh' content='30; url=/'><style> body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; } table { margin-left: auto; margin-right: auto;} </style></head><body><table>";
String Footer         = "</table></body></html>";
String settingsFileName = "/settings.json";   // a JSON settings file that contains all settings
String logFileName      = "/output.log";      // a text file for logs
int loopCounter         =1;                  // a loop counter for sampling current
int samplesPerMsg       =20;                 // how many samples before we send an IoT message there is a half second delay, so for a 1 minute sample set to 120
float ampAverage        =0.0;                  // used to hold the weighted average of the samples
float AAdebug           =0.0;                 // used to debug the ampAverage on a smaller power source
double Irms;                                  // will hold the current Amps as read from the CT Sensor
boolean debug           = true;

// Prototyping for Arduino IDE
void sendMessage(String keyName, String keyValue);
void connectToIoTHub();
void connectToWiFi();
String createDualForm (String target, String parmName1, String fieldOne, String parmName2, String fieldTwo, String buttonName, int inputLength);
void handleRoot();
void calibrate(String calibrateTo);

void log(const String data) {
  if (debug) {
    Serial.println(data);
  } else {
    sendMessage("Logging", data);
  }
  File logFile = SPIFFS.open(logFileName, "r");
  if (logFile.size() > (500*1024)) {
    // overwriting the log file
    File logFile = SPIFFS.open(logFileName, "w");
    logFile.println(data);
    logFile.close();
  } else {
    File logFile = SPIFFS.open(logFileName, "a");
    logFile.println(data);
    logFile.close();
  }
}

String getTime() {
  http.begin("http://no-ip.azurewebsites.net/time.php"); //HTTP
  int httpCode = http.GET();
  String mytime = ".";

  if (httpCode == HTTP_CODE_OK) {
    mytime = http.getString();
  }
  else {
    mytime = "Error getting date.";
  }
  return mytime;
}

boolean is_Authenticated() {
  if (server.hasHeader("Cookie")){   
    String cookie = server.header("Cookie");
    if (cookie.indexOf("LoginId=" + web_UserName) != -1) {
      return true;
      log("User Login=" + web_UserName);
    }
    else {
      return false;
    }
  }
  return false;
}

void authenticate() {
  String content = "<tr><td align=\"center\" colspan=\"2\">This site uses browser cookies.<br></td></tr><tr><td>" + createDualForm("/authenticate", "USERNAME", "User Name:", "PASSWORD", "Password:", "Login", 10) + "</td></tr>";
  server.send ( 200, "text/html", Header + "<br><br><br><br><br><br>" + content + Footer);
}

void handleLogin () {
    if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")){
      if (server.arg("USERNAME") == web_UserName &&  server.arg("PASSWORD") == web_Password ){
        String header = "HTTP/1.1 301 OK\r\nSet-Cookie: LoginId=" + web_UserName + "\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
        server.sendContent(header);
        return;
      } else {
        log("Failed Login.  Userid:" + server.arg("USERNAME") + "Password" + server.arg("PASSWORD"));
        authenticate();
      }
    } else {
      authenticate();
    }
    return;
}

void getSettings() {
  DynamicJsonBuffer jsonBuffer;
  File settingsFile = SPIFFS.open(settingsFileName, "r");
  if (settingsFile) {
    
    JsonObject& settings = jsonBuffer.parseObject(settingsFile.readString());
    if (settings.success()) {
      readSettings = true;
      
      String json_ssid            = settings["wireless_ssid"];
      String json_password        = settings["wireless_password"];
      String json_deviceName      = settings["deviceName"];
      String json_devicePassword  = settings["devicePassword"];
      float json_calibration      = settings["calibration"];
      String json_web_UserName    = settings["web_UserName"];
      String json_web_Password    = settings["web_Password"];
      String json_serverName      = settings["mqtt_server"];
      String json_customHeader    = settings["customHeader"];
      String json_customFooter    = settings["customFooter"];


      /*ssid = json_ssid;
      password = json_password;
      deviceName = json_deviceName;
      devicePassword = json_devicePassword;
      calibration = json_calibration;
      web_UserName = json_web_UserName;
      serverName = json_serverName;*/
      
      if (json_customHeader.length() > 1) {
        Header = json_customHeader;
      }

      if (json_customFooter.length() > 1) {
        Footer = json_customFooter;
      }
    }
  }
  settingsFile.close();
}

void saveSettings() {
    String strSettings;
    DynamicJsonBuffer jsonBuffer;
    JsonObject& settings = jsonBuffer.createObject();
    
    settings["wireless_ssid"]     = ssid;
    settings["wireless_password"] = password;
    settings["deviceName"]        = deviceName;
    settings["devicePassword"]    = devicePassword;
    settings["calibration"]       = calibration;
    settings["web_UserName"]      = web_UserName;
    settings["web_Password"]      = web_Password;
    settings["mqtt_server"]       = serverName;
    settings["lastSaved"]         = getTime();
    settings["customHeader"]      = Header;
    settings["customFooter"]      = Footer;
    settings.prettyPrintTo(strSettings);

    File settingsFile = SPIFFS.open(settingsFileName, "w");
    settingsFile.println(strSettings);
    settingsFile.close();
}

String getVariable (String variable) {
   if (variable == "ssid") {
      return ssid;
   } else if (variable == "password") {
      return password;
   } else if (variable == "deviceName") {
      return deviceName;
   } else if (variable == "devicePassword") {
      return devicePassword;
   } else if (variable == "web_UserName") {
      return web_UserName;
   } else if (variable == "web_Password") {
      return web_Password;
   } else if (variable == "serverName") {
      return serverName;
   } else if (variable == "Header") {
      return Header;
   } else if (variable == "Footer") {
      return Footer;
   } else {
      return "";
   }
}

void setVariable (String variable, String value) {
  log("Setting change.  " + variable + "=" + value);
   if (variable == "ssid") {
      ssid = value;
   } else if (variable == "password") {
      password = value;
   } else if (variable == "deviceName") {
      deviceName = value;
   } else if (variable == "devicePassword") {
      devicePassword = value;
   } else if (variable == "web_UserName") {
      web_UserName = value;
   } else if (variable == "web_Password") {
      web_Password = value;
   } else if (variable == "serverName") {
      serverName = value;
   } else if (variable == "Header") {
      Header = value;
   } else if (variable == "Footer") {
      Footer = value;
   }      
}

String createSimpleForm (String target, String parmName, String buttonName, int inputLength) {
  String  myform = "<form method=\"post\" action=\"" + target + "\">";
          myform += "<input type=\"text\" size=\"";
          myform += inputLength;
          myform += "\" name=\"" + parmName + "\" value=\"" + getVariable(parmName) + "\">";
          myform += "<input type=\"submit\" value=\"" + buttonName + "\"></form>";
  return myform;
}

String createSimpleFormButton (String target, String parmName, String parmValue, String buttonName) {
  String  myform = "<form method=\"post\" action=\"" + target + "\">";
          myform += "<input type=\"hidden\" name=\"" + parmName + "\" value=\"" + parmValue + "\">";
          myform += "<input type=\"submit\" value=\"" + buttonName + "\"></form>";
  return myform;
}

String createTextAreaForm (String target, String parmName, String buttonName, int rows, int cols ) {
  String  myform = "<form method=\"post\" action=\"" + target + "\">";
          myform += "<textarea rows=\"";
          myform += rows;
          myform += "\" cols=\"";
          myform += cols;
          myform += "\" name=\"" + parmName + "\">" + getVariable(parmName) + "</textarea>";
          myform += "<input type=\"submit\" value=\"" + buttonName + "\"></form>";
  return myform;
}

String createDualForm (String target, String parmName1, String fieldOne, String parmName2, String fieldTwo, String buttonName, int inputLength) {
  String  myform = "<form method=\"post\" action=\"" + target + "\">";
          myform += "<tr><td>" + fieldOne + "<br>" + fieldTwo +"</td><td><input type=\"text\" size=\"";
          myform += inputLength;
          myform += "\" name=\"" + parmName1 + "\" value=\"" + getVariable(parmName1) + "\"><br>";
          myform += "<input type=\"password\" size=\"";
          myform += inputLength;
          myform += "\" name=\"" + parmName2 + "\" value=\"" + getVariable(parmName2) + "\">";
          myform += "<input type=\"submit\" value=\"" + buttonName + "\"></td></tr></form>";
  return myform;
}

void handleRoot() {
  long days=0;
  long hours=0;
  long mins=0;
  long secs=0;
  secs = millis() /1000; //convect milliseconds to seconds
  mins=secs/60; //convert seconds to minutes
  hours=mins/60; //convert minutes to hours
  days=hours/24; //convert hours to days
  secs=secs-(mins*60); //subtract the coverted seconds to minutes in order to display 59 secs max 
  mins=mins-(hours*60); //subtract the coverted minutes to hours in order to display 59 minutes max
  hours=hours-(days*24); //subtract the coverted hours to days in order to display 23 hours max

   String One   = "<tr><td colspan=2><h1>Status: ";
          One   += deviceName;
          One   += "</h1></td></tr>";
   String Two   = "<tr><td>Version:</td><td>";
          Two   += sketchVersion;
          Two   += "<tr><td>Current Time:</td><td>" + getTime() + "</td></tr>";
          Two   += "<tr><td>Uptime:</td><td>";
          Two   += days;
          Two   += " days, ";
          Two   += hours;
          Two   += " hours, ";
          Two   += mins;
          Two   += " minutes</td></tr>";
          Two   += "</td></tr><tr><td>Current Amperage:</td><td><a href=\"./getAmperage\">";
          if (ampAverage < minimumReading) {
            Two   += 0.00;
          } else {
            Two   += ampAverage;
          }
          Two   += "</a></td></tr><tr><td>Current Calibration:</td><td>";
          Two   += calibration;
          Two   += "</td></tr><tr><td>Connected to Azure IoT:</td><td>";
          if (client.connected()) {
            Two   += "True</td></tr>";
          } else {
            Two   += "False</td></tr>";
          }
   String Three = "<tr><td></td><td></td></tr><tr><td></td><td></td></tr><tr><td>";
          Three += "<button type=\"button\" onclick=\"location.href='./calibrate'\">Calibrate Amperage</button></td><td><button type=\"button\" onclick=\"location.href='./config'\">Configure Settngs</button></td></tr>";

  server.send ( 200, "text/html", Header + One + Two + Three + Footer);
}

void handleCalibrate () {
  if (!is_Authenticated()) {
    authenticate();
    return;
  }
  String myCalibrate = "<tr><td>" + createSimpleForm("calibrateTo", "calibrateTo", "Calibrate Amperage", 10) + "</td></tr>";
  
  server.send ( 200, "text/html", Header + myCalibrate + Footer);
}

void handleCalibrateTo () {
  if (!is_Authenticated()) {
    authenticate();
    return;
  }
  String localHeader  = Header;
  String myCalibrate = "<tr><td>Calibrating to:</td><td>";
      myCalibrate   += server.arg("calibrateTo");
      myCalibrate   += " Amps</td></tr><tr><td></td><td></td></tr>";

  localHeader.replace("<meta http-equiv='refresh' content='30; url=/'>", "<meta http-equiv='refresh' content='15; url=/showLog'> ");
  server.send ( 200, "text/html", localHeader + myCalibrate + Footer);
  
  calibrate(server.arg("calibrateTo"));
}

void handleAmperage() {
    String  strAmperage = "{\"amperage\":";
            if (ampAverage >= minimumReading) {
              strAmperage += ampAverage;  
            } else {
              strAmperage += 0.00;
            }
            strAmperage += ", \"deviceName\": \"";
            strAmperage += deviceName;
            strAmperage += "\"}";
    server.send ( 200, "application/json", strAmperage);
}

void handleShowLog() {
    if (!is_Authenticated()) {
      authenticate();
      return;
    }

    log("Logfile viewed by:" + server.header("Cookie"));
    
    String localHeader = Header;
    localHeader.replace("<meta http-equiv='refresh' content='30; url=/'>", "<meta http-equiv='refresh' content='15; url=/showLog'> ");
    String myLog = "<tr><td>";

    File logFile = SPIFFS.open(logFileName, "r");
    if (logFile) {
      myLog += logFile.readString();
      myLog.replace("\r", "<br>");
    }
    myLog += "</td><tr><tr><td align=\"center\"><button type=\"button\" onclick=\"location.href='./'\">Return to Status</button></td></tr>";
    
    server.send ( 200, "text/html", localHeader + myLog + Footer);
}

void handleGetConfig() {
  if (!is_Authenticated()) {
    authenticate();
    return;
  }

  log("Configuration downloaded by:" + server.header("Cookie"));
  
  File settingsFile = SPIFFS.open(settingsFileName, "r");
  if (settingsFile) {
    server.send ( 200, "application/json", settingsFile.readString());
    settingsFile.close();
  }
}

void handleGetLog() {
  if (!is_Authenticated()) {
    authenticate();
    return;
  }

  log("Logfile downloaded by:" + server.header("Cookie"));
  
  File logFile = SPIFFS.open(logFileName, "r");
  if (logFile) {
    server.send ( 200, "text/plain", logFile.readString());
    logFile.close();
  }
}

void handleUpdate() {
  String localHeader = Header;
  String localFooter = "<br><br>Will return back to status in 30 seconds." + Footer; 
  if (!is_Authenticated()) {
    authenticate();
    return;
  }
  
  String myUpdate = "Updates:<br><br>";
  bool restart_WiFi = false;
  
  for ( uint8_t i = 0; i < server.args(); i++ ) {
    setVariable(server.argName(i), server.arg(i));
    myUpdate += server.argName(i) + "=" + server.arg(i) + "<br>";
  
    if (server.argName(i) == "ssid" || server.argName(i) == "password") {
      localHeader.replace("<meta http-equiv='refresh' content='30'", "<meta http-equiv='refresh' content='60; url=/'");
      localFooter = "<br><br>Will return back to status in 60 seconds." + Footer; 
      restart_WiFi = true;
    }
    
    if (server.argName(i) == "ota" && server.argName(i).startsWith("http://")) {
      myUpdate += "<br>WARNING: Performing Over the Air Update to ";
      myUpdate += server.arg(i);
      myUpdate += "<br><br>If the device does not return after 60 seconds, power cycle the device.";
      localHeader.replace("<meta http-equiv='refresh' content='30'", "<meta http-equiv='refresh' content='60; url=/'");
      localFooter = "<br><br>Will return back to status in 60 seconds.  If your IP Address has changed or DNS resolves to the wrong IP, your connection may fail." + Footer; 

      server.send ( 200, "text/html", Header + myUpdate + Footer);

      delay (2000);

      log("OTA Updated to:" + server.arg(i));
      t_httpUpdate_return OTAStatus = OTA.update(server.arg(i).c_str());
      return;
    } else if (server.argName(i) == "ota") {
      localHeader.replace("<meta http-equiv='refresh' content='30'", "<meta http-equiv='refresh' content='30; url=/config'");
      myUpdate += "Error: Over the Air Update URL must start with http:// and is case sensitive.";
    }
    if (server.argName(i) == "connectMQTT") {
      myUpdate += "WARNING: If a valid IoT Hub, deviceName and devicePassword exist, the device will auto reconnect.";
      if (server.arg(i) == "true"){
        connectToIoTHub();  
      } else {
        log("IoT Hub disconnected by:"+ server.header("Cookie"));
        client.disconnect();  
      }
      localHeader.replace("<meta http-equiv='refresh' content='30'", "<meta http-equiv='refresh' content='30; url=/config'");
    }
  }
  
  saveSettings();
  getSettings();

  if (restart_WiFi) {
    WiFi.disconnect();
    SPIFFS.end();
    server.stop();
    setup();
  }

  server.send ( 200, "text/html", localHeader + myUpdate + localFooter);
}

void handleConfig() {
  if (!is_Authenticated()) {
    authenticate();
    return;
  }
   String One   = "<tr><td colspan=2 align=\"center\"><h1>Configure: ";
          One   += deviceName;
          One   += "</h1></td></tr>";
   String Two   = createDualForm("update", "ssid", "Wireless SSID:", "password", "Wireless Password:", "Update", 15);
          Two   += "<tr><td></td><td></td></tr><tr><td></td><td></td></tr>";
          Two   += createDualForm("update", "web_UserName", "Web UserName:", "web_Password", "Web Password:", "Update", 15);
          Two   += "<tr><td></td><td></td></tr><tr><td></td><td></td></tr>";
          Two   += "<tr><td>Firmware Update URL:</td><td>";
          Two   += createSimpleForm("update", "ota", "Update", 50);
          Two   += "</td></tr><tr><td></td><td></td></tr>";
   String TwoT  = "<tr><td>HTML Header:</td><td>";
          TwoT  += createTextAreaForm("update", "Header", "Update", 5, 50);          
          TwoT  += "</td></tr>";
          TwoT  += "<tr><td>HTML Footer:</td><td>";
          TwoT  += createTextAreaForm("update", "Footer", "Update", 5, 50);               
          TwoT  += "</td></tr><tr><td></td><td></td></tr><tr><td></td><td></td></tr>";
   String Three  = "<tr><td></td><td></td></tr><tr><td>Azure IoT Hub:</td><td>"; 
          Three += createSimpleForm("update", "serverName", "Update", 50);
          Three += "</td></tr><tr><td>Azure Device Name:</td><td>";
          Three += createSimpleForm("update", "deviceName", "Update", 50);
          Three += "</td></tr><tr><td>Azure SAS:</td><td>";
          Three += createSimpleForm("update", "devicePassword", "Update", 50);
          Three += "</td></tr><td></td><td><small><i>SharedAccessSignature sr=<b>hubname.azure-devices.net</b>%2fdevices%2f<b>deviceName</b>&sig=mWkKeycq8%3d&se=1517419834</i></small>";
          Three += "</td></tr><tr><td>Connected to IoT:</td><td>";
          if (client.connected()) {
            Three   += createSimpleFormButton ("update", "connectMQTT", "false", "Disconnect") + "</td></tr>";
          } else {
            Three   += createSimpleFormButton ("update", "connectMQTT", "true", "Connect") + "</td></tr>";
          }
          
   String Four = "<tr><td></td><td></td></tr><tr><td></td><td></td></tr><tr><td colspan=\"2\" align=\"center\"><button type=\"button\" onclick=\"location.href='./getSettings'\">Download Configuration</button>";
          Four += "<button type=\"button\" onclick=\"location.href='./getLog'\">Download Logfile</button>";
          Four += "<button type=\"button\" onclick=\"location.href='./'\">Return to Status</button><br>";
          Four += "<button type=\"button\" onclick=\"location.href='./config'\">Basic Settings</button>";
          Four += "<button type=\"button\" onclick=\"location.href='./config?ShowIoT=true'\">IoT Settings</button>";
          Four += "<button type=\"button\" onclick=\"location.href='./config?EditHTML=true'\">HTML Settings</button></td></tr>";

          if (server.arg("EditHTML") == "true") {
            server.send ( 200, "text/html", Header + One + TwoT + Four + Footer);
          } else if (server.arg("ShowIoT") == "true") {
            server.send ( 200, "text/html", Header + One + Three + Four + Footer);
          } else {
            server.send ( 200, "text/html", Header + One + Two + Four + Footer);
          }

}

void connectToIoTHub() {
  int retry = 0;
  while (!client.connected() && retry < 5 && deviceName.length() > 0 && deviceName.length() > 0) {
    log("Connecting to IoT Hub. IP: ");
//    log(WiFi.localIP());
    String userName = serverName;
    userName += "/";
    userName += deviceName;
    client.connect(deviceName.c_str(), userName.c_str(), deviceSAS.c_str());
    log(deviceName.c_str());
    log(userName.c_str());
    log(deviceSAS.c_str());
    if (client.connected()) {
      log("Connection to IoTHub Success");
      String subscribestring = "devices/";
      subscribestring += deviceName;
      subscribestring += "/messages/devicebound/#";
      client.subscribe(subscribestring.c_str());
    }
    else {
      log("Failed to connect to IoTHub");
    }
    retry++;
    server.handleClient();
  }
}

void connectToWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    if(client.connected()){
      client.disconnect();
    }
    WiFi.stopSmartConfig();
    WiFi.hostname(deviceName);
    WiFi.enableAP(false);
    log("Connecting to WiFi");
    WiFi.begin(ssid.c_str(), password.c_str());
    
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      if (debug) {
        Serial.print(".");
      }
    }  
  Serial.println(WiFi.localIP());
  }
}

void verifyInterent() {
  http.begin("http://www.msftncsi.com/ncsi.txt"); //HTTP
  int httpCode = http.GET();
  
  if (httpCode != HTTP_CODE_OK) {
    WiFi.disconnect();
    connectToWiFi();
  }
  http.end();
}

void sendMessage(String keyName, String keyValue) {
    if (deviceName.length() > 0 && devicePassword.length() >0) {
      verifyInterent();
      connectToIoTHub();
  
      String json = "{ ";
      json += keyName;
      json += ": \"";
      json += keyValue;
      json += "\" }";
    
      String publishstring = "devices/";
      publishstring += deviceName;
      publishstring += "/messages/events/";
  
      client.publish(publishstring.c_str(), json.c_str());
    }
}

void sendMessage(String keyName, float keyValue) {
    if (deviceName.length() > 0 && devicePassword.length() >0) {
      verifyInterent();
      connectToIoTHub();
      
      String json = "{ ";
      json += keyName;
      json += ": ";
      json += keyValue;
      json += " }";
    
      String publishstring = "devices/";
      publishstring += deviceName;
      publishstring += "/messages/events/";
  
      client.publish(publishstring.c_str(), json.c_str());
    }
}

void calibrate(String calibrateTo) {
  float target = calibrateTo.toFloat();
  int mycounter = 1;
  float currentcalibration = calibration;                                 // where we start from
  double Irms;
  log("Calibrate");
  bool complete = false;
  
  while (complete == false) {
    emon1.current(17, currentcalibration);                                   // change the currentcalibration
    for (int i=0; i <= 6; i++){                                               // We have to read the sensor at least 4 times to get an accurate reading
      Irms = emon1.calcIrms(1480);
      log(String(Irms,5));
      delay(1000);
      server.handleClient();
    } 
    
    String logMessage = "count ";
    logMessage += mycounter;
    logMessage += " target is ";
    logMessage += target;
    logMessage += " reading was ";
    logMessage += Irms;
    log(logMessage);

    if (Irms < minimumReading) {
      log("Error: Not enough amperage to calculate.");
      break;
    } else if (Irms >= target - calibrationmtc && Irms <= target + calibrationmtc) {
      String message = "calibration Success: Target=";
      message += calibrateTo;
      message += " currentcalibration=";
      message += currentcalibration;
      message += " sensorReading=";
      message += Irms;
      sendMessage("calibrationStatus", message);
      log(message);
      complete = true;
    } else if (Irms < target) {
      currentcalibration = currentcalibration + calibrationchg;
      logMessage = "Adding to currentcalibration.  Value is now:";
      logMessage += currentcalibration;
      log(logMessage);
    } else {
      currentcalibration = currentcalibration - calibrationchg;
      logMessage = "Subtracting from currentcalibration.  Value is now:";
      logMessage += currentcalibration;
      log(logMessage);
    }

    mycounter++;
    saveSettings();
    server.handleClient();
  }
  calibration = currentcalibration;
  
  saveSettings();
  
  String message = "Wrote value ";
          message += currentcalibration;
          message += " to settings file ";
          message += settingsFileName;
  log(message);
  sendMessage("Calibration Complete", message);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message+=(char)payload[i];
  }

  log("recievedMessage:" + message);

  if (message.startsWith("Calibrate:")) {
    calibrate(message.substring(11));
  } else if (message == "getCalibration") {
    sendMessage("currentCalibration", calibration); 
  } else if (message == "getAmps") {
    sendMessage("currentAmps", Irms);
  } else if (message.startsWith("OTA:")){
    // We expect something like "OTA: http://www.server.com/mypath/mystuff/binary.bin"

    String fullURL = message.substring(message.indexOf("://") -4);;
    log("OTA updating from: " + fullURL);
    t_httpUpdate_return OTAStatus = OTA.update(fullURL.c_str());
    switch(OTAStatus) {
    case HTTP_UPDATE_FAILED:
      log("HTTP_UPDATE_FAILD Error");
      break;

    case HTTP_UPDATE_NO_UPDATES:
      log("HTTP_UPDATE_NO_UPDATES");
      break;

    case HTTP_UPDATE_OK:
      log("HTTP_UPDATE_OK");
      break;
    } 
  }
}

void setup()
{  
  Serial.begin(115200);
  SPIFFS.begin();

  getSettings();

  connectToWiFi();

  client.setServer(serverName.c_str(), 8883);
  client.setCallback(callback);
  
  connectToIoTHub();
  
  emon1.current(17, calibration);             // for NodeMCU, we only have one ADC
  
  sendMessage("booting", sketchVersion);

  sendMessage("calibrateValue", calibration);

  server.on ( "/", handleRoot );
  server.on ( "/getAmperage", handleAmperage );
  server.on ( "/config", handleConfig );
  server.on ( "/update", handleUpdate );
  server.on ( "/getSettings", handleGetConfig );
  server.on ( "/getLog", handleGetLog );
  server.on ( "/authenticate", handleLogin );
  server.on ( "/calibrate", handleCalibrate );
  server.on ( "/calibrateTo", handleCalibrateTo );
  server.on ( "/showLog", handleShowLog );

  const char * headerkeys[] = {"User-Agent","Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
  server.collectHeaders(headerkeys, headerkeyssize );
  
  server.begin();
}

void loop()
{
  connectToWiFi();
  connectToIoTHub();
//  log("client.loop start");
  client.loop();
  
  if (Serial.available() > 0) {
    String calibrateTo = Serial.readString();
    calibrate(calibrateTo);
  }

  Irms = emon1.calcIrms(1480);  // Calculate Irms only
//  log("calcIrms");
  log("loop counter pre");
  log(String(loopCounter,5));
  ampAverage = ((ampAverage * loopCounter) + Irms) / ( loopCounter + 1);
  log("loop counter post");
  log(String(loopCounter,5));
//  log("Irms:");
//  log(String(Irms,10));
  if (loopCounter >= samplesPerMsg) {
//    if (Irms > minimumReading) {                 // less than 2 Amps we send 0 for averaging
//      sendMessage("amps", ampAverage);
//      log("************************");
//      log("ampAverage");
//      log(String(ampAverage,5));
//    } else {
//      sendMessage("amps", 0.00);
//      log("************************");
//      log("0.00");
//    }
    AAdebug=1000000*ampAverage;
    sendMessage("amps", AAdebug);
    log("************************");
//    log("ampAverage");
//    log(String(AAdebug,5));
    loopCounter = 0;
    ampAverage = 0;
  }
  
  delay(500);

  loopCounter ++;
  server.handleClient();
}


