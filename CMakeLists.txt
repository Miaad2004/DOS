# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("CMakeProject1")

# Add source to this project's executable.
include_directories(${CMAKE_SOURCE_DIR}/include)

#add_executable (CMakeProject1 "src/main.cpp"  "src/dos_shell.cpp" "src/file_system.cpp" "src/memory_manager.cpp")

# TODO: Add tests and install targets if needed.
add_subdirectory(src)