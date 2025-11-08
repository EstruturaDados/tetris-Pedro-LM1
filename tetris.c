#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define FILA_CAP 5
#define PILHA_CAP 3

typedef struct {
    char tipo; // 'I','O','T','L'
    int id;
} Peca;

//Variáveis da fila circular
Peca fila[FILA_CAP];
int frente = 0;
int tamanhoFila = 0;

// Variáveis da pilha
Peca pilha[PILHA_CAP];
int topo = 0; // número de elementos na pilha

// Contador de id para peças
int proximoId = 1;

// Gera peça aleatória
Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'}; // tipos simples
    p.tipo = tipos[rand() % 4];
    p.id = proximoId++;
    return p;
}

// Funções da fila circular
bool filaVazia() {
    return tamanhoFila == 0;
}

bool filaCheia() {
    return tamanhoFila == FILA_CAP;
}

bool enqueue(Peca p) {
    if (filaCheia()) return false;
    int pos = (frente + tamanhoFila) % FILA_CAP;
    fila[pos] = p;
    tamanhoFila++;
    return true;
}

// remove e retorna a peça da frente (assume verificação externa)
Peca dequeue() {
    Peca p = fila[frente];
    frente = (frente + 1) % FILA_CAP;
    tamanhoFila--;
    return p;
}

// mostra fila em ordem da frente para tras
void mostrarFila() {
    printf("\n--- FILA (frente -> tras) ---\n");
    if (filaVazia()) {
        printf("[vazia]\n");
        return;
    }
    printf("%-6s %-6s\n", "Tipo", "ID");
    for (int i = 0; i < tamanhoFila; i++) {
        int idx = (frente + i) % FILA_CAP;
        printf("%-6c %-6d\n", fila[idx].tipo, fila[idx].id);
    }
}

//--- Funções da pilha
void inicializarPilha() {
    topo = 0;
}

bool pilhaVazia() {
    return topo == 0;
}

bool pilhaCheia() {
    return topo == PILHA_CAP;
}

bool push(Peca p) {
    if (pilhaCheia()) return false;
    pilha[topo] = p;
    topo++;
    return true;
}

Peca pop() {
    topo--;
    return pilha[topo];
}

void mostrarPilha() {
    printf("\n--- PILHA (topo em cima) ---\n");
    if (pilhaVazia()) {
        printf("[vazia]\n");
        return;
    }
    printf("%-6s %-6s\n", "Tipo", "ID");
    for (int i = topo - 1; i >= 0; i--) {
        printf("%-6c %-6d\n", pilha[i].tipo, pilha[i].id);
    }
}

//    Operações de jogo

//joga peça da frente: dequeue e repoe com nova peça gerada
void jogarPeca() {
    if (filaVazia()) {
        printf("Fila vazia, nada para jogar.\n");
        return;
    }
    Peca p = dequeue();
    printf("Você jogou a peça: Tipo %c | ID %d\n", p.tipo, p.id);
    Peca nova = gerarPeca();
    enqueue(nova);
    printf("Nova peça inserida no final: Tipo %c | ID %d\n", nova.tipo, nova.id);
}

//reserva peça da frente: remove da fila e empilha
void reservarPeca() {
    if (filaVazia()) {
        printf("Fila vazia, nada para reservar.\n");
        return;
    }
    if (pilhaCheia()) {
        printf("Pilha de reserva cheia! Não é possível reservar.\n");
        return;
    }
    Peca p = dequeue();
    if (push(p)) {
        printf("Peça reservada: Tipo %c | ID %d\n", p.tipo, p.id);
        Peca nova = gerarPeca();
        enqueue(nova);
        printf("Nova peça inserida no final: Tipo %c | ID %d\n", nova.tipo, nova.id);
    } else {
        printf("Erro ao reservar a peça.\n");
        enqueue(p);
    }
}

void usarPecaReservada() {
    if (pilhaVazia()) {
        printf("Pilha vazia! Não há peça reservada para usar.\n");
        return;
    }
    Peca p = pop();
    printf("Você usou a peça reservada: Tipo %c | ID %d\n", p.tipo, p.id);
}

// troca peça da frente da fila com o topo da pilha
void trocarFrenteComTopo() {
    if (filaVazia()) {
        printf("Fila vazia: impossível trocar.\n");
        return;
    }
    if (pilhaVazia()) {
        printf("Pilha vazia: não há topo para trocar.\n");
        return;
    }
    int idxFrente = frente;
    Peca temp = fila[idxFrente];
    fila[idxFrente] = pilha[topo - 1];
    pilha[topo - 1] = temp;
    printf("Troca realizada entre a frente da fila e o topo da pilha.\n");
}

// troca os 3 primeiros da fila com as 3 peças da pilha
void trocarTresFilaComPilha() {
    if (topo != 3) {
        printf("A pilha precisa ter exatamente 3 peças para esta troca.\n");
        return;
    }
    if (tamanhoFila < 3) {
        printf("A fila precisa ter pelo menos 3 peças para esta troca.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int idxFila = (frente + i) % FILA_CAP;
        int idxPilha = topo - 1 - i;
        Peca temp = fila[idxFila];
        fila[idxFila] = pilha[idxPilha];
        pilha[idxPilha] = temp;
    }
    printf("Troca dos 3 primeiros da fila com as 3 da pilha realizada.\n");
}

// inicializa fila com 5 peças geradas automaticamente
void inicializarFila() {
    frente = 0;
    tamanhoFila = 0;
    for (int i = 0; i < FILA_CAP; i++) {
        Peca p = gerarPeca();
        enqueue(p);
    }
}

//mostra estado completo
void mostrarEstado() {
    mostrarFila();
    mostrarPilha();
}

// menu simples
void mostrarMenu() {
    printf("\n=== TETRIS STACK ===\n");
    printf("1 - Jogar peça (dequeue)\n");
    printf("2 - Reservar peça (enviar da fila para pilha)\n");
    printf("3 - Usar peça reservada (pop)\n");
    printf("4 - Trocar peça da frente com topo da pilha\n");
    printf("5 - Trocar 3 primeiros da fila com 3 da pilha\n");
    printf("0 - Sair\n");
    printf("Escolha uma opcao: ");
}

int main() {
    srand((unsigned int)time(NULL));
    inicializarPilha();
    inicializarFila();

    int opc;
    do {
        mostrarEstado();
        mostrarMenu();
        if (scanf("%d", &opc) != 1) {
            // entrada inválida, limpar stdin
            while (getchar() != '\n');
            printf("Entrada invalida.\n");
            continue;
        }

        while (getchar() != '\n');

        switch (opc) {
            case 1:
                // jogar peça
                if (filaVazia()) {
                    printf("Fila vazia, nao ha como jogar.\n");
                } else {
                    Peca p = dequeue();
                    printf("Jogou a peça: Tipo %c | ID %d\n", p.tipo, p.id);
                    // repor automaticamente
                    Peca nova = gerarPeca();
                    enqueue(nova);
                    printf("Nova peça adicionada no final: Tipo %c | ID %d\n", nova.tipo, nova.id);
                }
                break;
            case 2:
                reservarPeca();
                break;
            case 3:
                usarPecaReservada();
                break;
            case 4:
                trocarFrenteComTopo();
                break;
            case 5:
                trocarTresFilaComPilha();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opc != 0);

    return 0;
}
