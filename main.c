#include <stdio.h>
#include <stdlib.h>
#include "mymemory.h"

mymemory_t *mem = NULL;  // Variável global para facilitar os testes

void teste1_alocacoes_simples() {
    printf("\n=== Teste 1: Alocacoes simples ===\n");
    void *bloco1 = mymemory_alloc(mem, 100);
    void *bloco2 = mymemory_alloc(mem, 200);
    void *bloco3 = mymemory_alloc(mem, 150);

    mymemory_display(mem);
    mymemory_stats(mem);
}

void teste2_liberar_meio() {
    printf("\n=== Teste 2: Liberando bloco do meio ===\n");
    // Tenta liberar um bloco de 200 bytes (offset 100)
    void *temp = mymemory_alloc(mem, 100);
    void *liberavel = mymemory_alloc(mem, 200);
    void *temp2 = mymemory_alloc(mem, 150);

    mymemory_free(mem, liberavel);

    mymemory_display(mem);
    mymemory_stats(mem);
}

void teste3_reutilizar_espaco() {
    printf("\n=== Teste 3: Reutilizar espaco liberado ===\n");
    void *temp = mymemory_alloc(mem, 100);
    void *vago = mymemory_alloc(mem, 200);
    void *temp2 = mymemory_alloc(mem, 150);
    mymemory_free(mem, vago);

    void *reuso = mymemory_alloc(mem, 120);  // deve reutilizar o espaço liberado

    mymemory_display(mem);
    mymemory_stats(mem);
}

void teste4_fragmentacao_fina() {
    printf("\n=== Teste 4: Fragmentacao fina ===\n");
    mymemory_alloc(mem, 50);
    mymemory_alloc(mem, 30);
    mymemory_alloc(mem, 60);

    mymemory_display(mem);
    mymemory_stats(mem);
}

void teste5_liberar_alternados() {
    printf("\n=== Teste 5: Liberando blocos alternados ===\n");
    void *b1 = mymemory_alloc(mem, 80);
    void *b2 = mymemory_alloc(mem, 120);
    void *b3 = mymemory_alloc(mem, 90);
    void *b4 = mymemory_alloc(mem, 60);

    mymemory_free(mem, b1);
    mymemory_free(mem, b3);

    mymemory_display(mem);
    mymemory_stats(mem);
}

void teste6_bloco_grande() {
    printf("\n=== Teste 6: Tentando alocar um bloco grande ===\n");
    void *big = mymemory_alloc(mem, 500);
    printf("Resultado da alocacao de 500 bytes: %p\n", big);

    mymemory_display(mem);
    mymemory_stats(mem);
}

void teste7_blocos_pequenos() {
    printf("\n=== Teste 7: Tentando alocar blocos pequenos ===\n");
    mymemory_alloc(mem, 40);
    mymemory_alloc(mem, 25);
    mymemory_alloc(mem, 10);

    mymemory_display(mem);
    mymemory_stats(mem);
}

void menu() {
    printf("\n======= MENU DE TESTES =======\n");
    printf("1 - Alocacoes simples\n");
    printf("2 - Liberar bloco do meio\n");
    printf("3 - Reutilizar espaco liberado\n");
    printf("4 - Fragmentacao fina\n");
    printf("5 - Liberar blocos alternados\n");
    printf("6 - Alocar bloco grande (500 bytes)\n");
    printf("7 - Alocar blocos pequenos\n");
    printf("8 - Cleanup e sair\n");
    printf("Escolha uma opcao: ");
}

int main() {
    size_t tamanho_pool = 1024;
    printf("Inicializando gerenciador com %zu bytes...\n", tamanho_pool);
    mem = mymemory_init(tamanho_pool);
    if (!mem) {
        printf("Erro ao inicializar a memória.\n");
        return 1;
    }

    int opcao;
    do {
        menu();
        scanf("%d", &opcao);
        switch (opcao) {
            case 1: teste1_alocacoes_simples(); break;
            case 2: teste2_liberar_meio(); break;
            case 3: teste3_reutilizar_espaco(); break;
            case 4: teste4_fragmentacao_fina(); break;
            case 5: teste5_liberar_alternados(); break;
            case 6: teste6_bloco_grande(); break;
            case 7: teste7_blocos_pequenos(); break;
            case 8: 
                mymemory_cleanup(mem); 
                printf("Memória limpa. Encerrando.\n"); 
                break;
            default: printf("Opcao invalida!\n"); break;
        }
    } while (opcao != 8);

    return 0;
}