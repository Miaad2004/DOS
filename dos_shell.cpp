#include "dos_shell.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstring>

bool DOSShell::isValidCommand(const std::string& cmd)
{
    std::string upperCmd = cmd;
    std::transform(upperCmd.begin(), upperCmd.end(), upperCmd.begin(), ::toupper);

    for (const auto& validCmd : VALID_COMMANDS)
    {
        if (validCmd == upperCmd)
            return true;
    }
    return false;
}

FileNode* currentDir;
FileNode* root;
MemoryManager memManager;

void DOSShell::showHelp()
{
    std::cout << "Available commands:\n"
        << "  DIR              - List directory contents\n"
        << "  CD <dir>         - Change directory\n"
        << "  MKDIR <dir>      - Create directory\n"
        << "  RMDIR <dir>      - Remove empty directory\n"
        << "  ECHO <text>      - Display text\n"
        << "  ECHO <text> >file- Write text to file\n"
        << "  DEL <file>       - Delete file\n"
        << "  REN <old> <new>  - Rename file\n"
        << "  TYPE <file>      - Display file contents\n"
        << "  RUN <file>       - Execute .COM file\n"
        << "  HIBERNATE <file> - Save system state\n"
        << "  RESUME <file>    - Restore system state\n"
        << "  HELP             - Show this help\n"
        << "  XCOPY src dest   - Copy files/directories recursively\n"
        << "  DATE [MM-DD-YYYY]- Display/set system date\n"
        << "  TIME [HH:MM:SS]  - Display/set system time\n"
        << "  EXIT             - Exit shell\n";
}

void DOSShell::runProgram(const std::string& filename)
{
    for (auto node : currentDir->children)
    {
        if (!node->isDirectory && node->name == filename)
        {
            if (filename.length() < 4 ||
                filename.substr(filename.length() - 4) != ".COM")
            {
                std::cout << "Not a COM file\n";
                return;
            }

            // Simple program execution simulation
            std::cout << "Executing " << filename << "...\n";
            // Here you could interpret the content as machine code
            // For now, just display the content as if it was output
            std::cout << "Program output:\n"
                << node->content << std::endl;
            return;
        }
    }
    std::cout << "Program not found\n";
}

// Add these methods to DOSShell class:
void DOSShell::serializeFileSystem(FileNode* node, std::ofstream& file)
{
    // Save node properties
    size_t nameLen = node->name.length();
    file.write((char*)&nameLen, sizeof(nameLen));
    file.write(node->name.c_str(), nameLen);
    file.write((char*)&node->isDirectory, sizeof(bool));

    if (!node->isDirectory && node->content)
    {
        file.write(node->content, PAGE_SIZE);
    }

    // Save children count and recursively save children
    size_t childCount = node->children.size();
    file.write((char*)&childCount, sizeof(childCount));
    for (auto child : node->children)
    {
        serializeFileSystem(child, file);
    }
}

FileNode* DOSShell::deserializeFileSystem(std::ifstream& file, FileNode* parent)
{
    // Read node properties
    size_t nameLen;
    file.read((char*)&nameLen, sizeof(nameLen));

    char* nameBuf = new char[nameLen + 1];
    file.read(nameBuf, nameLen);
    nameBuf[nameLen] = '\0';
    std::string name(nameBuf);
    delete[] nameBuf;

    bool isDir;
    file.read((char*)&isDir, sizeof(bool));

    FileNode* node = new FileNode(name, isDir, &memManager, parent);

    if (!isDir && node->content)
    {
        file.read(node->content, PAGE_SIZE);
    }

    // Read children
    size_t childCount;
    file.read((char*)&childCount, sizeof(childCount));
    for (size_t i = 0; i < childCount; i++)
    {
        node->children.push_back(deserializeFileSystem(file, node));
    }

    return node;
}

DOSShell::DOSShell()
{
    root = new FileNode("C:", true, &memManager);
    currentDir = root;
    
    // Initialize date/time
    dateTime.isCustomDate = false;
    dateTime.isCustomTime = false;
    
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    dateTime.month = timeinfo->tm_mon + 1;
    dateTime.day = timeinfo->tm_mday;
    dateTime.year = timeinfo->tm_year + 1900;
    dateTime.hour = timeinfo->tm_hour;
    dateTime.min = timeinfo->tm_min;
    dateTime.sec = timeinfo->tm_sec;
}

