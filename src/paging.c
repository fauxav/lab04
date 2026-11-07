#include <stdio.h>

#define PAGE_SIZE 256
#define NUM_PAGES 4
#define NUM_FRAMES 8
#define VA_BITS 10
#define PA_BITS 11

// Provided helper: show a number as fixed-width binary.
// You do not need to modify or memorize this function.
void printBinary(unsigned int value, int width) {
    for (int bit = width - 1; bit >= 0; bit--) {
        putchar((value & (1u << bit)) ? '1' : '0');
    }
}

typedef struct {
    int frameNumber;
} PageTableEntry;

typedef struct {
    PageTableEntry entries[NUM_PAGES];
} PageTable;

int translate(PageTable *table, int virtualAddress) {
    if (virtualAddress < 0 ||
        virtualAddress >= NUM_PAGES * PAGE_SIZE) {
        printf("VA %d: invalid virtual address\n",
               virtualAddress);
        return -1;
    }

    int pageNum = virtualAddress / PAGE_SIZE;
    int offset = virtualAddress % PAGE_SIZE;
    int frameNum = table->entries[pageNum].frameNumber;

    if (frameNum < 0 || frameNum >= NUM_FRAMES) {
        printf("Page-table error: invalid frame %d\n",
               frameNum);
        return -1;
    }

    int physicalAddress = frameNum * PAGE_SIZE + offset;
    printf("VA %d (hex 0x%03X, binary ",
           virtualAddress, (unsigned int)virtualAddress);
    printBinary((unsigned int)virtualAddress, VA_BITS);
    printf(") -> page %d, offset %d, frame %d\n",
           pageNum, offset, frameNum);
    printf("PA %d (hex 0x%03X, binary ",
           physicalAddress, (unsigned int)physicalAddress);
    printBinary((unsigned int)physicalAddress, PA_BITS);
    printf(")\n");
    return physicalAddress;
}

int main(void) {
    PageTable table = {{{3}, {5}, {7}, {0}}};

    translate(&table, 300);
    translate(&table, 800);
    return 0;
}