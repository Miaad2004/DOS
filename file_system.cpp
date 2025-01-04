#include "file_system.h"
#include "memory_manager.h"

FileNode::FileNode(std::string n, bool isDir, MemoryManager* mm, FileNode* p) : name(n), isDirectory(isDir), memManager(mm), content(nullptr), parent(p)
{
    if (!isDir)
    {
        content = memManager->allocatePage();
    }
}

FileNode::~FileNode()
{
    if (!isDirectory && content)
    {
        memManager->freePage(content);
    }
    for (auto child : children)
    {
        delete child;
    }
}

bool isValidFilename(const std::string& name)
{
    if (name.empty())
        return false;
    if (name.length() > 12)
        return false;

    const std::string invalidChars = "<>:\"/\\|?*";
    if (name.find_first_of(invalidChars) != std::string::npos)
        return false;

    return true;
}