// Move getCurrentPath() to public section
std::string DOSShell::getCurrentPath()
{
    std::vector<std::string> paths;
    FileNode* temp = currentDir;

    while (temp != nullptr)
    {
        paths.push_back(temp->name);
        temp = temp->parent;
    }

    std::string fullPath;
    for (int i = paths.size() - 1; i >= 0; i--)
    {
        fullPath += paths[i];
        if (i > 0)
            fullPath += "\\";
    }

    return fullPath;
}

void DOSShell::executeCommand(const std::string& cmdLine)
{
    std::istringstream iss(cmdLine);
    std::string command;
    iss >> command;

    if (command.empty())
        return;

    std::transform(command.begin(), command.end(), command.begin(), ::toupper);

    if (!isValidCommand(command))
    {
        std::cout << "Bad command or file name\n";
        return;
    }

    if (command == "DIR")
    {
        listDirectory();
    }
    else if (command == "CD")
    {
        std::string dir;
        iss >> dir;
        changeDirectory(dir);
    }
    else if (command == "ECHO")
    {
        std::string content, arrow, filename;
        std::getline(iss, content);
        size_t pos = content.find(">");

        if (pos != std::string::npos)
        {
            std::string text = content.substr(0, pos);
            filename = content.substr(pos + 1);
            filename.erase(0, filename.find_first_not_of(" "));
            createFile(filename, text);
        }
        else
        {
            std::cout << content << std::endl;
        }
    }
    else if (command == "MKDIR")
    {
        std::string dirname;
        iss >> dirname;
        createDirectory(dirname);
    }
    else if (command == "DEL")
    {
        std::string filename;
        iss >> filename;
        deleteFile(filename);
    }
    else if (command == "REN")
    {
        std::string oldname, newname;
        iss >> oldname >> newname;
        renameFile(oldname, newname);
    }
    else if (command == "TYPE")
    {
        std::string filename;
        iss >> filename;
        readFile(filename);
    }
    else if (command == "HIBERNATE")
    {
        std::string filename;
        iss >> filename;
        hibernate(filename);
        std::cout << "System state saved to " << filename << std::endl;
    }
    else if (command == "RESUME")
    {
        std::string filename;
        iss >> filename;
        resume(filename);
        std::cout << "System state restored from " << filename << std::endl;
    }
    else if (command == "HELP")
    {
        showHelp();
    }
    else if (command == "RUN")
    {
        std::string filename;
        iss >> filename;
        runProgram(filename);
    }
    else if (command == "RMDIR")
    {
        std::string dirname;
        iss >> dirname;
        removeDirectory(dirname);
    }
    else if (command == "XCOPY") {
        std::string source, dest;
        iss >> source >> dest;
        if (source.empty() || dest.empty()) {
            std::cout << "Syntax: XCOPY source destination\n";
        } else {
            xcopyCommand(source, dest);
        }
    }
    else if (command == "DATE") {
        std::string dateStr;
        std::getline(iss >> std::ws, dateStr);
        handleDate(dateStr);
    }
    else if (command == "TIME") {
        std::string timeStr;
        std::getline(iss >> std::ws, timeStr);
        handleTime(timeStr);
    }
}

void DOSShell::listDirectory()
{
    std::cout << "Directory of " << getCurrentPath() << "\n\n";
    for (auto node : currentDir->children)
    {
        std::cout << node->name << (node->isDirectory ? " <DIR>" : "") << std::endl;
    }
}

void DOSShell::changeDirectory(const std::string& dir)
{
    if (dir == "..")
    {
        if (currentDir->parent != nullptr)
        {
            currentDir = currentDir->parent;
        }
        return;
    }

    for (auto node : currentDir->children)
    {
        if (node->isDirectory && node->name == dir)
        {
            currentDir = node;
            return;
        }
    }
    std::cout << "Directory not found\n";
}

void DOSShell::createFile(const std::string& name, const std::string& content)
{
    // Validate filename
    if (!isValidFilename(name))
    {
        std::cout << "Invalid filename\n";
        return;
    }

    // Check if file already exists
    for (auto node : currentDir->children)
    {
        if (node->name == name)
        {
            std::cout << "File already exists\n";
            return;
        }
    }

    // Create new file with parent pointer
    FileNode* file = new FileNode(name, false, &memManager, currentDir);
    if (!file->content)
    {
        std::cout << "Error: Could not allocate memory for file\n";
        delete file;
        return;
    }

    // Copy content safely
    strncpy(file->content, content.c_str(), PAGE_SIZE - 1);
    file->content[PAGE_SIZE - 1] = '\0'; // Ensure null termination
    currentDir->children.push_back(file);
}

