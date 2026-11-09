#pragma once

// 1 = motor simulado (so imprime no Serial), 0 = motor real
#define SIMULAR_MOTOR 0

#define TERREO_HABILITADO 0

// Botoes de chamada 
#define BT_TERREO 33
#define BT_ANDAR1 32
#define BT_ANDAR2 4
#define BT_ANDAR3 12

// Sensores de andar
#define SENSOR_TERREO 35
#define SENSOR_ANDAR1 34
#define SENSOR_ANDAR2 39
#define SENSOR_ANDAR3 36

// LCD I2C
#define LCD_SDA 27
#define LCD_SCL 14

// Motor 
#define SERVO_PIN 13
constexpr int VELOCIDADE_PARADO  = 1500;
constexpr int VELOCIDADE_DESCIDA = 1400;
constexpr int VELOCIDADE_SUBIDA  = 1750;

constexpr int NUM_ANDARES = 4;
constexpr int ANDAR_MINIMO = TERREO_HABILITADO ? 0 : 1; 


constexpr bool andarHabilitado(int i) { return i >= ANDAR_MINIMO && i < NUM_ANDARES; }

constexpr unsigned long TIMEOUT_MS = 15000;
constexpr unsigned long TEMPO_ESTABILIZACAO_MS = 150;
constexpr unsigned long TEMPO_CONFIRMACAO_MS = 5;
constexpr unsigned long TEMPO_PORTA_ABERTA_MS = 2000;
