#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

// Core filesystem functions
void init_filesystem();
void cleanup_filesystem();
void format_disk();

// File operations
int create_file(const char* filename);

#endif // FILE_SYSTEM_H