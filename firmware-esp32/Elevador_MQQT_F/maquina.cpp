#include <Arduino.h>
#include "config.h"
#include "maquina.h"
#include "motor.h"
#include "sensores.h"
#include "pedidos.h"
#include "display.h"

static Estado estadoAtual = CALIBRANDO;
static Sentido ultimoSentido = NENHUM;
static int andarAtual = -1;
static unsigned long inicioEstado = 0;

static const char* nomeEstado(Estado e) {
  switch (e) {
    case CALIBRANDO: return "CALIBRANDO";
    case PARADO:     return "PARADO";
    case SUBINDO:    return "SUBINDO";
    case DESCENDO:   return "DESCENDO";
    case ATENDENDO:  return "ATENDENDO";
    case ERRO:       return "ERRO";
  }
  return "?";
}
const char* nomeDoEstado(Estado e) { return nomeEstado(e); }
static void mudarEstado(Estado novo) {
  
  estadoAtual = novo;
  inicioEstado = millis();
  Serial.print("Estado: ");
  Serial.println(nomeEstado(novo));
}


static int andarDeCalibracao() {
#if TERREO_HABILITADO
  return sensorAtivo(0) ? 0 : -1;
#else
  for (int i = ANDAR_MINIMO; i < NUM_ANDARES; i++) {
    if (sensorAtivo(i)) return i;
  }
  return -1;
#endif
}

void maquinaIniciar() {
  andarAtual = -1;
  ultimoSentido = NENHUM;
  mudarEstado(CALIBRANDO);
}

Estado estadoDaMaquina() { return estadoAtual; }
int andarDoElevador() { return andarAtual; }
int destinoDoElevador() {
  if (andarAtual < 0) return -1;   // ainda calibrando
  Sentido s = escolherSentido(andarAtual, ultimoSentido);
  return proximoPedido(andarAtual, s);
}

void atualizarMaquina() {
  switch (estadoAtual) {

    case CALIBRANDO: {
      if (millis() - inicioEstado < TEMPO_ESTABILIZACAO_MS) break;

      int achado = andarDeCalibracao();
      if (achado >= 0) {
        pararMotor();
        andarAtual = achado;
        Serial.print("Calibrado no andar ");
        Serial.println(achado);
        mudarEstado(PARADO);
      } else {
#if TERREO_HABILITADO
        if (direcaoMotor() != DESCER) {
          descerMotor();
          Serial.println("Calibrando: descendo ate o terreo");
        }
#else
        if (direcaoMotor() != SUBIR) {
          subirMotor();
          Serial.println("Calibrando: subindo ate achar um sensor");
        }
#endif
        if (millis() - inicioEstado > TIMEOUT_MS) {
          pararMotor();
          Serial.println("Erro de timeout na calibracao");
          mudarEstado(ERRO);
        }
      }
      break;
    }

    case PARADO: {
      if (haPedido(andarAtual)) {
        limparPedido(andarAtual);
        mudarEstado(ATENDENDO);
        break;
      }

      Sentido s = escolherSentido(andarAtual, ultimoSentido);

      if (s == SUBIR) {
        ultimoSentido = SUBIR;
        subirMotor();
        mudarEstado(SUBINDO);
      } else if (s == DESCER) {
        ultimoSentido = DESCER;
        descerMotor();
        mudarEstado(DESCENDO);
      }
      break;
    }

    case SUBINDO:
    case DESCENDO: {
      for (int i = 0; i < NUM_ANDARES; i++) {
        if (sensorChegou(i)) {
          andarAtual = i;
          if (haPedido(i)) {
            pararMotor();
            limparPedido(i);
            mudarEstado(ATENDENDO);
            return;
          }
        }
      }

      if (direcaoMotor() == NENHUM) {
        mudarEstado(PARADO);
        break;
      }

      if (millis() - inicioEstado > TIMEOUT_MS) {
        pararMotor();
        Serial.println("Erro de timeout no movimento");
        mudarEstado(ERRO);
      }
      break;
    }

    case ATENDENDO:
      if (haPedido(andarAtual)) {
        limparPedido(andarAtual);
        inicioEstado = millis();
      }
      if (millis() - inicioEstado >= TEMPO_PORTA_ABERTA_MS) {
        mudarEstado(PARADO);
      }
      break;

    case ERRO:
      pararMotor();
      break;

    default:
      break;
  }
}
