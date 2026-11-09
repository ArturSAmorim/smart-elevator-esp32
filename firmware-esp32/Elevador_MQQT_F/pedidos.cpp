#include <Arduino.h>
#include "config.h"
#include "pedidos.h"

static bool pendente[NUM_ANDARES] = {false, false, false, false};

// Andar desabilitado nao aceita pedido (o terreo some quando TERREO_HABILITADO = 0)
static bool andarValido(int andar) { return andarHabilitado(andar); }

void registrarPedido(int andar) {
  if (!andarValido(andar)) return;
   if (!pendente[andar]) {                  // DIAGNÓSTICO: só imprime quando o pedido é novo
    Serial.print("Pedido novo: andar ");
    Serial.println(andar);
  }
  pendente[andar] = true;
}

void limparPedido(int andar) {
  if (!andarValido(andar)) return;
  pendente[andar] = false;
}

bool haPedido(int andar) {
  if (!andarValido(andar)) return false;
  return pendente[andar];
}

bool pedidoAcima(int andar) {
  for (int i = andar + 1; i < NUM_ANDARES; i++) {
    if (pendente[i]) return true;
  }
  return false;
}

bool pedidoAbaixo(int andar) {
  for (int i = andar - 1; i >= ANDAR_MINIMO; i--) {
    if (pendente[i]) return true;
  }
  return false;
}

Sentido escolherSentido(int andar, Sentido sentidoAtual) {
  if (sentidoAtual == DESCER) {
    if (pedidoAbaixo(andar)) return DESCER;
    if (pedidoAcima(andar))  return SUBIR;
    return NENHUM;
  }
  if (pedidoAcima(andar))  return SUBIR;
  if (pedidoAbaixo(andar)) return DESCER;
  return NENHUM;
}

int proximoPedido(int andar, Sentido sentido) {
  if (sentido == SUBIR) {
    for (int i = andar + 1; i < NUM_ANDARES; i++) {
      if (pendente[i]) return i;
    }
  } else if (sentido == DESCER) {
    for (int i = andar - 1; i >= ANDAR_MINIMO; i--) {
      if (pendente[i]) return i;
    }
  }
  return -1;
}
