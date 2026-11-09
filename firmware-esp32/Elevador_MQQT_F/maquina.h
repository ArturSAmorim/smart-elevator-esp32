#pragma once
#include "tipos.h"

void maquinaIniciar();
void atualizarMaquina();
Estado estadoDaMaquina();
int andarDoElevador();
int destinoDoElevador();
const char* nomeDoEstado(Estado e);