# 💰 Sistema Bancário (C-Bank)

Projeto de console (C11) com foco em organização modular, persistência em arquivo binário, TUI com cores, entradas mascaradas (CPF/senha) e geração de comprovantes/boletos/contratos em HTML.

Discentes: Ane Paixão e Laura Luiza
Disciplina: LPII

---

## ✨ Destaques do Projeto
- Entrada mascarada com toggle: CPF e senha com exibição segura (TAB alterna visibilidade).
- TUI padronizada (cores/headers/barra de progresso) para melhor UX no console.
- Persistência binária transparente em `dados.bin` + log de operações em `log.txt`.
- Geração de HTML com autoabertura no navegador: `output/comprovante.html`, `output/boleto.html`, `output/contrato.html`.
- Bitwise para configurações/estados em 1 byte (flags); recursão aplicada (soma de saldos / simulação de rendimento).
- Arquitetura modular clara: `banco` (núcleo), `unir` (utilitários/TUI), `administrador`, `cliente`, `main`.

---

## 🗂️ Estrutura do Repositório

```
Sistema-Bancario/
├─ README.md
├─ LICENSE
└─ Trabalho_final/
	 ├─ main.c                 # Ponto de entrada e orquestração
	 ├─ banco.h / banco.c      # Núcleo: struct Conta, persistência, capacidade, log
	 ├─ unir.h / unir.c        # Utilidades: TUI, validações, entradas mascaradas, HTML
	 ├─ administrador.h / .c   # Módulo Admin: CRUD de contas, flags, salvar/carregar
	 ├─ cliente.h / cliente.c  # Módulo Cliente: login, extrato, depósito, saque, TED/PIX
	 ├─ run_test.bat           # Execução automatizada (usa test_input.txt)
	 ├─ test_input.txt         # Entrada simulada de teste
	 ├─ build/                 # (pasta auxiliar)
	 └─ output/                # binários/artefatos: banco.exe, comprovante.html, etc.
```

---

## 🧠 Modelo de Dados

Estrutura central (em `banco.h`):

```
typedef struct {
		int id;               // Número da conta
		char nome[50];        // Nome do titular
		char cpf[15];         // CPF (apenas dígitos armazenados)
		char senha[20];       // Senha numérica (6 dígitos)
		float saldo;          // Saldo em conta
		unsigned char flags;  // Estados (bitwise)
} Conta;
```

Flags principais (em `banco.h`):
- `FLAG_BLOQUEADA` (bit 0)
- `FLAG_PREMIUM` (bit 1)
- `FLAG_EMAIL_VERIFIED` (bit 2)

No módulo do cliente (`cliente.c`) também uso bits adicionais (4-6) para configurações (ex.: notificações/“transações noturnas”).

---

## 🧩 Arquitetura e Responsabilidades

- `banco.h/c` (núcleo)
	- Persistência binária: `salvarDados`, `carregarDados` (arquivo `dados.bin`).
	- Capacidade do vetor: `garantirCapacidade` (crescimento com `realloc`).
	- Log de auditoria: `registrarLog` (anexa em `log.txt` com data/hora).

- `unir.h/c` (utilidades e TUI)
	- UI: `cabecalho`, `barraCarregamento`, mensagens coloridas.
	- Entradas robustas: `lerInteiro`, `lerDouble`, `lerLinha`.
	- Máscaras: `lerCPFmascarado`, `lerSenhamascarada` (TAB alterna visibilidade);
		`validarCPF`, `senhaValidaNDigitos`.
	- Negócio compartilhado: `buscarIndicePorId/CPF`, `gerarProximoId`.
	- HTML: `gerarComprovanteHTML`, `gerarBoletoHTML`, `gerarContratoHTML`.

- `administrador.h/c`
	- Autenticação (hardcoded): usuário `ane`, senha `admin`.
	- Criar/Listar/Bloquear/Desbloquear/Remover contas.
	- Salvar/Carregar base (menu 8/9) via `banco.c`.
	- Recursão: `adminCalcularSaldoTotalRecursivo` soma todos os saldos.
	- Demonstração: lista encadeada de contas bloqueadas; matriz dinâmica 2x2.

- `cliente.h/c`
	- Login com CPF e senha mascarados (CPF normalizado para somente dígitos).
	- Extrato, Depósito (dinheiro/boleto), Saque.
	- Transferência: PIX (simulado) e TED (gera comprovante HTML).
	- Extras: configurações com bitwise; simulação de rendimento (recursão).

- `main.c`
	- Inicia carregando `dados.bin` (se existir), exibe menu “Admin”/“Cliente”,
		e ao sair salva novamente os dados.

---

## 🖥️ Execução e Build

Você pode compilar pelo VS Code (Task pronta) ou manualmente no PowerShell.

