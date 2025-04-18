// old code
// name: Kaylie Pham
// RedID: 828129478
//
// name: Aditya Bhagat
// RedID: 828612974

#include <iostream>
#include <unistd.h>

#include "vaddr_tracereader.h"
#include "page_table.h"
#include "log_helpers.h"


int main(int argc, char* argv[]) {

    int numOfAddresses = 0;         // count of addresses processed
    int frameNum = 0;           // stores physical frame number, which starts at 0 (and will be sequentially incremented)
    p2AddrTr mtrace;    // structure is typedefed in vaddr_tracereader
    unsigned int vAddr;    // unsigned 32-bit integer type
    bool cont_addresses = true;    // if true, continue retrieving addresses
    
    int maxNumMemAccesses = -1;     // only process first N memory accesses, default is all
    int numPhysicalFrames = 999999;     // number of available physical frames, default is 999999
    int ageRecentLastAccess = 10;     // age of last access considered recent for page replacement, default is 10
    const char* logMode = "summary";       // log mode, specifying what to be printed, default is "summary"

    int opt;
    while ((opt = getopt(argc, argv, "n:f:a:l")) != -1) {
        switch (opt) {
            case 'n':
                maxNumMemAccesses = atoi(optarg);
                if (maxNumMemAccesses < 1) {
                  std::cerr << "Number of memory accesses must be a number, greater than 0" << std::endl;
                  return 1;    // exit prgram, error occurred
                  }
            break;
            case 'f':
                numPhysicalFrames = atoi(optarg);
                if (numPhysicalFrames < 1) {
                  std::cerr << "Number of available frames must be a number, greater than 0" << std::endl;
                  return 1;    // exit program, error occurred
                }
            break;
            case 'a':
                ageRecentLastAccess = atoi(optarg);
                if (ageRecentLastAccess < 1) {
                  std::cerr << "Age of last access considered recent must be a number, greater than 0" << std::endl;
                }
            break;
            case 'l':
                 logMode = optarg;
            break;
            default:
                return 1;
        }
    }

    int idx = optind;     // parameter, indexes next element to be processed

    const char* mtrace_file_name = argv[idx];
    idx++;    // increment index to get next argument -- file
    const char* readswrites_file_name = argv[idx];
    idx++;     // increment index to get next argument -- num bits for level(s)

    FILE* mtrace_file = fopen(mtrace_file_name, "r");
    FILE* readswrites_file = fopen(readswrites_file_name, "r");

    if (!mtrace_file) {
        std::cerr << "Unable to open <<trace.tr>>" << std::endl;
             return 1; // exit program, error occurred
    }
    if (!readswrites_file) {
        std::cerr << "Unable to open <<readswrites.tr>>" << std::endl;
        return 1;
    }


    // store number of bits to be used for each level
    int totalNumBitsAllLevels = 0;    // store total number of bits for all levels
    int levelCount = argc - idx;
    int bitsPerLevel[levelCount];    // create array to store bits for each level
    if (idx < argc) {
        for (int i = 0; i < levelCount; i++) {
          bitsPerLevel[i] = atoi(argv[i]);
          totalNumBitsAllLevels += bitsPerLevel[i];
        }
       if (totalNumBitsAllLevels > 28) {
         std::cerr << "Too many bits used in page tables" << std::endl;
         return 1; // exit program, error occurred
       }
    }
    
    PageTable* page_table = create_pagetable(bitsPerLevel);
    
    while (NextAddress(mtrace_file, &mtrace) && cont_addresses) {     // while next address exists...
        vAddr = mtrace.addr;        // ...store next address
        insertVpn2PfnMapping(page_table, vAddr, frameNum);  // insert VPN
        numOfAddresses++;   // increment number of addresses processed

        // if max number of memory accesses is specified (in command line)
                  // and if max number of memory accesses is reached, then exit while loop
        if (maxNumMemAccesses && (numOfAddresses >= maxNumMemAccesses)) {
                cont_addresses = false;
            }
        }

    // print logs given the log mode from command line
    if (logMode == "bitmasks") {    // print bit masks for each level, starting with lowest tree level
        log_bitmasks(page_table->levelCount, page_table->bitMaskAry);
    }
    else if (logMode == "summary") {
        log_summary(page_table->pageSize, page_table->numOfPageReplaced, page_table->pageTableHits,
            page_table->numOfAddresses, page_table->numOfFramesAllocated, page_table->pgTableEntries);
    }

    // close files before exiting
    fclose(mtrace_file);
    fclose(readswrites_file);

    return 0; // exit program, as it successfully complete
}



