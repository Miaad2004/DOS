#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

// On demand paging with LRU replacement policy and virtual memory
#define PAGE_SIZE 4096
#define NUM_PAGES 1024

typedef struct
{
    int valid; // In physical mem?
    int frame_number;
    void *virtual_addr;
} PTE;

static struct
{
    PTE page_table[NUM_PAGES];
    void *physical_mem;
    int *frame_bitmap; // track free frames
    int num_frames;
} mmu;

void mem_init()
{
    mmu.physical_mem = malloc(PAGE_SIZE * NUM_PAGES);
    if (mmu.physical_mem == NULL)
    {
        fprintf(stderr, "Failed to allocate physical memory\n");
        exit(EXIT_FAILURE);
    }

    mmu.num_frames = NUM_PAGES;
    mmu.frame_bitmap = calloc(NUM_PAGES, sizeof(int));
    if (mmu.frame_bitmap == NULL)
    {
        fprintf(stderr, "Failed to allocate frame bitmap\n");
        free(mmu.physical_mem);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NUM_PAGES; i++)
    {
        mmu.page_table[i].valid = 0;
        mmu.page_table[i].frame_number = -1;
        mmu.page_table[i].virtual_addr = NULL;
    }
}


int _count_free_pages()
{
    int count = 0;
    for (int i = 0; i < NUM_PAGES; i++)
    {
        if (!mmu.page_table[i].valid)
            count++;
    }

    return count;
}

void *mem_alloc(size_t size)
{
    int n_req_pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    int n_free_pages = _count_free_pages();
    if (n_free_pages < n_req_pages)
    {
        fprintf(stderr, "Not enough memory\n");
        return NULL;
    }

    void *start_addr = NULL;
    int pages_allocated = 0;

    for (int i = 0; i < NUM_PAGES && pages_allocated < n_req_pages; i++)
    {
        if (!mmu.page_table[i].valid)
        {
            // free page found :)
            // lookin for a free frame...
            int frame = 0;
            for (frame = 0; frame < mmu.num_frames; frame++)
            {
                if (!mmu.frame_bitmap[frame])
                {
                    // free frame found !
                    mmu.frame_bitmap[frame] = 1;
                    break;
                }
            }

            if (frame == mmu.num_frames)
            {
                // to do: implement LRU
                return NULL;
            }
            

            // finally alloc the damn page
            mmu.page_table[i].valid = 1;
            mmu.page_table[i].frame_number = frame;
            mmu.page_table[i].virtual_addr = (void *)((char *)mmu.physical_mem + i * PAGE_SIZE);

            if (pages_allocated == 0)
                start_addr = mmu.page_table[i].virtual_addr;

            pages_allocated++;
        }
    }

    return start_addr;
}