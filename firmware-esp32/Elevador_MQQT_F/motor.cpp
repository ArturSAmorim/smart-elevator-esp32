#include <Arduino.h>
#include <ESP32Servo.h>
#include "config.h"
#include "motor.h"

static Servo servo;
static Sentido direcao = NENHUM;

void motorIniciar() {
  ESP32PWM::allocateTimer(0);
  servo.setPeriodHertz(50);
  servo.attach(SERVO_PIN, 1200, 2400);
  pararMotor();
}

void subirMotor()  { direcao = SUBIR;  servo.writeMicroseconds(VELOCIDADE_SUBIDA); }
void descerMotor() { direcao = DESCER; servo.writeMicroseconds(VELOCIDADE_DESCIDA); }
void pararMotor()  { direcao = NENHUM; servo.writeMicroseconds(VELOCIDADE_PARADO); }

Sentido direcaoMotor() { return direcao; }