void DOSShell::createDirectory(const std::string& name)
{
    if (!isValidFilename(name))
    {
        std::cout << "Invalid directory name\n";
        return;
    }

    // Check if directory already exists
    for (auto node : currentDir->children)
    {
        if (node->name == name)
        {
            std::cout << "Directory already exists\n";
            return;
        }
    }

    FileNode* dir = new FileNode(name, true, &memManager, currentDir);
    currentDir->children.push_back(dir);
}

void DOSShell::deleteFile(const std::string& name)
{
    auto it = std::find_if(currentDir->children.begin(),
        currentDir->children.end(),
        [&](FileNode* n)
        { return n->name == name; });

    if (it != currentDir->children.end())
    {
        delete* it;
        currentDir->children.erase(it);
    }
}

void DOSShell::renameFile(const std::string& oldname, const std::string& newname)
{
    if (!isValidFilename(newname))
    {
        std::cout << "Invalid filename\n";
        return;
    }

    // Check if new name already exists
    for (auto node : currentDir->children)
    {
        if (node->name == newname)
        {
            std::cout << "File already exists with that name\n";
            return;
        }
    }

    for (auto node : currentDir->children)
    {
        if (node->name == oldname)
        {
            node->name = newname;
            return;
        }
    }
    std::cout << "File not found\n";
}

void DOSShell::readFile(const std::string& name)
{
    for (auto node : currentDir->children)
    {
        if (!node->isDirectory && node->name == name)
        {
            std::cout << node->content << std::endl;
            return;
        }
    }
    std::cout << "File not found\n";
}

void DOSShell::hibernate(const std::string& filename)
{
    std::ofstream file(filename, std::ios::binary);

    // Save memory state
    memManager.hibernate(filename + ".mem");

    // Save file system state
    serializeFileSystem(root, file);

    // Save current directory path
    std::string currentPath = getCurrentPath();
    size_t pathLen = currentPath.length();
    file.write((char*)&pathLen, sizeof(pathLen));
    file.write(currentPath.c_str(), pathLen);
}

void DOSShell::resume(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);

    // Restore memory state
    memManager.resume(filename + ".mem");

    // Delete old file system
    delete root;

    // Restore file system
    root = deserializeFileSystem(file, nullptr);

    // Restore current directory
    size_t pathLen;
    file.read((char*)&pathLen, sizeof(pathLen));
    char* pathBuf = new char[pathLen + 1];
    file.read(pathBuf, pathLen);
    pathBuf[pathLen] = '\0';

    // Navigate to saved path
    currentDir = root;
    std::string path(pathBuf);
    std::stringstream ss(path);
    std::string dir;
    while (std::getline(ss, dir, '\\'))
    {
        if (dir != "C:")
        {
            changeDirectory(dir);
        }
    }

    delete[] pathBuf;
}

void DOSShell::removeDirectory(const std::string& name) {
    // Don't allow removing . or ..
    if (name == "." || name == "..") {
        std::cout << "Invalid directory name\n";
        return;
    }

    auto it = std::find_if(currentDir->children.begin(),
        currentDir->children.end(),
        [&](FileNode* n) { return n->name == name; });

    if (it == currentDir->children.end()) {
        std::cout << "Directory not found\n";
        return;
    }

    FileNode* dir = *it;
    if (!dir->isDirectory) {
        std::cout << "Not a directory\n";
        return;
    }

    if (!dir->children.empty()) {
        std::cout << "Directory not empty\n";
        return;
    }

    delete dir;
    currentDir->children.erase(it);
}

FileNode* DOSShell::findNode(const std::string& path, FileNode* startDir) {
    if (path.empty()) return startDir;
    
    std::istringstream pathStream(path);
    std::string segment;
    FileNode* current = startDir;

    while (std::getline(pathStream, segment, '\\')) {
        if (segment == ".") continue;
        if (segment == "..") {
            if (current->parent) current = current->parent;
            continue;
        }

        bool found = false;
        for (auto child : current->children) {
            if (child->name == segment) {
                current = child;
                found = true;
                break;
            }
        }
        if (!found) return nullptr;
    }
    return current;
}

