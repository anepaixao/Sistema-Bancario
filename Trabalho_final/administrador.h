#ifndef ADMINISTRADOR_H
#define ADMINISTRADOR_H

#include "banco.h"

// Módulo do Administrador — deixei aqui os pontos de entrada que uso na main
int adminAutenticar(void);
void adminMenu(Conta **contas, int *total, int *capacidade);
void adminCriarConta(Conta *contas, int *total);
void adminListarContas(Conta *contas, int total, int (*filtro)(const Conta *));
void adminBloquearConta(Conta *contas, int total);
void adminDesbloquearConta(Conta *contas, int total);
float adminCalcularSaldoTotalRecursivo(Conta *contas, int indice, int total);
void adminRemoverConta(Conta **contas, int *total);

#endif // ADMINISTRADOR_H
