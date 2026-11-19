#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>

//injeção de coordenadas(simulando modulo gps)//
float latitudes[] = {
  -23.6689,
  -23.6698,
  -23.6678
};

float longitudes[] = {
  -46.4614,
  -46.4598,
  -46.4630
};

int posicaoGPS = 0;

unsigned long ultimoMovimento = 0;
const unsigned long intervaloGPS = 10000;

//variavei//
#define sensor_ar 34
#define sensor_som 35
float pm25 = 0;
float ruido = 0;

//config mqtt//
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

const char* mqtt_topic = "smartcity/projeto001/estacao01/dados";

WiFiClient espClient;
PubSubClient client(espClient);

//funções//

String gerar_json(float pm25, float ruido, int posicaoGPS) {

  JsonDocument dados;

  dados["pm25"] = pm25;
  dados["qualidade_ar"] = classificar_ar(pm25);

  dados["ruido"] = ruido;
  dados["classificacao_ruido"] = classificar_ruido(ruido);

  dados["latitude"] = latitudes[posicaoGPS];
  dados["longitude"] = longitudes[posicaoGPS];

  String mensagemJSON;

  serializeJson(dados, mensagemJSON);

  return mensagemJSON;
}

String classificar_ar(float pm25) {
    if(pm25 <= 15){return "bom";}

    else if(pm25 <= 37.5){return "medio";}

    else{return "ruim";}
  }

String classificar_ruido(float ruido) {
    if(ruido <= 50){return "bom";}

    else if(ruido <= 70){return "medio";}

    else{return "ruim";}
  }

void setup() {
  pinMode(sensor_ar, INPUT);
  pinMode(sensor_som, INPUT);

  analogReadResolution(12);
  
  Serial.begin(115200);
  conect_wifi();
  client.setServer(mqtt_server, mqtt_port);
  Serial.println("=========Monitor Iniciado=========");
}

void loop() {
  int leitura_ar = analogRead(sensor_ar);
  int leitura_som = analogRead(sensor_som);

  float pm25 = 5.0 + (leitura_ar / 4095.0) * 145.0;
  float ruido = 35.0 + (leitura_som / 4095.0) * 70.0;

  if (millis() - ultimoMovimento >= intervaloGPS) {

    ultimoMovimento = millis();

    posicaoGPS++;

    if (posicaoGPS >= 3) {
      posicaoGPS = 0;
    }
  }

  if (!client.connected()) {
  conect_mqtt();
  }

  client.loop();

  String mensagem = gerar_json(pm25, ruido, posicaoGPS);

  if(client.publish(mqtt_topic, mensagem.c_str())){
    Serial.println("Dados enviados ao MQTT");
    Serial.println(mensagem);
  }
  else{
    Serial.println("erro ao enviar dados");
  }
  
  Serial.println("========Dados da Leitura========");

  Serial.print("PM2.5: ");
  Serial.println(pm25);
  Serial.print("Qualidade: ");
  Serial.println(classificar_ar(pm25));

  Serial.print("Ruido: ");
  Serial.println(ruido);
  Serial.print("Qualidade: ");
  Serial.println(classificar_ruido(ruido));

  Serial.println("========Localização========");

  Serial.print("Latitude: ");
  Serial.println(latitudes[posicaoGPS], 6);

  Serial.print("Longitude: ");
  Serial.println(longitudes[posicaoGPS], 6);  

  Serial.println("DADOS JSON:");
  Serial.println(mensagem);

  delay(3000);

}

void conect_wifi(){
  Serial.println("Conectando ao Wi-Fi...");

  WiFi.mode(WIFI_STA);

  WiFi.begin("Wokwi-GUEST", "", 6);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi conectado!");

  Serial.print("Endereco IP: ");
  Serial.println(WiFi.localIP());

}

void conect_mqtt(){
  if(!client.connected()){
    Serial.println("Conectando ao broker MQTT...");

    if(client.connect("SmartCityESP32_001")){
      Serial.println("MQTT conectado");
    }

    else{
      Serial.print("falha na conexao.....");
      Serial.println(client.state());
    }
  }
}
