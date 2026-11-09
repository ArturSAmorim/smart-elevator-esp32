#include <Arduino.h>
#include "config.h"
#include "botoes.h"
#include "pedidos.h"

static const int pinosBotao[NUM_ANDARES] = {BT_TERREO, BT_ANDAR1, BT_ANDAR2, BT_ANDAR3};

void botoesIniciar() {
  for (int i = 0; i < NUM_ANDARES; i++) {
    if (!andarHabilitado(i)) continue;   // botao do terreo ignorado quando desligado
    pinMode(pinosBotao[i], INPUT_PULLUP);
  }
}

void lerBotoes() {
  for (int i = 0; i < NUM_ANDARES; i++) {
    if (!andarHabilitado(i)) continue;
    if (digitalRead(pinosBotao[i]) == LOW) {
      registrarPedido(i);
    }
  }
}
