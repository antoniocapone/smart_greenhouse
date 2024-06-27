#include "secrets.h"

#include <DHT.h>	// DHT sensor library da Adafruit 1.4.6
#include <ESP8266WiFi.h>
#include <PubSubClient.h> // mqtt

// utility
#include <string>

// Update these with values suitable for your network.
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* mqtt_server = BROKER_ADDRESS;

WiFiClientSecure espClient;
PubSubClient * client;
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (500)
char msg[MSG_BUFFER_SIZE];
int value = 0;

// sensori
#define DTYPE DHT11 // richiesto dalla libreria
#define DPIN 4
DHT dht(DPIN, DTYPE);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
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
//   Serial.print("Message arrived [");
//   Serial.print(topic);
//   Serial.print("] ");
//   for (int i = 0; i < length; i++) {
//     Serial.print((char)payload[i]);
//   }
//   Serial.println();

//   // Switch on the LED if the first character is present
//   if ((char)payload[0] != NULL) {
//     digitalWrite(LED_BUILTIN, LOW); // Turn the LED on (Note that LOW is the voltage level
//     // but actually the LED is on; this is because
//     // it is active low on the ESP-01)
//     delay(500);
//     digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
//   } else {
//     digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
//   }
}


void reconnect() {
  // Loop until we’re reconnected
  while (!client->connected()) {
    Serial.print("Attempting MQTT connection…");
    String clientId = BOARD_NAME;
    // Attempt to connect
    // Insert your password
    if (client->connect(clientId.c_str(), USERNAME, PASSWORD)) {
      Serial.println("connected");
    //   // Once connected, publish an announcement…
    //   client->publish("testTopic", "hello world");
    //   // … and resubscribe
    //   client->subscribe("testTopic");
	// mettiamo qui tutte le sottoscrizioni
    } else {
      Serial.print("failed, rc = ");
      Serial.print(client->state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  delay(500);
  // When opening the Serial Monitor, select 9600 Baud
  Serial.begin(9600);
  delay(500);

  setup_wifi();

  espClient.setInsecure();
  client = new PubSubClient(espClient);

  client->setServer(mqtt_server, 8883);
  client->setCallback(callback);

  dht.begin();
}

int cont = 0;

void loop() {
  if (!client->connected()) {
    reconnect();
  }
  client->loop();

//   float temperature = dht.readTemperature();
  float temperature = 3.14 + cont++;
  float humidity = 80;
  if (client->publish("/topic/temperature", std::to_string(temperature).c_str())) {
	Serial.printf("Mando temperatura: %s\n", std::to_string(temperature).c_str());
  }
  if (client->publish("/topic/humidity", std::to_string(humidity).c_str())) {
	Serial.printf("Mando umidità: %s\n", std::to_string(humidity).c_str());
  }

  delay(5000);

}