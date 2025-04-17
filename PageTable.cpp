//
// name: Kaylie Pham
// RedID: 828129478
//
// name: Aditya Bhagat
// RedID: 828612974

#include "page_table.h"

Level* create_level(int depth, int entryCount, PageTable* pageTablePtr) {

  Level* newLevel = new Level;   // create new level

  newLevel->depth = depth;
  newLevel->pageTablePtr = pageTablePtr;

  newLevel->nextLevelPtr = nullptr;
  Map* mapPtr = nullptr;

  if (depth < pageTablePtr->levelCount - 1) {
    // newLevel.nextLevelPtr = new Level* [pageTablePtr.entryCount[depth + 1]];
    newLevel->nextLevelPtr = new Level*[entryCount];
    for (int i = 0; i < entryCount; i++) {
      newLevel->nextLevelPtr[i] = nullptr;
    }
  }
   else {
     newLevel->mapPtr = new Map[entryCount];
     for (int i = 0; i < entryCount; i++) {
       newLevel->mapPtr[i] = create_map();
     }
   }

  return newLevel;
}

Map* create_map() {
   Map* newMap = new Map;

   newMap->frameNum = -1;
   newMap->valid = false;
   newMap->dirty = false;
   newMap->VPN = -1;
   newMap->PFN = -1;
   newMap->PTHit = NULL;
   newMap->vpns = nullptr;

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

  int root_depth = 0;
  // create root level with depth 0, entry count for the first level (so index 0), page table pointer
  newPageTable->rootLevel = create_level(root_depth, newPageTable->entryCount[0] newPageTable);

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

void insertVpn2PfnMapping(PageTable *PageTable, unsigned int VPN, int frameNum) {

}

Map* findVpn2PfnMapping(PageTable *PageTable, unsigned int VPN) {


}
