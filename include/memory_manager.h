#pragma once
#include <string>
#include <fstream>
#include <climits>
#include <ctime>

const int PAGE_SIZE = 256;
const int TOTAL_PAGES = 16;
const int VIRTUAL_PAGES = 32;

class MemoryManager {
private:
    char physicalMemory[PAGE_SIZE * TOTAL_PAGES];
    char virtualMemory[PAGE_SIZE * VIRTUAL_PAGES];

    struct PageTableEntry {
        bool present;     // in physical memory?
        bool dirty;       // modified?
        int virtualPageNum;
        int physicalPageNum;
        long lastAccess;  // LRU
    } virtualPageTable[VIRTUAL_PAGES];

    std::string swapFilePath;
    int nextFreePage;

    void swapOut(int virtualPageNum);
    void swapIn(int virtualPageNum, int physicalPageNum);
    int findVictimPage();

public:
    MemoryManager();
    char* allocatePage();
    void freePage(char* page);
    void markDirty(char* page);
    void hibernate(const std::string& hibernateFile);
    void resume(const std::string& hibernateFile);
};