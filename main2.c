#include <stdio.h>
#include <stdlib.h>
#include "mymemory.h"

int main() {
    // 100 bytes
    mymemory_t *mem = mymemory_init(100);
    if (!mem) {
        printf("Erro ao inicializar o gerenciador de memória.\n");
        return 1;
    }

    // 20 bytes
    void *ptr1 = mymemory_alloc(mem, 20);
    printf("Alocação 1: %p\n", ptr1);

    // 30 bytes
    void *ptr2 = mymemory_alloc(mem, 30);
    printf("Alocação 2: %p\n", ptr2);

    // 40 bytes
    void *ptr3 = mymemory_alloc(mem, 40);
    printf("Alocação 3: %p\n", ptr3);

    // 20 bytes (deve falhar)
    void *ptr4 = mymemory_alloc(mem, 20);
    printf("Alocação 4 (esperado NULL): %p\n", ptr4);

    mymemory_display(mem);

    // mymemory_cleanup(mem);
    return 0;
}
