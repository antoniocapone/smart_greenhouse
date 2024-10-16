#include "secrets.h"

#define TOPIC_DEBUG_PRINT

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

// sensori
#define DTYPE DHT11 // richiesto dalla libreria
#define DPIN D4
DHT dht(DPIN, DTYPE);
#define TEMPERATURE_TRESHOLD  27.0f
#define TEMP_PIN_OUT D8

// comunicazione con arduino
#include "src/SoftwareSerial.h"
#define SERIAL_PORT_TX D3
#define SERIAL_PORT_RX D5
EspSoftwareSerial::UART espArduinoSerial;

uint8_t fan = 0;

typedef struct {
  uint8_t hum1;
  uint8_t hum2;
  uint8_t hum3;
  bool    pump1;
  bool    pump2;
  bool    pump3;
} pump_handle_t;

pump_handle_t pump_handle = {50, 50, 50, false, false, false};

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

#ifdef TOPIC_DEBUG_PRINT
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if the first character is present
  if ((char)payload[0] != NULL) {
    digitalWrite(LED_BUILTIN, LOW); // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
  } else {
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
  }
#endif

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
  Serial.printf("Setup started\n");

  setup_wifi();

  espClient.setInsecure();
  client = new PubSubClient(espClient);

  client->setServer(mqtt_server, 8883);
  client->setCallback(callback);

  dht.begin();

  espArduinoSerial.begin(9600, SWSERIAL_8N1, SERIAL_PORT_RX, SERIAL_PORT_TX, false);
  if (!espArduinoSerial) { // If the object did not initialize, then its configuration is invalid
    Serial.println("Invalid EspSoftwareSerial pin configuration, check config"); 
    while (1) { // Don't continue with invalid configuration
      Serial.println("Restart!");
      delay (1000);
    }
  }
  espArduinoSerial.setTimeout(3000);
  pinMode(TEMP_PIN_OUT, OUTPUT);
  espArduinoSerial.flush();
  Serial.println("SoftwareSerial started successfully");
}

uint8_t count = 0;

void loop() {
  if (!client->connected()) {
    reconnect();
  }
  client->loop();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (client->publish("/topic/temperature", std::to_string(temperature).c_str())) {
	  Serial.printf("Mando temperatura: %s\n", std::to_string(temperature).c_str());
  }
  if (client->publish("/topic/humidity", std::to_string(humidity).c_str())) {
	  Serial.printf("Mando umidità: %s\n", std::to_string(humidity).c_str());
  }
  if (client->publish("/topic/temperature_treshold", std::to_string(fan).c_str())) {
	  Serial.printf("Mando treshold superato: %s\n", std::to_string(fan).c_str());
  }

  if (temperature > TEMPERATURE_TRESHOLD) {
    fan = 1;
    digitalWrite(TEMP_PIN_OUT, HIGH);
  } else {
    fan = 0;
    digitalWrite(TEMP_PIN_OUT, LOW);
  }

  espArduinoSerial.write((uint8_t*)&fan, sizeof(fan));
  delay(50);

  count = (count + 1) % 10;
  if (count == 9) espArduinoSerial.flush();

  if (espArduinoSerial.available() > 0) {
    size_t ret = espArduinoSerial.readBytes((uint8_t *)&pump_handle, sizeof(pump_handle));
    Serial.printf("Ricevuti %d bytes da Arduino Uno\n", ret);
    if (ret == sizeof(pump_handle)) {
        if (client->publish("/topic/hum1", std::to_string(pump_handle.hum1).c_str())) {
          Serial.printf("Mando hum1: %s\n", std::to_string(pump_handle.hum1).c_str());
        }
        if (client->publish("/topic/hum2", std::to_string(pump_handle.hum2).c_str())) {
          Serial.printf("Mando hum2: %s\n", std::to_string(pump_handle.hum2).c_str());
        }
        if (client->publish("/topic/hum3", std::to_string(pump_handle.hum3).c_str())) {
          Serial.printf("Mando hum3: %s\n", std::to_string(pump_handle.hum3).c_str());
        }
        if (client->publish("/topic/pump1", std::to_string(pump_handle.pump1).c_str())) {
          Serial.printf("Mando pump1: %s\n", std::to_string(pump_handle.pump1).c_str());
        }
        if (client->publish("/topic/pump2", std::to_string(pump_handle.pump2).c_str())) {
          Serial.printf("Mando pump2: %s\n", std::to_string(pump_handle.pump2).c_str());
        }
        if (client->publish("/topic/pump3", std::to_string(pump_handle.pump3).c_str())) {
          Serial.printf("Mando pump3: %s\n", std::to_string(pump_handle.pump3).c_str());
        }
    }
  }

  delay(571);

}