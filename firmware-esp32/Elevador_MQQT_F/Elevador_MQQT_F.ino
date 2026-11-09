#include "motor.h"
#include "sensores.h"
#include "botoes.h"
#include "seguranca.h"
#include "display.h"
#include "maquina.h"
#include "mqtt_client.h"
#include "config.h"
void setup() {
  Serial.begin(115200);

  motorIniciar();
  sensoresIniciar();
  botoesIniciar();
  displayIniciar();
  maquinaIniciar();
  mqttIniciar();
}

void loop() {
  atualizarSensores();   
  lerBotoes();           
  verificarLimites();  
  atualizarMaquina();
  mqttAtualizar(); 
  displayMostrarAndar(andarDoElevador(), destinoDoElevador(), estadoDaMaquina());   
}
