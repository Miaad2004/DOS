#pragma once
#include <string>
#include <vector>
#include "memory_manager.h"

class FileNode
{
public:
    std::string name;
    bool isDirectory;
    std::vector<FileNode*> children;
    char* content;
    MemoryManager* memManager;
    FileNode* parent;

    FileNode(std::string n, bool isDir, MemoryManager* mm, FileNode* p = nullptr);
    ~FileNode();
};

bool isValidFilename(const std::string& name);