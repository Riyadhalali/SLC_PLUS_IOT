#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

void handleRoot() {
  String page = "<form action='/connect' method='POST'>"
                "SSID: <input type='text' name='ssid'><br>"
                "Password: <input type='text' name='password'><br>"
                "<input type='submit' value='Connect'></form>";
  server.send(200, "text/html", page);
}

void handleConnect() {
  String ssid = server.arg("ssid");
  String password = server.arg("password");

  WiFi.begin(ssid.c_str(), password.c_str());

  server.send(200, "text/html", "Connecting to WiFi...");
}

void handleStatus() {
  String status;
  if (WiFi.status() == WL_CONNECTED) {
    status = "Connected";
  } else {
    status = "Not connected";
  }
  server.send(200, "text/plain", status);
}

void setup() {
  Serial.begin(9600);

  // Set up as a soft access point
  WiFi.softAP("ESP8266_AP", "12345678");

  // Enable station mode to connect to router
  WiFi.mode(WIFI_AP_STA);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/connect", HTTP_POST, handleConnect);
  server.on("/status", HTTP_GET, handleStatus);

  server.begin();
  Serial.println("Server started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  server.handleClient();
}
