#include "DHT.h"
#include "stdio.h"
#define DHTPIN D7     // what digital pin we're connected to

//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// DHT22 
// 1. 3.3 V     2. Data to digital input    3. GND     4. Not in use pr. now

#include <ESP8266WiFi.h>


const char* ssid     = "MESH";
const char* password = "internetplease";
const char* host = "192.168.62.120";

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;
const int httpPort = 10000;

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
}
void connect_to_server(){
  WiFiClient client;
  const int httpPort = 10000;
  Serial.println("Trying to connect to server");
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  client.println("S1");
  client.println("Send this data to server");
  
}

void serial_print_data(float humidity, float temperature, float heat_index) {
  Serial.print("Humidity: ");
  Serial.print(humidity);

  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C ");
  Serial.print("Heat index: ");
  Serial.print(heat_index);
  Serial.print(" *C ");
  Serial.println();
}

void read_temp_and_humid() {
      // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float heat_index = dht.computeHeatIndex(temperature, humidity, false);

  serial_print_data(humidity, temperature, heat_index);


}

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  connect_to_wifi();
  dht.begin();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED)
  connect_to_wifi();

  connect_to_server();

  client.connect(host, httpPort);
  read_temp_and_humid();
  delay(1000);
}
