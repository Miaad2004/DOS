file(GLOB SOURCES
    main.c
    disk_manager.c
    memory_manager.c
    file_system.c
    command_parser.c
    commands/*.c
)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(DOS ${SOURCES})
