file(GLOB SOURCES
    main.cpp
    memory_manager.cpp
    file_system.cpp
    dos_shell.cpp
)

add_executable(DOS ${SOURCES})