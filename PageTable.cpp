#include "PageTable.h"
#include <iostream>
#include <cstdlib>
#include <cstring>


// constructor for level for PageTable Tree
Level::Level(int depth, int entryCount, PageTable *PageTablePtr, bool isLeaf):depth(depth), pageTablePtr(PageTablePtr), isLeaf(isLeaf){

    if (isLeaf){
        // allocating an array of map pointers for leaf nodes
        mapPtr = new Map*[entryCount];
        for (int i = 0; i < entryCount; ++i) 
        mapPtr[i] = nullptr;                //initializing each mapping to null
        nextLevelPtr = nullptr;             // no child level for leaf node
    }
    else {
        // allocating an array of Level pointers for non-leaf nodes
        nextLevelPtr = new Level*[entryCount];
        for (int i = 0; i < entryCount; i++)
        nextLevelPtr[i] = nullptr;          // initializing each next level pointer to null
        mapPtr = nullptr;
    }

};

// constructor for Map
Map::Map() {
    frameNum = -1;              // setting initial frame num to -1 to indicate invalid
    valid = false;              // initially invalid until mapping is added
    dirty = false;              // nothing wrriten 
}

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
