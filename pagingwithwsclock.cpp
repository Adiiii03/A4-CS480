#include <iostream>
#include <unistd.h>
#include <fstream>

#include "vaddr.tracereader.h"


int main(int argc, char* argv[]) {

    int numMemAccesses;     // only prpcess first N memory accesses, default is all
    int numPhysicalFrames = 999999;     // number of available physical frames, default is 999999
    int ageRecentLastAccess = 10;     // age of last access considered recent for page replacement, default is 10
    const char* logMode = "summary";       // log mode, specifying what to printed, default is "summary"

    int opt;
    while ((opt = getopt(argc, argv, "n:f:a:l")) != -1) {
        switch (opt) {
            case 'n':
                numMemAccesses = atoi(optarg);
                if (numMemAccesses < 1) {
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

    const char* mem_trace_file = argv[idx];
    idx++;    // increment index to get next argument -- file
    const char* reads_writes_file = argv[idx];
    idx++;     // increment index to get next argument -- num bits for level(s)

    std::ifstream mtrace_file_stream(mem_trace_file); // open memory trace file for reading
    std::ifstream reads_writes_file_stream(reads_writes_file); // open reads & writes file for reading

    // if memory trace file is non-existent, print error message
    if (!mtrace_file_stream.is_open()) {
        std::cerr << "Unable to open <<trace.tr>>" << std::endl;
        return 1; // exit program, error occurred
    }

    /// if reads & writes file is non-existent, print error message
    if (!reads_writes_file_stream.is_open()) {
        std::cerr << "Unable to open <<readswrites.txt>>" << std::endl;
        return 1; // exit program, error occurred
    }

    int totalNumBitsAllLevels = 0;
    if (idx < argc) {
        int bitsPerLevel [argc - idx];
        for (int i = idx; i < argc; i++) {
          bitsPerLevel[i] = atoi(argv[i]);
          totalNumBitsAllLevels += bitsPerLevel[i];
        }
       if (totalNumBitsAllLevels > 28) {
         std::cerr << "Too many bits used in page tables" << std::endl;
         return 1; // exit program, error occurred
       }
    }

    p2AddrTr mtrace;    // structure is typedefed in vaddr_tracereader
    uint32_t vAddr;    // unsigned 32-bit integer type
    if (NextAddress(mtrace_file_stream, &mtrace)) {    // if next address exists...
      vAddr = mtrace.addr;    // ...store next address
    }

    return 0;
}
