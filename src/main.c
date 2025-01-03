#include <stdio.h>
#include <string.h>
#include "mem.h"

#define MAX_COMMAND_LENGTH 256
#define ARRAY_SIZE 1000

int main()
{
    // Initialize memory system
    mem_init();
    
    // Allocate array
    int* arr = (int*)mem_alloc(ARRAY_SIZE * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }
    
    // Write values
    printf("Writing values to array...\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = i * 2;
    }
    
    // Read and verify values
    printf("Reading first 10 values:\n");
    for (int i = 0; i < 10; i++) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }
}