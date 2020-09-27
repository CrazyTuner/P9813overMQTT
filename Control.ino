
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#define    NUM_LEDS             1      // P9813 module: string is one l
#define    CLOCK_PIN            5 // Pin 1      // Clock and data pins for LED data
#define    DATA_PIN             4 // Pin 2
CRGB leds[NUM_LEDS];


const char* ssid = "WIFI SSID";
const char* password = "WIFI PASSWD";
char wiFiHostname[ ] = "WIFI HOSTNAME";
const char* mqtt_server = "MQTT SERVER";
const char* mqtt_user = "MQTT USER";
const char* mqtt_password = "MQTT PASSWD";

int Rot = 0;
int Blau = 0;
int Gruen = 0;



WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
   delay(100);
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.mode (WIFI_STA);
    WiFi.begin(ssid, password);
    wifi_station_set_hostname(wiFiHostname);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* payload, unsigned int length) {
  String PayloadString = "";
  for (int i = 0; i < length; i++) { PayloadString = PayloadString + (char)payload[i]; }

  Serial.println("New message arrived");  
  Serial.println(topic);  
  Serial.println(PayloadString);  

  if(strcmp(topic, "Bett/Rot") == 0) { 
    Rot = PayloadString.toInt();
    leds[0].setRGB( Rot, Gruen, Blau);
    FastLED.show();
  }  
  
  
 if(strcmp(topic, "Bett/Blau") == 0) { 
    Blau = PayloadString.toInt();
    leds[0].setRGB( Rot, Gruen, Blau);
    FastLED.show();
  }  
  

 if(strcmp(topic, "Bett/Gruen") == 0) { 
    Gruen = PayloadString.toInt();
    leds[0].setRGB( Rot, Gruen, Blau);
    FastLED.show();
  }  
  }
  
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("RGBLED1", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("Bett/Gruen");
      client.subscribe("Bett/Rot");
      client.subscribe("Bett/Blau");
      }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  LEDS.addLeds<P9813, DATA_PIN, CLOCK_PIN>(leds, NUM_LEDS);  // initialize LED control object
  FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN>(leds, NUM_LEDS);  // initialize LED control object
  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
