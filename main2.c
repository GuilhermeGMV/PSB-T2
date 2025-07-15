#include <stdio.h>
#include <stdlib.h>
#include "mymemory.h"
#include "mymemory.c"

int main() {
    printf("Inicializando gerenciador com 1024 bytes (1 KB)...\n");
    mymemory_t *mem = mymemory_init(1024);
    if (!mem) {
        printf("Erro ao inicializar o gerenciador de memória.\n");
        return 1;
    }

    printf("\n=== Teste 1: Alocacoes simples ===\n");
    void *a1 = mymemory_alloc(mem, 100);  // aloca 100 bytes
    void *a2 = mymemory_alloc(mem, 200);  // aloca 200 bytes
    void *a3 = mymemory_alloc(mem, 150);  // aloca 150 bytes
    mymemory_display(mem);
    mymemory_stats(mem);

    printf("\n=== Teste 2: Liberando bloco do meio (fragmentacoo) ===\n");
    mymemory_free(mem, a2);               // libera o bloco de 200 bytes
    mymemory_display(mem);
    mymemory_stats(mem);

    printf("\n=== Teste 3: Alocando bloco menor no espaco liberado ===\n");
    void *a4 = mymemory_alloc(mem, 120);  // tenta ocupar parte do espaço fragmentado
    mymemory_display(mem);
    mymemory_stats(mem);

    printf("\n=== Teste 4: Alocacoes pequenas para criar fragmentacoo ===\n");
    void *a5 = mymemory_alloc(mem, 50);
    void *a6 = mymemory_alloc(mem, 30);
    void *a7 = mymemory_alloc(mem, 60);
    mymemory_display(mem);
    mymemory_stats(mem);

    printf("\n=== Teste 5: Liberando blocos alternados ===\n");
    mymemory_free(mem, a1);  // libera primeiro
    mymemory_free(mem, a6);  // libera meio
    mymemory_display(mem);
    mymemory_stats(mem);

    printf("\n=== Teste 6: Tentando alocar um bloco grande (500 bytes) ===\n");
    void *a8 = mymemory_alloc(mem, 500); // deve falhar se não há espaço contíguo suficiente
    printf("Resultado da alocacao de 500 bytes: %p\n", a8);
    mymemory_display(mem);
    mymemory_stats(mem);

    printf("\n=== Teste 7: Tentando alocar varios blocos pequenos ===\n");
    void *b1 = mymemory_alloc(mem, 40);
    void *b2 = mymemory_alloc(mem, 25);
    void *b3 = mymemory_alloc(mem, 10);
    mymemory_display(mem);
    mymemory_stats(mem);

    printf("\n=== Teste Final: Cleanup ===\n");
    mymemory_cleanup(mem);
    printf("Memoria limpa!\n");

    return 0;
}