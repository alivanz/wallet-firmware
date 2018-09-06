cmake_minimum_required(VERSION 3.1)
project (XWallet)
set(CMAKE_BUILD_TYPE Release)

macro(use_c99)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
    endif ()
  else ()
    set (CMAKE_C_STANDARD 99)
  endif ()
endmacro(use_c99)

if(WIN32)
  # Windows
  # include_directories(deps/pthread/prebuilt-dll-2-9-1-release/include)
  # add_definitions(-DHAVE_STRUCT_TIMESPEC)

  # execute_process(
  #   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/clib
  #   COMMAND go build -buildmode=c-shared -o clib.lib
  # )
  # if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  #     # 64 bits
  #     link_directories(deps/pthread/prebuilt-dll-2-9-1-release/lib/x64)
  # elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
  #     # 32 bits
  #     link_directories(deps/pthread/prebuilt-dll-2-9-1-release/lib/x86)
  # endif()
ELSE()
  # execute_process(
  #   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/clib
  #   COMMAND go build -buildmode=c-shared -o clib.so
  # )
ENDIF()

use_c99()
include_directories(micro-ecc)
include_directories(firmware)
# link_directories(clib)
# add_library(clib.a)
add_executable(XWallet-Simulator main.c firmware/command.c firmware/ecc.c firmware/sha2.c micro-ecc/uECC.c)
