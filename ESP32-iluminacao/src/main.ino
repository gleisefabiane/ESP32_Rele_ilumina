// Importações das bibliotecas
#include <WiFi.h> // Padrão do Esp32
#include <PubSubClient.h> // Necessário para fazer comunicação broker

// Configurar a conexão com Wifi
const char *SSID = "AAPM-510";
const char *PASSWORD = "aapmSenai#";   

// Configurações do Broker MQTT
const char *MQTT_HOST = "broker.hivemq.com";
const int MQTT_PORT = 1883;

// Criando as instâncias (objetos)
WiFiClient espClient;
PubSubClient client(espClient);

//Tópicos que vai escutar
const char *MQTT_TOPICO_SALA_LIGAR = "senai301/lampada/sala/ligar";
const char *MQTT_TOPICO_SALA_DESLIGAR = "senai301/lampada/sala/desligar";
const char *MQTT_TOPICO_COZINHA_LIGAR = "senai301/lampada/cozinha/ligar";
const char *MQTT_TOPICO_COZINHA_DESLIGAR = "senai301/lampada/cozinha/desligar";

const int PINO_RELE_SALA = 23;
const int PINO_RELE_COZINHA = 19;

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
  // Define o servidor do MQTT que vai receber as mensagens
  client.setServer(MQTT_HOST, MQTT_PORT);

  // Define a função que será chamada quando receber uma mensagem
  client.setCallback(mensagemRecebida);

  while(!client.connected())
  {
    // Cria um identificador único para a conexão
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

  // Se inscreve no(s) tópico(s)
  client.subscribe(MQTT_TOPICO_SALA_LIGAR);
  client.subscribe(MQTT_TOPICO_SALA_DESLIGAR);
  client.subscribe(MQTT_TOPICO_COZINHA_LIGAR);
  client.subscribe(MQTT_TOPICO_COZINHA_DESLIGAR);
}

void mensagemRecebida(char *topico, byte *payload, unsigned int tamanho)
{
  if (strcmp(topico, MQTT_TOPICO_SALA_LIGAR) == 0)
  {
    // ATENÇÃO NO RELE USADO É INVERTIDO O ESTADO PARA LIGAR - LOW***
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