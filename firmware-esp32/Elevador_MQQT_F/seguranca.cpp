#include <Arduino.h>
#include "config.h"
#include "seguranca.h"
#include "motor.h"
#include "sensores.h"

void verificarLimites() {
  if (direcaoMotor() == DESCER && sensorAtivo(ANDAR_MINIMO)) pararMotor();
  if (direcaoMotor() == SUBIR  && sensorAtivo(NUM_ANDARES - 1)) pararMotor();
}
