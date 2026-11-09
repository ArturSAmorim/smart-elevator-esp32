#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"
#include "display.h"
#include "tipos.h"

static LiquidCrystal_I2C lcd(0x3F, 16, 2);
static int ultimoAndar = -99, ultimoBotao = -99, ultimoEstado = -99;

void displayIniciar() {
  Wire.begin(LCD_SDA, LCD_SCL);
  lcd.init();
  lcd.backlight();
}

void displayMostrarAndar(int andaratual, int botao, Estado e) {
  if (andaratual == ultimoAndar && botao == ultimoBotao && (int)e == ultimoEstado) return;
  const char* status;
  switch (e) {
    case CALIBRANDO: status = "CAL"; break;
    case PARADO:     status = "PAR"; break;
    case SUBINDO:    status = "SUB"; break;
    case DESCENDO:   status = "DES"; break;
    case ATENDENDO:  status = "ATE"; break;
    case ERRO:       status = "ERR"; break;
    default:         status = "404"; break;
  }

  lcd.setCursor(0, 0);
  lcd.print("Destino:");
  lcd.print(botao);
  lcd.print(" ");
  lcd.setCursor(12, 0);
  lcd.print(status);

  lcd.setCursor(0, 1);
  lcd.print("Atual: ");
  if (andaratual == 0) lcd.print("T");
  else lcd.print(andaratual);
  lcd.print("     ");
  ultimoAndar = andaratual;
  ultimoBotao = botao;
  ultimoEstado = (int)e;
}
