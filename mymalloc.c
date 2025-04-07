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

}

void myfree(void* ptr)
{

}

mblock_t* findLastMemlistBlock()
{

}

mblock_t* findFreeBlockOfSize(size_t size)
{

}

void splitBlockAtSize(mblock_t* block, size_t newSize)
{

}

void coallesceBlockPrev(mblock_t* freedBlock)
{

}

void coallesceBlockNext(mblock_t* freedBlock)
{
    
}

mblock_t* growHeapBySize(size_t size)
{
    
}

