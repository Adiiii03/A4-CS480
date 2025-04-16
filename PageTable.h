//
// name: Kaylie Pham
// RedID: 828129478
//
// name: Aditya Bhagat
// RedID: 828612974

#include <iostream>
#include <cstring>


struct Level{
    int Depth;                  // current depth level (root=0)
    PageTable* PageTablePtr;    // pointer back to the parent table
    Level* NextLevelPtr;        // for non-leaf nodes
    Map** MapPtr;               // for leaf nodes
    bool isleaf;                // to check weather the node is a leaf or not

    Level::Level(int Depth, int entryCount, PageTable* PageTablePtr, bool isLeaf);     // constructor for Level


};

struct Map{
    int frameNum;               // the frame number
    bool valid;                 // wether the mapping is valid or not 
    int dirty;                 // to check if the page has written or updated
    int VPN;
    int PFN;
    bool HitorMiss; 
    int vpns[];                 // virtual page number associated with level idx

    Map::Map();                      // constructor
};



struct PageTable{
    int levelCount;             // to track number of levels
    int* bitMaskAry;            // bits used for each level
    int* shiftAry;              // shifts for each level
    int* entryCount;            // total entries for each level
    Level* rootLevel;           // root level for the pagetable tree

    // additional variable for summary
    int pageSize;               // bytes per page 
    int numOfPageReplaced;      // number of page replacement 
    int pageTableHits;          // number of times a virtual page was mapped
    int numOfAddress;           // number of addresses processed
    int numOfFramesAllocated;   // number of frames allocated
    int pgTableEntries;         // total number of page table entries acrosss all levels


    PageTable::PageTable(int bitsPerLevel[]);

    unsigned int getVPNFromVirtualAddress(unsigned int virtualAdd, unsigned int mask, unsigned int shift);

    void insertVpn2PfnMapping(PageTable *PageTable, unsigned int VPN, int frameNum);

    Map* findVpn2PfnMapping(PageTable *PageTable, unsigned int VPN);


};
