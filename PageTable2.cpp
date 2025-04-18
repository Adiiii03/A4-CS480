#include "PageTable.h"
#include <iostream>
#include <cstdlib>
#include <cstring>


// constructor for level for PageTable Tree
Level* create_level(int depth, int entryCount, PageTable* pageTablePtr){

    Level* newlevel = new Level;

    newlevel->depth = depth;
    newlevel->pageTablePtr = pageTablePtr;

    // for non-leaf nodes, allocating an array of pointers to the next level
    if (depth < pageTablePtr->levelCount - 1){
        newlevel->nextLevelPtr = new Level*[entryCount];
        for (int i = 0; i < entryCount; i++){
            newlevel->nextLevelPtr[i] = nullptr;
        }
        newlevel->mapPtr = nullptr;            // if not used
    }
    // for leaf nodes, allocating an array of map entries
    else {
        newlevel->nextLevelPtr = nullptr;
        newlevel->mapPtr = new Map[entryCount];
        for (int i = 0; i < entryCount; i++) {
            newlevel->mapPtr[i].valid = false;             // initially invalid until mapping is added
            newlevel->mapPtr[i].frameNum = -1;             // setting initial frame num to -1 to indicate invalid
            newlevel->mapPtr[i].dirty = -1;                // nothing wrriten yet
        }
    }
    return newlevel;
};

// constructor for Map
Map* create_map(int frameNum) {
    Map* map = new Map;
    map->frameNum = frameNum;
    map->valid = true;
    return map;
}

// constructor for the Pagetable
PageTable* create_pagetable(int bitsPerLevel[]) {

  PageTable* newPageTable = new PageTable;  // create new page table

  newPageTable->levelCount = sizeof(bitsPerLevel) / sizeof(bitsPerLevel[0]); // store number of levels

  // store count of bits for offset; offset = 32 - (number of bits for each level)
  newPageTable->numOfBitsOffset = 32;   // assuming the address has 32 bits, initialize to 32 and decrement from there
  for (int i = 0; i < bitsPerLevel[0]; i++) {
    newPageTable->numOfBitsOffset -= bitsPerLevel[i];
  }

  // create array of bitmasks corresponding to level i
  newPageTable->bitMaskAry = new int[newPageTable->levelCount];
  for (int i = 0; i < newPageTable->levelCount; i++) {
    newPageTable->bitMaskAry[i];
  }

  // create array of bits to shift for getting level i page index
  newPageTable->shiftAry = new int[newPageTable->levelCount];
  int shiftAryAmt = 32;   // assuming the address has 32 bits, initialize to 32 and decrement from there
  for (int i = 0; i < newPageTable->levelCount; i++) {
    shiftAryAmt -= bitsPerLevel[i];
    newPageTable->shiftAry[i] = shiftAryAmt;
  }

  // create array of # of next level entries for level i
       // for each level, the entry count is 2^(number of bits for level i)
  newPageTable->entryCount = new int[newPageTable->levelCount];
  int base = 2;    // base is 2 because two binary values: 0 and 1
  int exponent = 0;
  for (int i = 0; i < newPageTable->levelCount; i++) {
    exponent = bitsPerLevel[i];  // exponent = number of bits for level i
    newPageTable->entryCount[i] = std::pow(base, exponent);
  }

  // create root level with depth 0, entry count for the first level (so index 0), page table pointer
  newPageTable->rootLevel = create_level(0, newPageTable->entryCount[0], newPageTable);

  // additional variable for summary
  exponent = newPageTable->numOfBitsOffset;
  int pageSize = std::pow(base, exponent);               // bytes per page = 2^(offset)

  // all initialized to 0 because no addresses have been processed, replaced, mapped, allocated
  newPageTable->numOfPageReplaced = 0;      // number of page replacement
  newPageTable->pageTableHits = 0;          // number of times a virtual page was mapped
  newPageTable->numOfAddress = 0;           // number of addresses processed
  newPageTable->numOfFramesAllocated = 0;   // number of frames allocated
  newPageTable->pgTableEntries = 0;         // total number of page table entries acrosss all levels

  return newPageTable;

}

unsigned int getVPNFromVirtualAddress(unsigned int virtualAdd, unsigned int mask, unsigned int shift) {

  int page;   // stores the VPN of any level or full VPN, depending on given mask or shift amount

  page = virtualAdd & mask;  // extract the relevant bits with bitwise and
  page >>= shift;            // shift right by given shift amount

  return page;
}

// Inserting VPN -> PFN mapping to pagetable
void insertVpn2PfnMapping(PageTable *PageTable, unsigned int VPN, int frameNum) {
    // starting from root level
    Level* currLevel = PageTable->rootLevel;

    // traversing thru each level in pagetable
    for (int i=0; i < PageTable->levelCount; ++i){              // i: current depth in the page table (0 = root level)
        int index = getVPNFromVirtualAddress(VPN, PageTable->bitMaskAry[i], PageTable->shiftAry[i]);            // index: position within this level’s array derived from VPN bits for this level

    // when leaf level reached

        if ((i == PageTable->levelCount - 1)){                            
            currLevel->mapPtr[index].frameNum = frameNum;           // assigning PFN
            currLevel->mapPtr[index].valid = (frameNum >= 0);       // setting validity
            return;
            }
        

        // creating next level nodes for non-leaf level if it doesn't exist
        if (currLevel->nextLevelPtr[index] == nullptr){
            currLevel-> nextLevelPtr[index] = create_level(i+1, PageTable->entryCount[i+1], PageTable);
            PageTable->pgTableEntries++;                            // incrementing pagetable entry count
        }
        currLevel= currLevel->nextLevelPtr[index];                  // moving to next level
    }

}

// looks up the VPN in the pagetable and returns the Map only if valid
Map* findVpn2PfnMapping(PageTable* PageTable, unsigned int VPN){
    // starting at thew root node
    Level* currLevel = PageTable->rootLevel;    

    // traversing thru levels in pagetable
    for (int i=0; i < PageTable->levelCount; ++i){          
        int index = getVPNFromVirtualAddress(VPN, PageTable->bitMaskAry[i], PageTable->shiftAry[i]);        // extractingn the index into that level's array

        //if at leaf level and its valid flag, return the map
        if (i == PageTable->levelCount - 1){
            if (!currLevel->mapPtr[index].valid){        // check if the map entry is valid
            return nullptr;                             // if not return nullptr
            }
            return &currLevel->mapPtr[index];           // if valid, return the pointer to Map object with PFN
        }

        // moving to next level if exists, else return null
        if (currLevel->nextLevelPtr[index] == nullptr){         //check if valid mapping found
            return nullptr;                                     // returning nullptr if not
        }
        // if found valid mapping, go to next level
        currLevel= currLevel->nextLevelPtr[index];
    }

}
