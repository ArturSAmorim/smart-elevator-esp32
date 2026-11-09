#pragma once
#include "tipos.h"

void registrarPedido(int andar);
void limparPedido(int andar);
bool haPedido(int andar);
bool pedidoAcima(int andar);
bool pedidoAbaixo(int andar);
Sentido escolherSentido(int andar, Sentido sentidoAtual);
int proximoPedido(int andar, Sentido sentido);
