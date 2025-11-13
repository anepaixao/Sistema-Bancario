# 💰 Sistema Bancário

Projeto desenvolvido para disciplina de Linguagem de Programação II.

## 👥 Equipe
- Ane Paixão
- Laura Luiza

## 🧱 Estrutura
- **main.c:** menu principal (cliente e administrador)
- **banco.h:** definição da estrutura `Conta` e protótipos do módulo administrador
- **administrador.c:** operações administrativas (autenticação, criar, listar, bloquear, desbloquear, saldo total recursivo)
- **cliente.c / cliente.h:** módulo do cliente (responsabilidade da outra parte da dupla)

## 🔑 Funções (Módulo Administrador)
- `adminAutenticar` – valida credenciais do administrador
- `adminMenu` – loop do menu administrativo
- `adminCriarConta` – cria conta com validação de nome não vazio, CPF e senha numérica (6 dígitos)
- `adminListarContas` – lista contas ativas e bloqueadas
- `adminBloquearConta` / `adminDesbloquearConta` – altera status
- `adminCalcularSaldoTotalRecursivo` – exemplo de recursão para somar saldos

## 🧪 Validações
- Nome: não pode ser vazio ou apenas espaços
- CPF: verificação de dígitos e dígitos verificadores
- Senha: exatamente 6 caracteres numéricos + confirmação

## 📐 Decisões de Design
- Recursão utilizada apenas onde pedagógico (soma total) sem impacto negativo de desempenho dado pequeno volume de contas.
- Realocação dinâmica dobra capacidade do vetor de contas para reduzir fragmentação.
 
## 🎯 Tópicos Avançados (Administrador)
- Ponteiros: simples e duplos (`Conta*`, `Conta**` em gestão de capacidade)
- Alocação dinâmica: `malloc`, `realloc`, `free` (vetor de contas e matriz 2x2)
- Recursão: soma de saldos (`adminCalcularSaldoTotalRecursivo`)
- Estruturas: `struct Conta` (dados das contas)
- Listas encadeadas: lista simples de contas bloqueadas (uso interno)

### Onde está no código
- Estruturas: `banco.h` (`struct Conta`); `Trabalho final/administrador.c` (`typedef struct NoBloq`).
- Ponteiros simples: `adminCriarConta(Conta*, int*)`, `adminListarContas(Conta*, int)` em `Trabalho final/administrador.c`.
- Aritmética de ponteiros: laço em `adminListarContas` com `for (Conta *p = contas; p < contas + total; p++)`.
- Ponteiros duplos: `adminGarantirCapacidade(Conta **, int *, int)` e chamada em `adminMenu` (`&contas`, `&capacidade`).
- Alocação dinâmica (vetor): `adminMenu` (`malloc` e `free` de `contas`) e `adminGarantirCapacidade` (`realloc`).
- Alocação dinâmica (matriz): `adminMatrizDinamicaPequena` em `Trabalho final/administrador.c` (alocação em duas etapas e liberação).
- Recursão: `adminCalcularSaldoTotalRecursivo(Conta*, int, int)` em `Trabalho final/administrador.c`.
- Listas encadeadas: `NoBloq`, `construirListaBloqueadas`, `liberarListaBloqueadas` usados dentro de `adminListarContas` em `Trabalho final/administrador.c`.


## 🚀 Como compilar (Windows PowerShell)
```powershell
gcc -Wall -Wextra -std=c11 "Trabalho final/main.c" "Trabalho final/administrador.c" "Trabalho final/cliente.c" -I "Trabalho final" -o "Trabalho final/output/banco.exe"
"Trabalho final/output/banco.exe"
```

## 🔐 Acesso
- Administrador: usuário `ane` senha `admin`

## 📄 Licença
Veja arquivo `LICENSE`.

## ⚙️ Como compilar
```bash
gcc main.c administrador.c cliente.c -o banco.exe
./banco.exe
