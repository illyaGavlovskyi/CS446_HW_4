//Illya Gavlovskyi
//CS446
//HW 4

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

typedef struct _mblock_t {
    struct _mblock_t * prev;
    struct _mblock_t * next;
    size_t size;
    int status;
    void * payload;
} mblock_t;

mlist_t mlist = {NULL};

#define MBLOCK_HEADER_SZ offsetof(mblock_t, payload)

typedef struct _mlist_t { 
    mblock_t * head;
} mlist_t;

void printMemList(const mblock_t* headptr);
void* mymalloc(size_t size);
void myfree(void* ptr);
mblock_t* findLastMemlistBlock();
mblock_t* findFreeBlockOfSize(size_t size);
void splitBlockAtSize(mblock_t* block, size_t newSize);
void coallesceBlockPrev(mblock_t* freedBlock);
void coallesceBlockNext(mblock_t* freedBlock);
mblock_t* growHeapBySize(size_t size);

int main(int argc, char* argv[])
{
    void * p1 = mymalloc(10);
    void * p2 = mymalloc(100);
    void * p3 = mymalloc(200);
    void * p4 = mymalloc(500);
    myfree(p3); p3 = NULL;
    myfree(p2); p2 = NULL;
    void * p5 = mymalloc(150);
    void * p6 = mymalloc(500);
    myfree(p4); p4 = NULL;
    myfree(p5); p5 = NULL;
    myfree(p6); p6 = NULL;
    myfree(p1); p1 = NULL;

    return 0;
}

void printMemList(const mblock_t* head) {
    const mblock_t* p = head;
    size_t i = 0;
    while(p != NULL) {
        printf("[%ld] p: %p\n", i, (void*)p);
        printf("[%ld] p->size: %ld\n", i, p->size);
        printf("[%ld] p->status: %s\n", i, p->status > 0 ? "allocated" : "free");
        printf("[%ld] p->prev: %p\n", i, (void*)p->prev);
        printf("[%ld] p->next: %p\n", i, (void*)p->next);
        printf("___________________________\n");
        ++i;
        p = p->next;
    }
    printf("===========================\n");
}

void* mymalloc(size_t size)
{
    if(size == 0)
    {
        return NULL;
    }
    mblock_t *block = findFreeBlockOfSize(size);
    if(block == NULL)
    {
        block = growHeapBySize(size);
        if(block == NULL)
        {
            return NULL;
        }
    }
    splitBlockAtSize(block, size);
    block->status = 1;
    return block->payload;
}

void myfree(void* ptr)
{
    if(ptr == NULL)
    {
        return;
    }
    mblock_t* block = (mblock_t*)((char*)ptr - MBLOCK_HEADER_SZ);
    block->status = 0;
    coallesceBlockNext(block);
    coallesceBlockPrev(block);
}

mblock_t* findLastMemlistBlock()
{
    mblock_t* current = mlist.head;
    if(current != NULL)
    {
        while(current->next != NULL)
        {
            current = current->next;
        }
    }
    return current; 
}

mblock_t* findFreeBlockOfSize(size_t size)
{

}

void splitBlockAtSize(mblock_t* block, size_t newSize)
{

}

void coallesceBlockPrev(mblock_t* freedBlock)
{
    mblock_t *prev = freedBlock->prev;
    mblock_t *next = freedBlock->next;
    if(prev != NULL && prev->status == 0)
    {
        prev->size = prev->size + MBLOCK_HEADER_SZ + freedBlock->size;
        prev->next = next;
        if(next == NULL)
        {
            return;
        }
        next->prev = prev;
    }
}

void coallesceBlockNext(mblock_t* freedBlock)
{
    mblock_t *next = freedBlock->next;
    if(next != NULL && next->status == 0)
    {
        freedBlock->size = freedBlock->size + MBLOCK_HEADER_SZ + next->size;
        freedBlock->next = next->next;
        if(next->next == NULL)
        {
            return;
        }
        next->next->prev = freedBlock;
    }
}

mblock_t* growHeapBySize(size_t size)
{
    size_t totalSize = size + MBLOCK_HEADER_SZ;
    mblock_t* newBlock = (mblock_t*)sbrk(totalSize);
    if(newBlock == (void*) -1) {
        return NULL;
    }
    newBlock->size = size - MBLOCK_HEADER_SZ;
    newBlock->status = 0;
    newBlock->payload = (void *)((char *)newBlock + MBLOCK_HEADER_SZ);
    newBlock->next = NULL;
    mblock_t* last = findLastMemlistBlock();
    newBlock->prev = last;
    if (mlist.head == NULL)
    {
        mlist.head = newBlock;
    } 
    else
    {
        last->next = newBlock;
    }
}

