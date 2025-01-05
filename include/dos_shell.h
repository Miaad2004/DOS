#pragma once
#include <string>
#include "file_system.h"
#include "memory_manager.h"

class DOSShell {
private:
    static constexpr const char* VALID_COMMANDS[] = {
        "DIR", "CD", "MKDIR", "RMDIR", "ECHO", "DEL", "REN",
        "TYPE", "RUN", "HIBERNATE", "RESUME", "HELP", "EXIT", "XCOPY"
    };

    FileNode* currentDir;
    FileNode* root;
    MemoryManager memManager;

    bool isValidCommand(const std::string& cmd);
    void showHelp();
    void runProgram(const std::string& filename);
    void serializeFileSystem(FileNode* node, std::ofstream& file);
    FileNode* deserializeFileSystem(std::ifstream& file, FileNode* parent);
    void listDirectory();
    void changeDirectory(const std::string& dir);
    void createFile(const std::string& name, const std::string& content);
    void createDirectory(const std::string& name);
    void deleteFile(const std::string& name);
    void renameFile(const std::string& oldname, const std::string& newname);
    void readFile(const std::string& name);
    void removeDirectory(const std::string& name);
    void xcopyCommand(const std::string& source, const std::string& dest);
    FileNode* findNode(const std::string& path, FileNode* startDir);
    FileNode* copyNode(FileNode* source, FileNode* destParent);

public:
    DOSShell();
    std::string getCurrentPath();
    void executeCommand(const std::string& cmdLine);
    void hibernate(const std::string& filename);
    void resume(const std::string& filename);
};