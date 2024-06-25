#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


const char* ap_ssid = "ESP8266_AP";
const char* ap_password = "12345678";

// Set web server port number to 80
ESP8266WebServer server(80);

#define LED 2 
//-----------------------------------------FUNCTIONS DEFINE-----------------------------------
void configWiFi();
void clientWiFi();
void config();

//-----------------------------------------FUNCTIONS------------------------------------------
void configWiFi()
{
 // Start the access point
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("Access Point started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Define the handler for the root URL
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", "<form action='/connect' method='POST'>SSID: <input type='text' name='ssid'><br>Password: <input type='text' name='password'><br><input type='submit' value='Connect'></form>");
  });

  // Define the handler for the connect URL
  server.on("/connect", HTTP_POST, []() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");

    String response = "Received SSID: " + ssid + "<br>Received Password: " + password;
    server.send(200, "text/html", response);

    // Connect to the target Wi-Fi network
    WiFi.begin(ssid.c_str(), password.c_str());

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      Serial.print(".");
      attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected to Wi-Fi");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("Failed to connect to Wi-Fi");
    }
  });

  // Define the handler to check connection status
  server.on("/status", HTTP_GET, []() {
    String status;
    if (WiFi.status() == WL_CONNECTED) {
      status = "Connected to Wi-Fi. IP: " + WiFi.localIP().toString();
    } else {
      status = "Failed to connect to Wi-Fi.";
    }
    server.send(200, "text/plain", status);
  });

  // Start the server
  server.begin();
}

//--------------------------------------------CONFIGURATION------------------------------
void config()
{
  Serial.begin(9600); 
  pinMode(LED,OUTPUT); 
}
//---------------------------------------------MAIN-------------------------------------
void setup() {

config();
configWiFi(); // connecting to wifi 

}

void loop() {
server.handleClient();

}
