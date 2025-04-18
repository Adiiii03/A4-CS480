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

void page_replacement(WSClockEntry** entries, int clock_hand_position, int ageThreshold, int currentTime, unsigned int newVpn, PageTable* PageTable){


    bool validFrame = false;             // initializing validFrame to false


    while(!validFrame) {
        WSClockEntry* entry = entries[clock_hand_position];

        int ageOfLastAccessConsideredRecent = currentTime - (entry->lastUsedTime);

        //checking if the age of the page is greater than threshold
        if (ageOfLastAccessConsideredRecent > ageThreshold){

            // checking if the page is clean or not
            if (entry->dirty == false){
                // invalidating the current vpn
                unsigned int evictedVPN = entry->vpn;               // evictedVPN: setting with vpn of the current evicted page
                Map* victimMap = findVpn2PfnMapping(PageTable, evictedVPN);

                if (victimMap != nullptr) {
                    victimMap->valid = false;
                }

                // updating WSClock entry with new vpn information
                entry->vpn = newVpn;
                entry->lastUsedTime = currentTime;
                entry->dirty = false;

                // inserting the new vpn mapping to pagetable
                insertVpn2PfnMapping(PageTable, newVpn, entry->frameNum);

                // franeNum for the victim page
                int victimFrame = entry->frameNum;

        }
    }
        else{
            // clearing the dirty flag
            entry->dirty = false;
        }

    }
    // advancing clock_hand_position
    clock_hand_position = clock_hand_position + 1;
}
