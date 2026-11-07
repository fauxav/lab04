#include <stdio.h>                                   // for printf

#define PAGE_SIZE 256                                // each page/frame is 256 bytes
#define NUM_PAGES 4                                  // 4 virtual pages per process
#define NUM_FRAMES 8                                 // 8 physical frames total

typedef struct {
    int frameNumber;                                 // which frame the page lives in (-1 = none)
    int valid;                                       // 1 = page belongs to the process
    int present;                                     // 1 = page is actually in memory rn
} PageTableEntry;

typedef struct {
    PageTableEntry entries[NUM_PAGES];               // one entry per virtual page
} PageTable;

int translate(PageTable *table, int virtualAddress) {
    if (virtualAddress < 0 ||                        // negative addr = not real
        virtualAddress >= NUM_PAGES * PAGE_SIZE) {   // past 1023 = out of range
        printf("VA %d: invalid virtual address\n",
               virtualAddress);
        return -1;
    }

    int pageNum = virtualAddress / PAGE_SIZE;        // which page (floor division)
    int offset = virtualAddress % PAGE_SIZE;         // spot inside the page
    PageTableEntry *entry = &table->entries[pageNum]; // grab page's entry

    if (!entry->valid) {                             // page isnt the process's is not allowed
        printf("VA %d: invalid access (page %d)\n",
               virtualAddress, pageNum);
        return -1;
    }
    if (!entry->present) {                           // valid but not loaded goes to page fault
        printf("VA %d: page fault (page %d not present)\n",
               virtualAddress, pageNum);
        return -1;
    }
    if (entry->frameNumber < 0 ||                    // sanity check the frame #
        entry->frameNumber >= NUM_FRAMES) {
        printf("Page-table error: invalid frame\n");
        return -1;
    }

    int physicalAddress =
        entry->frameNumber * PAGE_SIZE + offset;     // frame start + offset
    printf("VA %d -> page %d, frame %d, offset %d -> PA %d\n",
           virtualAddress, pageNum, entry->frameNumber,
           offset, physicalAddress);
    return physicalAddress;
}

int main(void) {
    PageTable processA = {{                          // {frame, valid, present}
        { 3, 1, 1 }, { 5, 1, 1 },                    // pages 0,1 loaded in frames 3,5
        {-1, 1, 0 }, {-1, 0, 0 }                     // page 2 valid but not loaded, page 3 invalid
    }};
    PageTable processB = {{                          // own table, separate mappings from A
        { 6, 1, 1 }, { 2, 1, 1 },                    // pages 0,1 in frames 6,2
        { 4, 1, 1 }, {-1, 0, 0 }                     // page 2 in frame 4, page 3 invalid
    }};

    printf("Process A, VA 300:\n");
    translate(&processA, 300);                       // page 1 to frame 5
    printf("Process B, VA 300:\n");
    translate(&processB, 300);                       // same VA but page 1 to frame 2 here
    printf("Process A, VA 600:\n");
    translate(&processA, 600);                       // page 2 not present to page fault
    printf("Process A, VA 800:\n");
    translate(&processA, 800);                       // page 3 not valid to invalid access

    // Add the page-loading simulation here in Step 3.
    processA.entries[2].frameNumber = 7;             // os loads page 2 into frame 7
    processA.entries[2].present = 1;                 // mark page 2 as in memory now
    translate(&processA, 600);
    processB.entries[1].frameNumber = 5;
    translate(&processB, 300);
                          
    return 0;
}