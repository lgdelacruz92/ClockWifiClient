/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

#include <ESP8266WiFi.h>


#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "worldtimeapi.org";
const uint16_t port = 80;

void setup() {
  Serial.begin(115200);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    // Serial.println("connection failed");
    delay(5000);
    return;
  }

  // This will send a string to the server
  // Serial.println("sending data to server");
  if (client.connected()) {
    client.println("GET /api/timezone/America/New_York HTTP/1.1");
    client.println();
  }

  // wait for data to be available
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      // Serial.println(">>> Client Timeout !");
      Serial.println("failed");
      client.stop();
      delay(3000);
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  // Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  String result = "";
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    result += String(ch);
  }
  String searchString = "\"datetime\"";
  int ind1 = result.indexOf(searchString);
  String date = result.substring(ind1+searchString.length()+13, ind1+searchString.length()+13+5);
  Serial.println(date);

  // Close the connection
  client.stop();
  delay(3000);
}