FileNode* DOSShell::copyNode(FileNode* source, FileNode* destParent) {
    FileNode* newNode = new FileNode(source->name, source->isDirectory, &memManager, destParent);
    
    if (!source->isDirectory && source->content) {
        strncpy(newNode->content, source->content, PAGE_SIZE);
        memManager.markDirty(newNode->content);
    }

    // Recursively copy children
    for (auto child : source->children) {
        FileNode* newChild = copyNode(child, newNode);
        newNode->children.push_back(newChild);
    }

    return newNode;
}

void DOSShell::xcopyCommand(const std::string& source, const std::string& dest) {
    // Find source node
    FileNode* sourceNode = findNode(source, currentDir);
    if (!sourceNode) {
        std::cout << "Source not found\n";
        return;
    }

    // Find or create destination path
    std::string destPath = dest;
    std::string destName = sourceNode->name;
    size_t lastSep = dest.find_last_of('\\');
    FileNode* destParent;

    if (lastSep != std::string::npos) {
        destPath = dest.substr(0, lastSep);
        destName = dest.substr(lastSep + 1);
        destParent = findNode(destPath, currentDir);
    } else {
        destParent = currentDir;
        destName = dest;
    }

    if (!destParent) {
        std::cout << "Destination path not found\n";
        return;
    }

    // Check if destination already exists
    for (auto child : destParent->children) {
        if (child->name == destName) {
            std::cout << "Destination already exists\n";
            return;
        }
    }

    // Create copy
    FileNode* newNode = copyNode(sourceNode, destParent);
    newNode->name = destName;
    destParent->children.push_back(newNode);

    std::cout << "Successfully copied ";
    std::cout << (sourceNode->isDirectory ? "directory" : "file") << "\n";
}

void DOSShell::handleDate(const std::string& dateStr)
{
    if (dateStr.empty()) {
        char buffer[11];
        if (dateTime.isCustomDate) {
            sprintf(buffer, "%02d-%02d-%04d", 
                dateTime.month, dateTime.day, dateTime.year);
        } else {
            time_t now = time(nullptr);
            struct tm* timeinfo = localtime(&now);
            strftime(buffer, sizeof(buffer), "%m-%d-%Y", timeinfo);
        }
        std::cout << "Current date is: " << buffer << std::endl;
        return;
    }

    // Validate and parse date format MM-DD-YYYY
    if (dateStr.length() != 10 || dateStr[2] != '-' || dateStr[5] != '-') {
        std::cout << "Invalid date format. Use MM-DD-YYYY\n";
        return;
    }

    try {
        int month = std::stoi(dateStr.substr(0, 2));
        int day = std::stoi(dateStr.substr(3, 2));
        int year = std::stoi(dateStr.substr(6, 4));

        if (month < 1 || month > 12 || day < 1 || day > 31 || year < 1900) {
            std::cout << "Invalid date values\n";
            return;
        }

        dateTime.month = month;
        dateTime.day = day;
        dateTime.year = year;
        dateTime.isCustomDate = true;
        std::cout << "Date set to: " << dateStr << std::endl;
    }
    catch (...) {
        std::cout << "Invalid date format\n";
    }
}

void DOSShell::handleTime(const std::string& timeStr)
{
    if (timeStr.empty()) {
        char buffer[9];
        if (dateTime.isCustomTime) {
            sprintf(buffer, "%02d:%02d:%02d", 
                dateTime.hour, dateTime.min, dateTime.sec);
        } else {
            time_t now = time(nullptr);
            struct tm* timeinfo = localtime(&now);
            strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
        }
        std::cout << "Current time is: " << buffer << std::endl;
        return;
    }

    // Validate and parse time format HH:MM:SS
    if (timeStr.length() != 8 || timeStr[2] != ':' || timeStr[5] != ':') {
        std::cout << "Invalid time format. Use HH:MM:SS\n";
        return;
    }

    try {
        int hour = std::stoi(timeStr.substr(0, 2));
        int min = std::stoi(timeStr.substr(3, 2));
        int sec = std::stoi(timeStr.substr(6, 2));

        if (hour < 0 || hour > 23 || min < 0 || min > 59 || sec < 0 || sec > 59) {
            std::cout << "Invalid time values\n";
            return;
        }

        dateTime.hour = hour;
        dateTime.min = min;
        dateTime.sec = sec;
        dateTime.isCustomTime = true;
        std::cout << "Time set to: " << timeStr << std::endl;
    }
    catch (...) {
        std::cout << "Invalid time format\n";
    }
}