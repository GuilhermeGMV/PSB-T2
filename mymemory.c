#include "mymemory.h"

mymemory_t* mymemory_init(size_t size)
{
    mymemory_t *memory = malloc(sizeof(mymemory_t));
    if (!memory) return NULL;

    memory->pool = malloc(size);
    if (!memory->pool)
    {         
        free(memory);
        return NULL;
    }
    memory->total_size = size;
    memory->head = NULL;
    return memory;
}


void* mymemory_alloc(mymemory_t *memory, size_t size){
    if (!memory || size > memory->total_size || size == 0) 
    {
        return NULL;
    }
    void *pool_start = memory->pool;
    void *pool_end = (char*)memory->pool + memory->total_size;

    // Caso seja a primeira alocação
    if (memory->head == NULL) 
    {
        allocation_t *alloc = malloc(sizeof(allocation_t));
        if (!alloc) return NULL;
        alloc->start = pool_start;
        alloc->size = size;
        alloc->next = NULL;
        memory->head = alloc;
        return alloc->start;
    }

    // Verifica espaço antes da primeira alocação
    allocation_t *prev_temp = NULL;
    allocation_t *curr_temp = memory->head;
    void *start_temp = pool_start;
    size_t space = (char*)curr_temp->start - (char*)start_temp;

    if (space >= size) 
    {
        allocation_t *alloc = malloc(sizeof(allocation_t));
        if (!alloc) return NULL;
        alloc->start = start_temp;
        alloc->size = size;
        alloc->next = curr_temp;
        memory->head = alloc;
        return alloc->start;
    }

    // Percorre a lista encadeada procurando espaço entre alocações
    while (curr_temp->next) 
    {
        prev_temp = curr_temp;
        curr_temp = curr_temp->next;

        start_temp = (char*)prev_temp->start + prev_temp->size;
        space = (char*)curr_temp->start - (char*)start_temp;

        if (space >= size) 
        {
            allocation_t *alloc = malloc(sizeof(allocation_t));
            if (!alloc) return NULL;
            alloc->start = start_temp;
            alloc->size = size;
            alloc->next = curr_temp;
            prev_temp->next = alloc;
            return alloc->start;
        }
    }

    // Verifica espaço após a última alocação
    start_temp = (char*)curr_temp->start + curr_temp->size;
    space = (char*)pool_end - (char*)start_temp;

    if (space >= size) 
    {
        allocation_t *alloc = malloc(sizeof(allocation_t));
        if (!alloc) return NULL;
        alloc->start = start_temp;
        alloc->size = size;
        alloc->next = NULL;
        curr_temp->next = alloc;
        return alloc->start;
    }

    return NULL; // Sem espaço disponível
}

// Exibe todas as alocações realizadas
void mymemory_display(mymemory_t *memory)
{
    if (!memory) return;
    allocation_t *curr = memory->head;
    printf("\nLista de alocacoes:\n");
    while (curr) 
    {
        size_t offset = (char*)curr->start - (char*)memory->pool;
        printf("-Inicio: %p (offset %zu), Tamanho: %zu bytes\n", curr->start, offset, curr->size);
        curr = curr->next;
    }
    printf("\n");
}

// Libera um bloco de memória previamente alocado
void mymemory_free(mymemory_t *memory, void *ptr) 
{
    if (!memory || !ptr || !memory->head) return;

    allocation_t *curr = memory->head;
    allocation_t *prev = NULL;

    while (curr) 
    {
        if (curr->start == ptr) 
        {
            if (prev == NULL) 
            {
                memory->head = curr->next; // Era o primeiro da lista
            } else 
            {
                prev->next = curr->next;   // Remove do meio ou fim
            }
            free(curr); // Libera o nó da lista
            return;
        }

        prev = curr;
        curr = curr->next;
    }
}

// Exibe estatísticas da memória: uso, fragmentação etc.
void mymemory_stats(mymemory_t *memory) 
{
    if (!memory) 
    {
    return;
    }
    allocation_t *curr = memory->head;
    size_t total_allocs = 0;
    size_t total_allocated = 0;
    size_t total_free = 0;
    size_t largest_free_block = 0;
    size_t fragment_count = 0;

    char *start = (char *)memory->pool;
    char *end = start + memory->total_size;

    while (curr) 
    {
        total_allocs++;
        total_allocated += curr->size;

        char *curr_start = (char *)curr->start;
        size_t gap = curr_start - start;

        if (gap > 0) 
        {
            total_free += gap;
            fragment_count++;
            if (gap > largest_free_block) largest_free_block = gap;
        }

        start = curr_start + curr->size;
        curr = curr->next;
    }

    // Verifica espaço livre no final do pool
    if (start < end) 
    {
        size_t gap = end - start;
        total_free += gap;
        fragment_count++;
        if (gap > largest_free_block) largest_free_block = gap;
    }

    printf("Estatisticas de memoria:\n");
    printf("-Total de alocacoes: %zu\n", total_allocs);
    printf("-Memoria alocada: %zu bytes\n", total_allocated);
    printf("-Memoria livre: %zu bytes\n", total_free);
    printf("-Maior bloco livre: %zu bytes\n", largest_free_block);
    printf("-Fragmentos livres: %zu\n", fragment_count);
}

// Libera toda a memória (alocações + pool) e zera tudo
void mymemory_cleanup(mymemory_t *memory) 
{
    if (!memory) return;

    allocation_t *curr = memory->head;
    int count = 0;

    while (curr) 
    {
        allocation_t *next = curr->next;
        curr->start = NULL;
        curr->size = 0;
        curr->next = NULL;
        free(curr);  // Libera o nó da alocação
        curr = next;
        count++;
    }

    // Zera e libera o pool de memória
    if (memory->pool) 
    {
        for (size_t i = 0; i < memory->total_size; i++) 
        {
            ((char *)memory->pool)[i] = 0;
        }
        free(memory->pool);
        memory->pool = NULL;
    }

    memory->head = NULL;
    memory->total_size = 0;

    free(memory); // Libera a estrutura principal
    printf("Total de blocos liberados (com limpeza manual): %d\n", count);
}