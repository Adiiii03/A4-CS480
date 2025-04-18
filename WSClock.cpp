#include "WSClock.h"

// creating a WSClockEntry and initiliazing it
WSClockEntry create_WSClock_entry(unsigned int vpn, int frameNum, int lastAccessTime, bool dirty) {
    WSClockEntry entry;
    entry.vpn = vpn;
    entry.frameNum = frameNum;
    entry.lastUsedTime = lastAccessTime;
    entry.dirty = dirty;
    return entry;
}

// WSClock page replacement function
int page_replacement(std::vector<WSClockEntry>& clock, int& clockHand, int ageThreshold,int currentTime, unsigned int& oldVPN) {
    int n = clock.size();
    int start = clockHand;

    while (true) {
        WSClockEntry& entry = clock[clockHand];

        bool isOld = (currentTime - entry.lastUsedTime) > ageThreshold;

        if (isOld && !entry.dirty) {
            // Found a victim
            oldVPN = entry.vpn;
            int frame = entry.frameNum;
            clock[clockHand] = create_WSClock_entry(0, frame, currentTime, false); // placeholder entry for new insert
            clockHand = (clockHand + 1) % n;
            return frame;
        } else if (entry.dirty) {
            // Schedule write to disk
            entry.dirty = false; // simulate clearing dirty bit
        }

        clockHand = (clockHand + 1) % n;

        if (clockHand == start) {
            // If we made a full circle and found nothing, evict current entry
            WSClockEntry& victim = clock[clockHand];
            oldVPN = victim.vpn;
            int frame = victim.frameNum;
            clock[clockHand] = create_WSClock_entry(0, frame, currentTime, false);
            clockHand = (clockHand + 1) % n;
            return frame;
        }
    }
}
