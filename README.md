# MS-DOS Shell Simulation

## Overview
This project is a simplified operating system shell inspired by MS-DOS. It simulates file management functionalities, directory navigation, and command execution. The system allows users to interact with files and directories, execute basic commands, and manage memory structures in a simulated DOS environment.

## Features
- **File System Management**
  - Create, delete, and rename files and directories.
  - List directory contents.
  - Search for specific content within files.
  
- **Command Execution**
  - Execute basic shell commands such as `MKDIR`, `CD`, `DEL`, `DIR`, `ECHO`, `FIND`, `REN`, `TYPE`, and `XCOPY`.
  - Set and retrieve environment variables.
  - Execute conditional statements with `IF`.
  
- **System Utilities**
  - Hibernate and resume system state.
  - Display and modify system date and time.
  - Store system memory state on disk.

## Commands
The shell supports the following commands:

| Command  | Description |
|----------|-------------|
| `MKDIR`  | Creates a new directory in the current path. |
| `CD`     | Changes the current directory. |
| `ECHO`   | Displays text or writes content to a file. |
| `DIR`    | Lists all files and directories in the current path. |
| `DEL`    | Deletes a specified file. |
| `FIND`   | Searches for a string in files. |
| `REN`    | Renames a file. |
| `TYPE`   | Displays the contents of a file. |
| `XCOPY`  | Copies files or directories from one location to another. |
| `RMDIR`  | Removes an empty directory. |
| `TIME`   | Displays or modifies the system time. |
| `DATE`   | Displays or modifies the system date. |
| `HIBERNATE` | Saves the current system state to disk. |
| `RESUME` | Restores the system from a saved state. |
| `HELP`   | Displays available commands and their descriptions. |


## Example Usage
```bash
> MKDIR myFolder
> CD myFolder
> ECHO "Hello World" > file.txt
> TYPE file.txt
Hello World
```

## Contributors
- **Miad Kimiyagari**
- **Amir Taha Najaf**


