#include <WiFi.h>

#include "Adafruit_MQTT.h"

#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       "sidhant kumar"

#define WLAN_PASS       "12345678"

#define AIO_SERVER      "io.adafruit.com"

#define AIO_SERVERPORT  1883                  

#define AIO_USERNAME    "sidhantkumar"
#define AIO_KEY         "aio_jjvT073jOTmRLPgNpCGwLJvGl2JX"


int output=2;

// Create an ESP8266 WiFiClient class to connect to the MQTT server.

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);        // Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.

Adafruit_MQTT_Subscribe Light_Control = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Light_Control");

void MQTT_connect();

void setup() {

  Serial.begin(115200);

  delay(10);

pinMode(2,OUTPUT);

  // Connect to WiFi access point.

  Serial.println(); Serial.println();

  Serial.print("Connecting to ");

  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

  }

  Serial.println();

  Serial.println("WiFi connected");

  Serial.println("IP address: "); Serial.println(WiFi.localIP()); 

  mqtt.subscribe(&Light_Control);

}

uint32_t x=0;

void loop() { 

  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;

  while ((subscription = mqtt.readSubscription(5000))) {

    if (subscription == &Light_Control) {

      Serial.print(F("Got: "));

      Serial.println((char *)Light_Control.lastread);

       if (!strcmp((char*) Light_Control.lastread, "ON"))

      {

        //Active low logic

        digitalWrite(2, HIGH);

      }

      else

      {

        digitalWrite(2, LOW);

      }

    }

  }

}

void MQTT_connect() {

  int8_t ret;

  // Stop if already connected.

  if (mqtt.connected()) {

    return;

  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected

       Serial.println(mqtt.connectErrorString(ret));

       Serial.println("Retrying MQTT connection in 5 seconds...");

       mqtt.disconnect();

       delay(5000);  // wait 5 seconds

       retries--;

       if (retries == 0) {

         // basically die and wait for WDT to reset me

         while (1);

       }

  }

  Serial.println("MQTT Connected!");

}
