#include "WSClock.h"

WSClockEntry* create_WSClock_entry(unsigned int vpn, int frameNum, int lastAccessTime, bool dirty) {

    // create new WSClock entry and initialize attributes 
    WSClockEntry* WSClock_entry = new WSClockEntry;
    WSClock_entry->vpn = vpn;
    WSClock_entry->frameNum = frameNum;
    WSClock_entry->lastUsedTime = lastAccessTime;
    WSClock_entry->dirty = dirty;
    return WSClock_entry;
}

void page_replacement(WSClockEntry* entries, int clock_hand_position){

    
}
