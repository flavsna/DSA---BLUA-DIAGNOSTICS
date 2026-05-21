#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORICO 3
#define MAX_PACIENTES 50

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct Queue {
    Node *front;
    Node *rear;
} Queue;

typedef struct Stack {
    Node *top;
} Stack;

typedef struct Consulta {
    char descricao[200];
} Consulta;

typedef struct Paciente {
    int id;
    char nome[50];
    char sintomas[120];
    int emergencia;
    Consulta historico[MAX_HISTORICO];
    int totalConsultas;
} Paciente;

/* ================= FILA ================= */

void initQueue(Queue *queue) {
    queue->front = NULL;
    queue->rear = NULL;
}

int isQueueEmpty(Queue *queue) {
    return queue->front == NULL;
}

void enqueue(Queue *queue, void *data) {
    Node *newNode = (Node *) malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    newNode->data = data;
    newNode->next = NULL;

    if (isQueueEmpty(queue)) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

void *dequeue(Queue *queue) {
    if (isQueueEmpty(queue)) {
        return NULL;
    }

    Node *temp = queue->front;
    void *data = temp->data;

    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    return data;
}

void *peekQueue(Queue *queue) {
    if (isQueueEmpty(queue)) {
        return NULL;
    }

    return queue->front->data;
}

/* ================= PILHA ================= */

void initStack(Stack *stack) {
    stack->top = NULL;
}

int isStackEmpty(Stack *stack) {
    return stack->top == NULL;
}

void push(Stack *stack, void *data) {
    Node *newNode = (Node *) malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    newNode->data = data;
    newNode->next = stack->top;
    stack->top = newNode;
}

void *pop(Stack *stack) {
    if (isStackEmpty(stack)) {
        return NULL;
    }

    Node *temp = stack->top;
    void *data = temp->data;

    stack->top = stack->top->next;

    free(temp);
    return data;
}

void *peekStack(Stack *stack) {
    if (isStackEmpty(stack)) {
        return NULL;
    }

    return stack->top->data;
}

/* ================= UTIL ================= */

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void lerTexto(char texto[], int tamanho) {
    fgets(texto, tamanho, stdin);
    texto[strcspn(texto, "\n")] = '\0';
}

/* ================= PACIENTE ================= */

void inicializarPaciente(Paciente *paciente, int id, char nome[]) {
    paciente->id = id;
    strcpy(paciente->nome, nome);
    strcpy(paciente->sintomas, "");
    paciente->emergencia = 0;
    paciente->totalConsultas = 0;
}

Paciente *buscarPaciente(Paciente pacientes[], int totalPacientes, int id) {
    int i;

    for (i = 0; i < totalPacientes; i++) {
        if (pacientes[i].id == id) {
            return &pacientes[i];
        }
    }

    return NULL;
}

void registrarConsulta(Paciente *paciente, char descricao[]) {
    int i;

    if (paciente->totalConsultas < MAX_HISTORICO) {
        strcpy(paciente->historico[paciente->totalConsultas].descricao, descricao);
        paciente->totalConsultas++;
    } else {
        for (i = 0; i < MAX_HISTORICO - 1; i++) {
            paciente->historico[i] = paciente->historico[i + 1];
        }

        strcpy(paciente->historico[MAX_HISTORICO - 1].descricao, descricao);
    }
}

void mostrarHistoricoPaciente(Paciente *paciente) {
    int i;

    printf("\nFicha de %s - ultimas %d consultas:\n", paciente->nome, MAX_HISTORICO);

    if (paciente->totalConsultas == 0) {
        printf("Nenhuma consulta registrada.\n");
        return;
    }

    for (i = 0; i < paciente->totalConsultas; i++) {
        printf("%d. %s\n", i + 1, paciente->historico[i].descricao);
    }
}

void listarPacientes(Paciente pacientes[], int totalPacientes) {
    int i;

    if (totalPacientes == 0) {
        printf("\nNenhum paciente cadastrado.\n");
        return;
    }

    printf("\nPacientes cadastrados:\n");

    for (i = 0; i < totalPacientes; i++) {
        printf("ID: %d | Nome: %s\n", pacientes[i].id, pacientes[i].nome);
    }
}

/* ================= SISTEMA BLUA ================= */

void solicitarTeleconsulta(Queue *filaNormal, Queue *filaEmergencia, Paciente *paciente) {
    printf("Digite os sintomas do paciente: ");
    lerTexto(paciente->sintomas, 120);

    printf("E emergencia? (1 = sim / 0 = nao): ");
    scanf("%d", &paciente->emergencia);
    limparBuffer();

    if (paciente->emergencia == 1) {
        enqueue(filaEmergencia, paciente);
        printf("Paciente %s entrou na fila de EMERGENCIA.\n", paciente->nome);
    } else {
        enqueue(filaNormal, paciente);
        printf("Paciente %s entrou na fila normal.\n", paciente->nome);
    }
}

void mostrarProximoPaciente(Queue *filaNormal, Queue *filaEmergencia) {
    Paciente *paciente;

    if (!isQueueEmpty(filaEmergencia)) {
        paciente = (Paciente *) peekQueue(filaEmergencia);
        printf("\nProximo paciente: %s [EMERGENCIA]\n", paciente->nome);
    } else if (!isQueueEmpty(filaNormal)) {
        paciente = (Paciente *) peekQueue(filaNormal);
        printf("\nProximo paciente: %s [NORMAL]\n", paciente->nome);
    } else {
        printf("\nNao ha pacientes aguardando atendimento.\n");
    }
}

void atenderProximoPaciente(Queue *filaNormal, Queue *filaEmergencia) {
    Paciente *paciente;
    char descricao[250];

    if (!isQueueEmpty(filaEmergencia)) {
        paciente = (Paciente *) dequeue(filaEmergencia);
        printf("\nAtendimento prioritario iniciado para: %s\n", paciente->nome);
    } else if (!isQueueEmpty(filaNormal)) {
        paciente = (Paciente *) dequeue(filaNormal);
        printf("\nAtendimento normal iniciado para: %s\n", paciente->nome);
    } else {
        printf("\nNao ha pacientes aguardando atendimento.\n");
        return;
    }

    printf("Sintomas avaliados: %s\n", paciente->sintomas);

    sprintf(descricao, "Consulta finalizada. Sintomas avaliados: %s", paciente->sintomas);
    registrarConsulta(paciente, descricao);

    printf("Consulta registrada na ficha individual de %s.\n", paciente->nome);
}

void registrarAcaoAdministrativa(Stack *pilhaAcoes) {
    char descricao[150];
    char *acao;

    printf("Digite a acao administrativa: ");
    lerTexto(descricao, 150);

    acao = (char *) malloc(strlen(descricao) + 1);

    if (acao == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    strcpy(acao, descricao);
    push(pilhaAcoes, acao);

    printf("Acao registrada: %s\n", acao);
}

void desfazerUltimaAcao(Stack *pilhaAcoes) {
    char *acao = (char *) pop(pilhaAcoes);

    if (acao == NULL) {
        printf("\nNenhuma acao administrativa para desfazer.\n");
        return;
    }

    printf("\nAcao desfeita: %s\n", acao);
    free(acao);
}

/* ================= MAIN ================= */

int main() {
    Queue filaNormal;
    Queue filaEmergencia;
    Stack pilhaAcoes;

    Paciente pacientes[MAX_PACIENTES];

    int totalPacientes = 0;
    int opcao;
    int idBusca;
    Paciente *paciente;
    char nome[50];

    initQueue(&filaNormal);
    initQueue(&filaEmergencia);
    initStack(&pilhaAcoes);

    do {
        printf("\n===== SISTEMA BLUA =====\n");
        printf("1. Cadastrar paciente\n");
        printf("2. Solicitar teleconsulta\n");
        printf("3. Mostrar proximo paciente\n");
        printf("4. Atender proximo paciente\n");
        printf("5. Mostrar historico de paciente\n");
        printf("6. Registrar acao administrativa\n");
        printf("7. Desfazer ultima acao administrativa\n");
        printf("8. Listar pacientes\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                if (totalPacientes >= MAX_PACIENTES) {
                    printf("Limite de pacientes atingido.\n");
                    break;
                }

                printf("Digite o nome do paciente: ");
                lerTexto(nome, 50);

                inicializarPaciente(&pacientes[totalPacientes], totalPacientes + 1, nome);

                printf("Paciente cadastrado com ID %d.\n", totalPacientes + 1);

                totalPacientes++;
                break;

            case 2:
                printf("Digite o ID do paciente: ");
                scanf("%d", &idBusca);
                limparBuffer();

                paciente = buscarPaciente(pacientes, totalPacientes, idBusca);

                if (paciente == NULL) {
                    printf("Paciente nao encontrado.\n");
                } else {
                    solicitarTeleconsulta(&filaNormal, &filaEmergencia, paciente);
                }
                break;

            case 3:
                mostrarProximoPaciente(&filaNormal, &filaEmergencia);
                break;

            case 4:
                atenderProximoPaciente(&filaNormal, &filaEmergencia);
                break;

            case 5:
                printf("Digite o ID do paciente: ");
                scanf("%d", &idBusca);
                limparBuffer();

                paciente = buscarPaciente(pacientes, totalPacientes, idBusca);

                if (paciente == NULL) {
                    printf("Paciente nao encontrado.\n");
                } else {
                    mostrarHistoricoPaciente(paciente);
                }
                break;

            case 6:
                registrarAcaoAdministrativa(&pilhaAcoes);
                break;

            case 7:
                desfazerUltimaAcao(&pilhaAcoes);
                break;

            case 8:
                listarPacientes(pacientes, totalPacientes);
                break;

            case 0:
                printf("Encerrando sistema.\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    return 0;
}