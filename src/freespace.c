#include <stdio.h>                                   
#include <stdlib.h>                                  

// Free block structure definition
typedef struct FreeBlock {
    int size;                                        // how many bytes are free in this block
    struct FreeBlock* next;                          // pointer to the next free block (NULL if last)
} FreeBlock;

// Free list structure definition
typedef struct {
    FreeBlock* head;                                 // first free block in the list
} FreeList;

// Function to print the free list
void printFreeList(FreeList* list) {
    FreeBlock* temp = list->head;                    // start at the front
    printf("Free list: ");
    while (temp != NULL) {                           // go til we fall off the end
        printf("%d -> ", temp->size);                // print this block's size
        temp = temp->next;                           // move to the next one
    }
    printf("NULL\n");                                // show where the list ends
}

// Function to deallocate (add a free block to the end of the list)
void deallocate(FreeList* list, int size) {
    FreeBlock* newBlock = (FreeBlock*)malloc(sizeof(FreeBlock)); // make space for the node
    FreeBlock* temp = list->head;                    // start walking from the head
    newBlock->size = size;                           // store the block size
    newBlock->next = NULL;                           // its gonna be last so next is NULL

    if (list->head == NULL) {
        list->head = newBlock;                       // list is empty so it becomes the head
    } else {
        while (temp->next != NULL) {                 // keep going til the last block
            temp = temp->next;
        }
        temp->next = newBlock;                       // add it at the end
    }
    printf("Deallocated %d\n", size);
}

// Function to allocate (take memory from the first block big enough)
void allocate(FreeList* list, int size) {
    FreeBlock *temp = list->head, *prev = NULL;      // temp = current, prev = the one behind it

    // Search for the first block big enough
    while (temp != NULL && temp->size < size) {
        prev = temp;                                 // remember the block before
        temp = temp->next;                           // step forward
    }

    // If no block was big enough
    if (temp == NULL) {
        printf("Allocate %d failed, no block big enough\n", size);
        return;
    }

    if (temp->size == size) {                        // exact fit -> remove the block
        if (prev == NULL) {
            list->head = temp->next;                 // it was the head so move head up
        } else {
            prev->next = temp->next;                 // skip over temp
        }
        free(temp);                                  // free the node since its used up
    } else {
        temp->size -= size;                          // bigger -> just shrink it
    }
    printf("Allocated %d\n", size);
}

int main() {
    FreeList list = { NULL };                        // start with an empty free list

    deallocate(&list, 100);                          // add some free blocks
    deallocate(&list, 300);
    deallocate(&list, 200);
    printFreeList(&list);

    allocate(&list, 150);                            // first fit is the 300 block -> 150 left
    printFreeList(&list);

    allocate(&list, 100);                            // exact fit -> 100 block gets removed
    printFreeList(&list);

    allocate(&list, 500);                            // nothing that big -> fails
    printFreeList(&list);

    deallocate(&list, 250);                          // give a block back
    printFreeList(&list);

    // Free the nodes left before quitting
    while (list.head != NULL) {
        FreeBlock* temp = list.head;                 // hold the current head
        list.head = list.head->next;                 // move head up
        free(temp);                                  // free the old head
    }

    return 0;
}