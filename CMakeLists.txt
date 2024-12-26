# CMakeLists.txt : CMake project for DOS, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("DOS" LANGUAGES C) # Specify the project uses C

# Set the C standard
set(CMAKE_C_STANDARD 99) # Adjust to 11 or 17 if needed
set(CMAKE_C_STANDARD_REQUIRED True)

# Add source to this project's executable
add_executable (DOS "DOS.c" "DOS.h")

# TODO: Add tests and install targets if needed.
