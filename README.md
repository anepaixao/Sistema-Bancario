# 💰 Sistema Bancário

Projeto desenvolvido para disciplina de Linguagem de Programação II.

## 👥 Equipe
- Ane Paixão
- Laura Luiza

## 🧱 Estrutura
- **main.c:** menu principal (cliente e administrador)
- **administrador.c:** funções de operações bancárias do admin
- **cliente.c:** funções de operações bancarias do cliente
- **cliente.h:** definições e protótipos do clinte

## 🔑 Funções implementadas
- Criar conta, depositar, sacar, consultar saldo
- Bloquear/desbloquear contas (modo administrador)
- Soma total de saldos

## ⚙️ Como compilar
```bash
gcc main.c administrador.c cliente.c -o banco.exe
./banco.exe
