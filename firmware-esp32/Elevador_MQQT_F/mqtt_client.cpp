#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <stdio.h>
#include <string.h>
#include "secrets.h"
#include "config.h"
#include "mqtt_client.h"
#include "maquina.h"
#include "motor.h"
#include "pedidos.h"


static WiFiClient redeWifi;
static PubSubClient mqtt(redeWifi);

static const char* T_COMANDO = "elevador/comando";   
static const char* T_ANDAR   = "elevador/andar";     
static const char* T_ESTADO  = "elevador/estado";
static const char* T_DESTINO = "elevador/destino";
static const char* T_ONLINE  = "elevador/online";

static const char* ID_CLIENTE = "elevador-esp32";
static const unsigned long INTERVALO_TENTATIVA_MS = 5000;  
static const int  LIMITE_SOCKET_MS = 500;                   
static const int  LIMITE_RESPOSTA_S = 2;                    


static unsigned long ultimaTentativa = 0;
static bool jaTentou = false;
static bool avisouWifi = false;


static int andarPublicado   = -99;
static int estadoPublicado  = -99;
static int destinoPublicado = -99;
static void callBack(char* topico, uint8_t* payload, unsigned int tamanho) {
  if (strcmp(topico, T_COMANDO) != 0) return;
  if (tamanho != 1) return;

  char c = (char)payload[0];
  if (c < '0' || c >= '0' + NUM_ANDARES) return;

  int andar = c - '0';
  registrarPedido(andar);
  Serial.print("MQTT comando: andar ");
  Serial.println(andar);
}
static bool podeTentarConectar() {
  if (WiFi.status() != WL_CONNECTED) return false;  
  if (direcaoMotor() != NENHUM) return false;             
  if (jaTentou && millis() - ultimaTentativa < INTERVALO_TENTATIVA_MS) return false;
  return true;
}

static void tentarConectar() {
  if (!podeTentarConectar()) return;
  jaTentou = true;
  ultimaTentativa = millis();

  redeWifi.stop();
  if (!redeWifi.connect(MQTT_HOST, MQTT_PORTA, LIMITE_SOCKET_MS)) return;
  if (!mqtt.connect(ID_CLIENTE, T_ONLINE, 1, true, "offline")) return;

  mqtt.subscribe(T_COMANDO);
  mqtt.publish(T_ONLINE, "online", true);
  andarPublicado = estadoPublicado = destinoPublicado = -99;
  Serial.println("MQTT conectado");
}
static void andarParaTexto(int andar, char* texto, size_t tamanho) {
  if (andar < 0) snprintf(texto, tamanho, "-");
  else           snprintf(texto, tamanho, "%d", andar);
}

static void publicarMudancas() {
  char texto[8];

  int andar = andarDoElevador();
  if (andar != andarPublicado) {
    andarParaTexto(andar, texto, sizeof(texto));
    mqtt.publish(T_ANDAR, texto, true);
    andarPublicado = andar;
  }

  int estado = (int)estadoDaMaquina();
  if (estado != estadoPublicado) {
    mqtt.publish(T_ESTADO, nomeDoEstado(estadoDaMaquina()), true);
    estadoPublicado = estado;
  }

  int destino = destinoDoElevador();
  if (destino != destinoPublicado) {
    andarParaTexto(destino, texto, sizeof(texto));
    mqtt.publish(T_DESTINO, texto, true);
    destinoPublicado = destino;
  }
}

void mqttIniciar() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_SENHA);       
  mqtt.setServer(MQTT_HOST, MQTT_PORTA);
  mqtt.setCallback(callBack);
  mqtt.setSocketTimeout(LIMITE_RESPOSTA_S);
}

void mqttAtualizar() {
  if (WiFi.status() == WL_CONNECTED && !avisouWifi) {
    avisouWifi = true;
    Serial.print("Wi-Fi conectado: ");
    Serial.println(WiFi.localIP());
  }
  if (WiFi.status() != WL_CONNECTED) avisouWifi = false;

  if (!mqtt.connected()) {
    tentarConectar();
    return;
  }

  mqtt.loop();          
  publicarMudancas();
}