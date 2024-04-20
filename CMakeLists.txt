cmake_minimum_required (VERSION 3.15)

# Enable Hot Reload for MSVC compilers if supported.
#if (POLICY CMP0141)
#  cmake_policy(SET CMP0141 NEW)
#  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
#endif()

project (BlockForge)

# Set proper configuration
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ARCHITECTURE "Win32")
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHITECTURE "x64")
  endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(ARCHITECTURE "Unix")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  message("MacOS build is EXPERIMENTAL! Expect bugs.")
  set(ARCHITECTURE "Unix")
else()
  message(WARNING "This platform is unsupported! Assumming it's unix-based...")
  set(ARCHITECTURE "Unix")
endif()

if(NOT CMAKE_BUILD_TYPE)
  if(DEBUG_MODE)
    set(CMAKE_BUILD_TYPE "Debug")
  else()
    set(CMAKE_BUILD_TYPE "Release")
  endif()
endif()

# Add the common source files
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(BlockForge ${SOURCE_FILES})
include_directories(BlockForge 
    PRIVATE 
    "${CMAKE_SOURCE_DIR}/include"
)

# Files to copy
set(to_copy
    "Settings.json"
    "data"
)

foreach(file ${to_copy})
    set(copy_src "${CMAKE_SOURCE_DIR}/${file}")
    set(copy_dst "${CMAKE_BINARY_DIR}/${file}")

    if(EXISTS ${copy_dst})
        file(REMOVE_RECURSE ${copy_dst})
    endif()

    file(COPY ${copy_src} DESTINATION ${CMAKE_BINARY_DIR})
endforeach()

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(OpenGL::GL)

# Libraries
include(FetchContent)


FetchContent_Declare (
  fastnoise2
  GIT_REPOSITORY https://github.com/Auburn/FastNoise2.git
  GIT_TAG        v0.10.0-alpha
)

FetchContent_Declare (
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.4
)

FetchContent_Declare (
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        1.0.1
)

FetchContent_Declare (
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        v1.90.5
)

FetchContent_Declare (
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.3
)

FetchContent_Declare (
  robin_hood_hashing
  GIT_REPOSITORY https://github.com/martinus/robin-hood-hashing.git
  GIT_TAG        3.11.5
)

FetchContent_Declare (
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.13.0
)

FetchContent_Declare (
  spscqueue
  GIT_REPOSITORY https://github.com/rigtorp/SPSCQueue.git
  GIT_TAG        v1.1
)

FetchContent_MakeAvailable(fastnoise2 glfw glm imgui json robin_hood_hashing spdlog spscqueue)
target_link_libraries(${PROJECT_NAME} PRIVATE fastnoise2 glfw glm imgui json robin-hood-hashing spdlog spscqueue)

# Repositories without tag
include_directories(PRIVATE 
    "${CMAKE_SOURCE_DIR}/vendor/glText"
    "${CMAKE_SOURCE_DIR}/vendor/stb"
)

# Directories
include_directories("${glfw_SOURCE_DIR}/include")
include_directories("${glm_SOURCE_DIR}/glm")
include_directories("${robin_hood_hashing_SOURCE_DIR}/src/include")
include_directories("${json_SOURCE_DIR}/include")
include_directories("${fastnoise2_SOURCE_DIR}/include")
include_directories("${imgui_SOURCE_DIR}")
include_directories("${spscqueue_SOURCE_DIR}/include")
