#include "DHT.h"
#include "stdio.h"
#define DHTPIN D7     // what digital pin we're connected to

#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// DHT22 
// 1. 3.3 V     2. Data to digital input    3. GND     4. Not in use pr. now

#include <ESP8266WiFi.h>


const char* ssid = "Zhone403406";
const char* password = "FdRvWj9xPF";
const char* host = "192.168.20.6";
const int httpPort = 9999;

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

}

void connect_to_server(){
  
  Serial.println("Trying to connect to server");
  Serial.println(host);
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }  
}

void connect_to_socket() {
    // Use WiFiClient class to create TCP connections
 

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
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

  format_data(humidity, temperature, heat_index);

}


void sop_send(char *data_series) {
//void sop_send() {
    // Try sending stuff
  // which sensor am i 

  client.print("S1");
  delay(200);
  //client.print("temperature: 2, humidity: 10, status: 0");
  
  client.print(data_series);
  // Make it right
  
}

void format_data(float humidity, float temperature, float heat_index) {
    char hum[40];
    char temp[40];
    char heat_i[40];
    char stat[10] = "Status: 0"; 
    char all_data[100];
    
    float fingers = 5.5;
    float toes = 2.2;
    int fin = 1;
    sprintf(temp, "temperature: %2.1f, ", temperature);
    sprintf(hum, "humidity: %2.1f ,", humidity);
    sprintf(heat_i, "heat_index: %2.1f ,", heat_index);
    
    all_data[0] = '\0';
    strcat(all_data, temp);
    strcat(all_data, hum);
    strcat(all_data, heat_i);
    strcat(all_data, stat);
    Serial.println(all_data);
    //char *data_pointer = all_data;
    sop_send(all_data);
    //sop_send();
} 


void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  delay(500);
  //2connect_to_wifi();
}

void loop() {

  if (WiFi.status() != WL_CONNECTED)
    connect_to_wifi();

  connect_to_server();
  
  delay(500);
  read_temp_and_humid();

  String line = client.readStringUntil('\n');
  Serial.println("Recieved from server");
  Serial.println(line);
  delay(1000);

  if (line == 0){
    Serial.println("Amazing, it worked");
  }

  client.stop();
  Serial.println("\n[Disconnected]");
  delay(900000);
  // WORKS, must connect PORT D0 to RST
  // HOWEVER IF D0 is connected at boot to RST, it will not load script
  //Serial.println("Going into deep sleep for 20 seconds");
  //ESP.deepSleep(20e6); // e6 for microseconds
  //ESP.deepSleep(120e6); // 20e6 is 20 microseconds

 
}
