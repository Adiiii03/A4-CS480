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

// constructor for the Pagetable
PageTable::PageTable(int bitPerLevel[], int levelCount){
    this->levelCount = levelCount;
};
