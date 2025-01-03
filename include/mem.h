#ifndef MEM_H
#define MEM_H

#include <stddef.h> // for size_t

/**
 * Initializes the memory management system
 * Sets up physical memory and page tables
 */
void mem_init(void);

/**
 * Allocates memory of specified size
 * @param size Number of bytes to allocate
 * @return Pointer to allocated memory or NULL if allocation fails
 */
void* mem_alloc(size_t size);

#endif /* MEM_H */