cmake_minimum_required (VERSION 3.2)

set(TARGET_NAME GlewGlutter)
project(${TARGET_NAME})


set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_DIR}/build/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_DIR}/build/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_DIR}/build/lib")


if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()


if(0)
  set(CMAKE_CXX_FLAGS "-O3 -march=native -lrt")
endif()



file(GLOB CPP_FILES ${PROJECT_DIR}/src/*.cpp)
file(GLOB HPP_FILES ${PROJECT_DIR}/src/*.h)


# Add a target to build an executable
add_executable(${TARGET_NAME} ${HPP_FILES} ${CPP_FILES})

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")


# adding glew
add_definitions(-DGLEW_STATIC)
option(glew-cmake_BUILD_SHARED "" OFF)
add_subdirectory(external/glew-cmake)
include_directories(external/glew-cmake/include external/glew-cmake/src)

# adding glfw
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Do not build the GLFW example programs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Do not build the GLFW tests programs" FORCE)
set(GLFW_INSTALL OFF CACHE STRING "" FORCE)
add_subdirectory(external/glfw)
include_directories(SYSTEM external/glfw/include)

# adding glm
include_directories(SYSTEM external/glm)

# adding assimp
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Do not build the tools" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Do not build the tests" FORCE)
add_subdirectory(external/assimp)
include_directories(SYSTEM external/assimp/include)

# adding SOIL
add_subdirectory(external/SOIL)
include_directories(SYSTEM external/SOIL/src)

if(WIN32)
  set(LIBS glfw opengl32 libglew_static SOIL_Static assimp)
elseif(UNIX)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
  # Linux packages native to CMake
  set(LIBS glfw)
  list(APPEND LIBS libglew_static)
  list(APPEND LIBS SOIL_Static)
  list(APPEND LIBS assimp)
  find_package(OpenGL REQUIRED)
  list(APPEND LIBS ${OPENGL_gl_LIBRARY}) # setting LIBS for the first time
  add_definitions(${OPENGL_DEFINITIONS})
  find_package(X11 REQUIRED)
  list(APPEND LIBS ${X11_Xrandr_LIB} ${X11_Xxf86vm_LIB} ${X11_Xinerama_LIB} ${X11_Xi_LIB} ${X11_Xcursor_LIB})
  find_package(Threads REQUIRED)
  list(APPEND LIBS ${CMAKE_THREAD_LIBS_INIT})
  #find_library(RT_LIB rt)
  #list(APPEND LIBS ${RT_LIB})
  # append non-native packages
else()
  set(LIBS )
endif(WIN32)

# Because we use glfw3 we need to link the application with Cocoa
IF(APPLE)
  INCLUDE_DIRECTORIES(/System/Library/Frameworks)
  FIND_LIBRARY(Cocoa_LIBRARY Cocoa)
  FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
  FIND_LIBRARY(IOKit_LIBRARY IOKit)
  FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
  MARK_AS_ADVANCED(Cocoa_LIBRARY OpenGL_LIBRARY)
  SET(APPLE_LIBS ${Cocoa_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
  set(LIBS ${LIBS} ${APPLE_LIBS})
ENDIF(APPLE)


# clang && macosx
if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") AND APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()
# c++11
if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
# clang && debug adds address sanitizer
if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") AND NOT APPLE)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
endif()

#IF (FREETYPE_FOUND)
 # INCLUDE_DIRECTORIES(
  #  ${FREETYPE_INCLUDE_DIR_ft2build}
  #  ${FREETYPE_INCLUDE_DIR_ft2build}/freetype2
  #)
#ENDIF()



# This executable requires a few libraries to link
#TARGET_LINK_LIBRARIES(${TARGET_NAME} ${VRLIB} ${SOILLIB} ${GLUT_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} )
#${PNG_LIBRARY} ${ZLIB_LIBRARY} ${FREETYPE_LIBRARY} ${JPEG_LIBRARY}

#set (CMAKE_CXX_FLAGS "-O3 -march=native -lrt")
##set(CMAKE_BUILD_TYPE DEBUG)
#set(CMAKE_C_FLAGS "-O3 -march=native -lrt")
#set(CMAKE_C_FLAGS_DEBUG " -O3 -march=native -lrt")
#set(CMAKE_C_FLAGS_RELEASE " -O3 -march=native -lrt")
#set(CMAKE_CXX_FLAGS "-O3 -march=native -lrt")
#set(CMAKE_CXX_FLAGS_DEBUG " -O3 -march=native -lrt")
#set(CMAKE_CXX_FLAGS_RELEASE " -O3 -march=native -lrt")


