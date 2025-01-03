file(GLOB SOURCES
    main.c
    mem.c
    file_system.c
    command_parser.c
)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(DOS ${SOURCES})
