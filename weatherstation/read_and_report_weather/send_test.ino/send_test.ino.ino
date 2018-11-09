#include "DHT.h"

#define DHTPIN D7     // what digital pin we're connected to

//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// DHT22 
// 1. 3.3 V     2. Data to digital input    3. GND     4. Not in use pr. now

#include <ESP8266WiFi.h>


const char* ssid     = "Zhone403406";
const char* password = "FdRvWj9xPF";
const char* host = "192.168.20.20";

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void connect_to_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
 
  const int httpPort = 10000;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }


  
}

void sop_send() {
    // Try sending stuff
  client.print("Temp");
  client.print("2");
  client.print("End");
}

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  delay(500);
  // connect_to_wifi();
  // dht.begin();
}

void loop() {
  
  connect_to_wifi();
  delay(500);
  sop_send();
  String line = client.readStringUntil('\n');
  Serial.println(line);
  delay(10000);

  client.stop();
  Serial.println("\n[Disconnected]");


  //read_temp_and_humid();
}
