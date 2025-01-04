#include "memory_manager.h"

MemoryManager::MemoryManager() : nextFreePage(0), swapFilePath("swap.bin")
{
    for (int i = 0; i < VIRTUAL_PAGES; i++)
    {
        virtualPageTable[i].present = false;
        virtualPageTable[i].dirty = false;
    }
}

void MemoryManager::swapOut(int virtualPageNum)
{
    auto& page = virtualPageTable[virtualPageNum];
    if (page.dirty)
    {
        std::ofstream swapFile(swapFilePath, std::ios::binary | std::ios::app);
        swapFile.seekp(virtualPageNum * PAGE_SIZE);
        swapFile.write(&physicalMemory[page.physicalPageNum * PAGE_SIZE], PAGE_SIZE);
    }
    page.present = false;
}

void MemoryManager::swapIn(int virtualPageNum, int physicalPageNum)
{
    std::ifstream swapFile(swapFilePath, std::ios::binary);
    swapFile.seekg(virtualPageNum * PAGE_SIZE);
    swapFile.read(&physicalMemory[physicalPageNum * PAGE_SIZE], PAGE_SIZE);

    virtualPageTable[virtualPageNum].present = true;
    virtualPageTable[virtualPageNum].physicalPageNum = physicalPageNum;
    virtualPageTable[virtualPageNum].lastAccess = time(nullptr);
}

int MemoryManager::findVictimPage()
{
    long oldestAccess = LONG_MAX;
    int victimPage = 0;

    for (int i = 0; i < VIRTUAL_PAGES; i++)
    {
        if (virtualPageTable[i].present && virtualPageTable[i].lastAccess < oldestAccess)
        {
            oldestAccess = virtualPageTable[i].lastAccess;
            victimPage = i;
        }
    }
    return victimPage;
}

char* MemoryManager::allocatePage()
{
    int virtualPageNum = 0;
    for (; virtualPageNum < VIRTUAL_PAGES; virtualPageNum++)
    {
        if (!virtualPageTable[virtualPageNum].present)
            break;
    }

    if (virtualPageNum >= VIRTUAL_PAGES)
    {
        return nullptr;
    }

    if (nextFreePage >= TOTAL_PAGES)
    {
        int victimVirtualPage = findVictimPage();
        swapOut(victimVirtualPage);
        nextFreePage = virtualPageTable[victimVirtualPage].physicalPageNum;
    }

    virtualPageTable[virtualPageNum].present = true;
    virtualPageTable[virtualPageNum].dirty = false;
    virtualPageTable[virtualPageNum].physicalPageNum = nextFreePage;
    virtualPageTable[virtualPageNum].lastAccess = time(nullptr);

    nextFreePage++;
    return &physicalMemory[virtualPageTable[virtualPageNum].physicalPageNum * PAGE_SIZE];
}

void MemoryManager::freePage(char* page)
{
    if (!page)
        return;

    int physicalPageNum = (page - physicalMemory) / PAGE_SIZE;

    for (int i = 0; i < VIRTUAL_PAGES; i++)
    {
        if (virtualPageTable[i].present &&
            virtualPageTable[i].physicalPageNum == physicalPageNum)
        {
            virtualPageTable[i].present = false;
            virtualPageTable[i].dirty = false;
            if (nextFreePage > 0)
                nextFreePage--;
            break;
        }
    }
}

void MemoryManager::markDirty(char* page)
{
    int physicalPageNum = (page - physicalMemory) / PAGE_SIZE;
    for (int i = 0; i < VIRTUAL_PAGES; i++)
    {
        if (virtualPageTable[i].present &&
            virtualPageTable[i].physicalPageNum == physicalPageNum)
        {
            virtualPageTable[i].dirty = true;
            break;
        }
    }
}

void MemoryManager::hibernate(const std::string& hibernateFile)
{
    std::ofstream file(hibernateFile, std::ios::binary);
    file.write((char*)virtualPageTable, sizeof(PageTableEntry) * VIRTUAL_PAGES);
    file.write(physicalMemory, PAGE_SIZE * TOTAL_PAGES);
    file.write(virtualMemory, PAGE_SIZE * VIRTUAL_PAGES);
}

void MemoryManager::resume(const std::string& hibernateFile)
{
    std::ifstream file(hibernateFile, std::ios::binary);
    file.read((char*)virtualPageTable, sizeof(PageTableEntry) * VIRTUAL_PAGES);
    file.read(physicalMemory, PAGE_SIZE * TOTAL_PAGES);
    file.read(virtualMemory, PAGE_SIZE * VIRTUAL_PAGES);
}