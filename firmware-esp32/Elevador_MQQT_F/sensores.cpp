#include <Arduino.h>
#include "config.h"
#include "sensores.h"

struct Sensor {
  int pino;
  bool estavel;
  bool ultimaLeitura;
  unsigned long instanteMudanca;
  bool anterior;
};

static Sensor sensores[NUM_ANDARES] = {
  {SENSOR_TERREO, false, false, 0, false},
  {SENSOR_ANDAR1, false, false, 0, false},
  {SENSOR_ANDAR2, false, false, 0, false},
  {SENSOR_ANDAR3, false, false, 0, false},
};


static bool indiceValido(int i) { return andarHabilitado(i); }

void sensoresIniciar() {
  for (int i = 0; i < NUM_ANDARES; i++) {
    if (!andarHabilitado(i)) continue;
    pinMode(sensores[i].pino, INPUT);
  }
}

static void atualizarSensor(int i) {
  bool leitura = (digitalRead(sensores[i].pino) == HIGH);

  if (leitura != sensores[i].ultimaLeitura) {
    sensores[i].ultimaLeitura = leitura;
    sensores[i].instanteMudanca = millis();
  }

  if (millis() - sensores[i].instanteMudanca >= TEMPO_CONFIRMACAO_MS) {
    sensores[i].estavel = leitura;
  }
}

void atualizarSensores() {
  for (int i = 0; i < NUM_ANDARES; i++) {
    sensores[i].anterior = sensores[i].estavel;
    if (andarHabilitado(i)) {
      atualizarSensor(i);
    } else {
      sensores[i].estavel = false;
    }
  }
}

bool sensorAtivo(int i) {
  if (!indiceValido(i)) return false;
  return sensores[i].estavel;
}

bool sensorChegou(int i) {
  if (!indiceValido(i)) return false;
  return sensores[i].estavel && !sensores[i].anterior;
}
