# Sistema Blua - Gerenciamento de Filas para Teleconsultas

## Integrantes
- Henry Vasconcellos (principal responsável pela lógica e implementação)
- Colaboradores do grupo responsáveis por testes, validação e apoio no desenvolvimento

---

## Descrição do Projeto

Este projeto foi desenvolvido em linguagem C com o objetivo de simular um sistema de gerenciamento de filas para teleconsultas da plataforma Blua da Care Plus.

O sistema utiliza:
- Filas FIFO para atendimento normal
- Fila prioritária para emergências
- Pilha LIFO para ações administrativas
- Listas encadeadas implementadas manualmente

O projeto foi desenvolvido sem uso de bibliotecas prontas de fila ou pilha.

---

## Estruturas Utilizadas

### Fila (FIFO)
Utilizada para organizar pacientes na ordem de chegada.

Operações implementadas:
- enqueue
- dequeue
- peek
- is_empty

### Pilha (LIFO)
Utilizada para registrar ações administrativas.

Operações implementadas:
- push
- pop
- peek
- is_empty

---

## Funcionalidades

- Cadastro de pacientes
- Solicitação de teleconsulta
- Priorização de emergências
- Atendimento sequencial
- Histórico individual limitado a 3 consultas
- Registro de ações administrativas
- Desfazer ações administrativas
- Menu interativo

---

## Como Executar

### 1. Compile o código

```bash
gcc SprintFinal.c -o SprintFinal
```

### 2. Execute

```bash
./SprintFinal
```

No Windows PowerShell:

```powershell
.\SprintFinal.exe
```

---

## Estrutura do Sistema

O sistema foi implementado utilizando listas encadeadas através da estrutura Node.

A partir dela foram criadas:
- Queue
- Stack

Cada paciente possui:
- ID
- Nome
- Sintomas
- Status de emergência
- Histórico individual

---

## Cenários Demonstrados

1. Atendimento FIFO
2. Priorização de emergência
3. Histórico limitado
4. Ações administrativas com pilha

---

## Complexidade das Operações

| Operação | Complexidade |
|---|---|
| enqueue | O(1) |
| dequeue | O(1) |
| push | O(1) |
| pop | O(1) |
| peek | O(1) |

---

## Tecnologias Utilizadas

- Linguagem C
- GCC
- VS Code
- Listas Encadeadas
