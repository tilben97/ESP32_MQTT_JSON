#include <Arduino.h>
#include "WiFi.h"
#include "ArduinoJson.h"
#include "PubSubClient.h"

const char* ssid = "selma-razvoj";
const char* pass = "selmasubotica";
const char* brokerUser = "tilben97biker@gmail.com";
const char* brokerPass = "0dc9d70b";
const char* broker = "mqtt.dioty.co";

WiFiClient espClient;
PubSubClient client(espClient);

void MQTT_Callback(char* topic, byte* payload, unsigned int length)
{
  StaticJsonDocument<256> doc;
  deserializeJson(doc, payload, length);
  char out_doc[100];
    if(doc["led"] == "on")
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  int v = doc["value"];
  serializeJson(doc, Serial);
  serializeJson(doc, out_doc);
  Serial.println("");
  Serial.println(v);
  client.publish("/tilben97biker@gmail.com/repeat", out_doc);
  
}
void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi.");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n Connected");
  client.setServer(broker, 1883);
  Serial.print("Connecting to MQTT.");
  while(!client.connected())
  {
    Serial.print(".");
    if(client.connect("ESP32", brokerUser, brokerPass))
    {
      Serial.println("\n Connected");
      client.subscribe("/tilben97biker@gmail.com/in");
    }
  }
  client.setCallback(MQTT_Callback);

}

void loop() 
{
  StaticJsonDocument<300> encoder;

  encoder["device"] = "ESP32";
  JsonArray values = encoder.createNestedArray("values");
  values.add(20);
  values.add(21);
  char Json_message[100];
  //encoder.printTo(Json_message, sizeof(Json_message));
  serializeJson(encoder, Json_message);
  //Serial.print("Sending next message:    ");
  //Serial.println(Json_message);
  if(client.publish("/tilben97biker@gmail.com/out", Json_message) == true )
  {
    //Serial.println("Message successfuly send.");
  }
  else
  {
    //Serial.println("Error! Message does not send");
  }
  client.loop();
  Serial.println("----------------------------------------------------");
  delay(1000);
}