#include <ArduinoJson.h>
#include "EspMQTTClient.h"
#include "DHT.h"
//Configuração MQTT
EspMQTTClient client(
  "nobody 3.0", //nome da sua rede Wi-Fi
  "milokzin", //senha da sua rede Wi-Fi
  "mqtt.tago.io",  // MQTT Broker server ip padrão da tago
  "Default",   // username
  "a057f7fe-aeb1-4b01-81fe-c86d4495c741",   // Código do Token
  "TestClient",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);
// Configuração DHT
#define DHTPIN 13     // Pino conectado ao DHT
#define DHTTYPE DHT11   // Definindo o modelo do DHT
DHT dht(DHTPIN, DHTTYPE);
//Variáveis DHT e JSON
char temperature[100];
char humidity[100];
int dhtHumidity;
int  dhtTemperature;
void setup()
{
  Serial.begin(9600);
  dht.begin();
  Serial.println("Conectando WiFi");
  while(!client.isWifiConnected()){
    Serial.print('.');
    client.loop();
    delay(1000);
  }
  Serial.println("Conectado!");
  Serial.println("Conectando com o broker MQTT");
  while(!client.isMqttConnected()){
    Serial.print('.');
    client.loop();
    delay(1000);
  }
  Serial.println("Conectado!");
}
// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{}
void loop()
{
  dhtHumidity = dht.readHumidity();
  dhtTemperature = dht.readTemperature();
  StaticJsonDocument<300> temperatura;
  temperatura["variable"] = "Temperatura";
  temperatura["value"] = dhtTemperature;
  serializeJson(temperatura, temperature);
  StaticJsonDocument<300> umidade;
  umidade["variable"] = "Umidade";
  umidade["value"] = dhtHumidity;
  serializeJson(umidade, humidity);

//envio de dados
  client.publish("info/temperature", temperature); // You can activate the retain flag by setting the third parameter to true
  client.publish("info/humidity", humidity); // You can activate the retain flag by setting the third parameter to true
  delay(5000);
  client.loop();
}
