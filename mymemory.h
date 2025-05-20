#ifndef MYMEMORY_H
#define MYMEMORY_H

#include <stddef.h>
#include <stdlib.h>

typedef struct allocation {
    void *start;
    size_t size;
    struct allocation *next;
} allocation_t;

typedef struct {
    void *pool; // ponteiro para o bloco de memória real
    size_t total_size;
    allocation_t *head; // ponteiro para lista encadeada
} mymemory_t;

mymemory_t* mymemory_init(size_t size);
void* mymemory_alloc(mymemory_t *memory, size_t size);
void mymemory_free(mymemory_t *memory, void *ptr);
void mymemory_display(mymemory_t *memory);
void mymemory_stats(mymemory_t *memory);
void mymemory_cleanup(mymemory_t *memory);

mymemory_t* mymemory_init(size_t size){
    mymemory_t *memory = malloc(sizeof(mymemory_t)); // criando o struct dinamicamente
    if (!memory) return NULL;

    memory->pool = malloc(size); // alocação do pool de memória

    if (!memory->pool) { // não conseguiu alocar o pool de memória
        free(memory);
        return NULL;
    }

    memory->total_size = size; 
    memory->head = NULL;
    return memory;
}

void* mymemory_alloc(mymemory_t *memory, size_t size){
    if (!memory || size == 0 || size > memory->total_size) {
        return NULL;
    }

    void *pool_start = memory->pool;
    void *pool_end = (char*)memory->pool + memory->total_size; // (char*) para fazer a aritmética de ponteiros

    if (memory->head == NULL) {
        allocation_t *alloc = malloc(sizeof(allocation_t)); // criando o struct dinamicamente
        if (!alloc) return NULL;

        alloc->start = pool_start;
        alloc->size = size;
        alloc->next = NULL;
        memory->head = alloc;
        return alloc->start;
    }

    allocation_t *prev_temp = NULL;
    allocation_t *curr_temp = memory->head;

    void *start_temp = pool_start;
    size_t space = (char*)curr_temp->start - (char*)start_temp; // espaço entre a primeira alocação e o inicio do pool
    
    if (space >= size) { // se couber
        allocation_t *alloc = malloc(sizeof(allocation_t));
        if (!alloc) return NULL;

        alloc->start = start_temp;
        alloc->size = size;
        alloc->next = curr_temp;

        memory->head = alloc;
        return alloc->start;
    }

    while (curr_temp->next) { // percorre a lista encadeada
        prev_temp = curr_temp;
        curr_temp = curr_temp->next;

        start_temp = (char*)prev_temp->start + prev_temp->size; // fim do bloco anterior
        space = (char*)curr_temp->start - (char*)start_temp; // espaço entre o fim do bloco anterior e o inicio bloco atual

        if (space >= size) { // se couber
            allocation_t *alloc = malloc(sizeof(allocation_t));
            if (!alloc) return NULL;

            alloc->start = start_temp;
            alloc->size = size;
            alloc->next = curr_temp;
            prev_temp->next = alloc;

            return alloc->start;
        }
    }

    start_temp = (char*)curr_temp->start + curr_temp->size; // fim do ultimo bloco
    space = (char*)pool_end - (char*)start_temp; // espaço entre o fim do ultimo bloco e o fim do pool

    if (space >= size) { // se couber
        allocation_t *alloc = malloc(sizeof(allocation_t));
        if (!alloc) return NULL;

        alloc->start = start_temp;
        alloc->size = size;
        alloc->next = NULL;
        curr_temp->next = alloc;

        return alloc->start;
    }

    return NULL;
}

void mymemory_display(mymemory_t *memory){
    if (!memory) return;
    allocation_t *curr = memory->head; 
    printf("Lista de alocações:\n");
    while (curr) {
        size_t offset = (char*)curr->start - (char*)memory->pool; // calcula a distancia do início do pool (offset)
        printf("  - Início: %p (offset %zu), Tamanho: %zu bytes\n", curr->start, offset, curr->size);
        curr = curr->next; // passa para o próximo bloco
    }
}

#endif /* MYMEMORY_H */