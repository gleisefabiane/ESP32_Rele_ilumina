# 1 "C:\\Users\\INSTRU~1\\AppData\\Local\\Temp\\tmp5sx25h0e"
#include <Arduino.h>
# 1 "C:/Users/Instrutor/Documents/GitHub/ESP32_Rele_ilumina/ESP32-iluminacao/src/main.ino"

#include <WiFi.h>
#include <PubSubClient.h>


const char *SSID = "AAPM-510";
const char *PASSWORD = "aapmSenai#";


const char *MQTT_HOST = "broker.hivemq.com";
const int MQTT_PORT = 1883;


WiFiClient espClient;
PubSubClient client(espClient);


const char *MQTT_TOPICO_SALA_LIGAR = "senai301/lampada/sala/ligar";
const char *MQTT_TOPICO_SALA_DESLIGAR = "senai301/lampada/sala/desligar";
const char *MQTT_TOPICO_COZINHA_LIGAR = "senai301/lampada/cozinha/ligar";
const char *MQTT_TOPICO_COZINHA_DESLIGAR = "senai301/lampada/cozinha/desligar";

const int PINO_RELE_SALA = 23;
const int PINO_RELE_COZINHA = 19;
void setup();
void loop();
void conectarWifi();
void conectarMQTT();
void mensagemRecebida(char *topico, byte *payload, unsigned int tamanho);
#line 26 "C:/Users/Instrutor/Documents/GitHub/ESP32_Rele_ilumina/ESP32-iluminacao/src/main.ino"
void setup()
{
  Serial.begin(115200);
  conectarWifi();
  conectarMQTT();
  pinMode(PINO_RELE_SALA, OUTPUT);
  pinMode(PINO_RELE_COZINHA, OUTPUT);
}

void loop()
{
  if (!client.connected())
  {
    Serial.println("MQTT desconectado");
    conectarMQTT();
  }

  client.loop();
  delay(1000);
}

void conectarWifi()
{
  WiFi.begin(SSID, PASSWORD);
  Serial.println("Conectando no Wifi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
  }

  String ip = " " + WiFi.localIP().toString();
  Serial.println("Conectado WiFi");
  Serial.println(ip.c_str());
}

void conectarMQTT()
{

  client.setServer(MQTT_HOST, MQTT_PORT);


  client.setCallback(mensagemRecebida);

  while(!client.connected())
  {

    String clientId = "ESP32";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str()))
    {
      Serial.println("Conectado ao Broker :)");
      Serial.println(clientId);
    }
    else
    {
      Serial.println("Erro ao conectar no Broker :(");
      String mqttError = "Codigo do erro: " + String(client.state());
      Serial.println(mqttError.c_str());
    }
  }


  client.subscribe(MQTT_TOPICO_SALA_LIGAR);
  client.subscribe(MQTT_TOPICO_SALA_DESLIGAR);
  client.subscribe(MQTT_TOPICO_COZINHA_LIGAR);
  client.subscribe(MQTT_TOPICO_COZINHA_DESLIGAR);
}

void mensagemRecebida(char *topico, byte *payload, unsigned int tamanho)
{
  if (strcmp(topico, MQTT_TOPICO_SALA_LIGAR) == 0)
  {

    digitalWrite(PINO_RELE_SALA, LOW);
  }
  else if (strcmp(topico, MQTT_TOPICO_SALA_DESLIGAR) == 0)
  {
    digitalWrite(PINO_RELE_SALA, HIGH);
  }
   else if (strcmp(topico, MQTT_TOPICO_COZINHA_LIGAR) == 0)
  {
    digitalWrite(PINO_RELE_COZINHA, LOW);
  }
   else if (strcmp(topico, MQTT_TOPICO_COZINHA_DESLIGAR) == 0)
  {
    digitalWrite(PINO_RELE_COZINHA, HIGH);
  }
}