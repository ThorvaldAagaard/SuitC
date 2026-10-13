cmake_minimum_required(VERSION 3.21)

# The version follows Gerrit van der Velde's revision numbering: Rev 2.020 -> 2.20.
project(SuitC VERSION 2.20 DESCRIPTION "Bridge suit combination analyzer" LANGUAGES C)

option(SUITC_APP "Build SuitC with SUITCAPP=YES (the reduced output used by embedding applications)" OFF)
option(SUITC_BUILD_DDS "Build the dds double dummy solver" ON)
option(SUITC_BUILD_PLAYC "Build the playc play-code utility" ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# The programs recurse deeply and keep large tables on the stack; give Windows
# the same 8 MB main-thread stack that Linux and macOS have by default.
function(suitc_program_settings target)
  if(MSVC)
    target_compile_definitions(${target} PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(${target} PRIVATE /W1 /wd4996)
    target_link_options(${target} PRIVATE /STACK:8388608)
  elseif(MINGW)
    target_link_options(${target} PRIVATE -Wl,--stack,8388608)
  endif()
  if(NOT MSVC)
    target_link_libraries(${target} PRIVATE m)
  endif()
  install(TARGETS ${target} RUNTIME DESTINATION bin)
endfunction()

# extern.h is generated from global.h, as Gerrit's Makefiles do with sed.
function(suitc_generate_extern dir)
  set(out "${CMAKE_CURRENT_BINARY_DIR}/generated/${dir}/extern.h")
  add_custom_command(
    OUTPUT "${out}"
    COMMAND "${CMAKE_COMMAND}" -DIN=${CMAKE_CURRENT_SOURCE_DIR}/${dir}/global.h -DOUT=${out}
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/generate_extern.cmake"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/global.h" "${CMAKE_CURRENT_SOURCE_DIR}/cmake/generate_extern.cmake"
    COMMENT "Generating ${dir}/extern.h")
  set(EXTERN_H "${out}" PARENT_SCOPE)
endfunction()

# ---------------------------------------------------------------- SuitC
file(GLOB SUITC_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
suitc_generate_extern(src)
add_executable(suitc ${SUITC_SOURCES} "${EXTERN_H}")
set_target_properties(suitc PROPERTIES OUTPUT_NAME SuitC)
target_include_directories(suitc PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated/src" "${CMAKE_CURRENT_SOURCE_DIR}/src")
if(SUITC_APP)
  target_compile_definitions(suitc PRIVATE SUITCAPP=YES)
else()
  target_compile_definitions(suitc PRIVATE SUITCAPP=NO)
endif()
# Since Rev 2.0xx BLANK is set on the compiler command line, as in src/Makefile.
target_compile_definitions(suitc PRIVATE BLANK=NO)
suitc_program_settings(suitc)

# ---------------------------------------------------------------- dds
if(SUITC_BUILD_DDS)
  file(GLOB DDS_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/dds/*.c")
  suitc_generate_extern(dds)
  add_executable(dds ${DDS_SOURCES} "${EXTERN_H}")
  target_include_directories(dds PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated/dds" "${CMAKE_CURRENT_SOURCE_DIR}/dds")
  target_compile_definitions(dds PRIVATE TICK=1.0e-3)
  suitc_program_settings(dds)
endif()

# ---------------------------------------------------------------- playc
if(SUITC_BUILD_PLAYC)
  # playcode.c #includes the other playc sources (one translation unit).
  add_executable(playc "${CMAKE_CURRENT_SOURCE_DIR}/playc/playcode.c")
  target_include_directories(playc PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/playc")
  suitc_program_settings(playc)
endif()

# ---------------------------------------------------------------- tests
include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