1) Usando a Task do VS Code (recomendado)
- Abra a pasta do repositório no VS Code.
- Execute a task “Build banco (gcc)”. Ela compila com `-Wall -Wextra -std=c11` e gera `Trabalho_final/output/banco.exe`.

2) Compilação manual (PowerShell, dentro de `Trabalho_final`)

```powershell
gcc -Wall -Wextra -std=c11 main.c administrador.c cliente.c unir.c banco.c -I . -o output\banco.exe
```

3) Executar

```powershell
cd .\Trabalho_final
.\output\banco.exe
```

Observações
- A build task já usa o diretório de trabalho `Trabalho_final`, então os caminhos relativos funcionam (ex.: `dados.bin`, `output\*.html`).
- Os comprovantes/boletos/contratos são salvos em `Trabalho_final/output` e abertos automaticamente no navegador padrão.

---

## 📦 Persistência e Arquivos Gerados

- Banco de dados: `dados.bin` (arquivo binário) — salvo na saída do programa e carregado na inicialização.
- Log de auditoria: `log.txt` — anota operações (saque, depósito, transferência) com timestamp.
- HTMLs (em `output/`):
	- `comprovante.html` — ao concluir uma TED (origem/destino/valor/descrição).
	- `boleto.html` — quando o cliente escolhe depósito via boleto.
	- `contrato.html` — emitido pelos “Planos de Segurança”.

---

## 🔐 Fluxo do Administrador

Credenciais
- Usuário: `ane`
- Senha: `admin`

Menu (resumo)
- Criar conta: valida nome, CPF (dígitos verificadores) e senha (6 dígitos); `id` é gerado automaticamente.
- Listar contas: todas/ativas/bloqueadas (uso de filtro via callback).
- Bloquear/Desbloquear: marca/limpa `FLAG_BLOQUEADA` por número da conta.
- Salvar/Carregar: serializa tudo para `contas.dat` (opcional no admin; fora isso, o sistema usa `dados.bin` na entrada/saída do app).
- Saldo total (recursivo): soma `saldo` de todas as contas.
- Remover conta: desloca o vetor (shift à esquerda) e decrementa `total`.

---

## 👤 Fluxo do Cliente

- Login: digite CPF (somente dígitos) e senha mascarada; CPF é validado e normalizado.
- Extrato: mostra informações básicas da conta.
- Depósito: valor em dinheiro; opção de gerar boleto (HTML) e depois somar ao saldo.
- Saque: checa bloqueio e saldo suficiente.
- Transferência:
	- PIX: fluxo simulado (opções de chave/QR Code).
	- TED: entre contas do sistema; gera comprovante HTML.
- Outras opções:
	- Planos de segurança: contratos e comprovantes (HTML).
	- Configurações (bitwise): ativar/desativar flags como “Notificações” e “Transações Noturnas”.
	- Simulação de rendimento: recursão mês a mês com 1% a.m. (demonstração pedagógica).

Entradas mascaradas
- CPF: mostra apenas primeiro e último dígitos; TAB alterna entre oculto/visível.
- Senha: mostra asteriscos; TAB alterna exibição.

---

## 🧪 Testes Rápidos

Entrada automatizada (se desejado):

```powershell
cd .\Trabalho_final
.\run_test.bat
```

- O script utiliza `test_input.txt` para alimentar o programa e redireciona a saída para `output\test_run.txt` (ajuste conforme necessidade).

---

## 🛠️ Solução de Problemas

- Cores ANSI não aparecem? Tente o Windows Terminal/PowerShell atualizados. No pior caso, verá sequências `\033`, é apenas visual.
- HTML não abre automaticamente? Verifique permissões do Windows e o caminho `output\*.html`. Abra manualmente no navegador.
- `dados.bin` “sumiu”? Confirme o diretório de execução. Rodando dentro de `Trabalho_final`, o arquivo será criado ali.
- Caminho com espaço (OneDrive): sempre use aspas ou a task do VS Code (já configurada).

---

## ⚠️ Notas de Segurança

- Credenciais do administrador são fixas (apenas para fins acadêmicos). Não use isso em produção.
- O arquivo `dados.bin` não é criptografado; o foco é didático, não segurança real.

---

## 🚀 Próximos Passos (Sugestões)

- Criptografia/Hash de senhas (ex.: SHA-256) e política de senha forte.
- Transações PIX reais no fluxo por chave (dados de favorecido/descrição).
- Exportação de extrato (CSV/HTML) e relatórios com filtros.
- Testes automatizados com entradas variadas e validação de saídas.

---

## 📄 Licença

Consulte o arquivo `LICENSE` na raiz do repositório.

## ⚙️ Como compilar
```bash
gcc main.c administrador.c cliente.c banco.c unir.c -o banco.exe
./banco.exe
