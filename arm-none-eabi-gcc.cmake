set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)


string(FIND $ENV{PATH} "RUDironDistributive/components/gcc-arm-none-eabi/bin" TOOLCHAIN_FOUND_IN_PATH)

#проверка нахождения gcc в path
if (NOT ${TOOLCHAIN_FOUND_IN_PATH} MATCHES "-1")
    set(TOOLCHAIN_PREFIX arm-none-eabi-)
else ()
    #при необходимости укажите полный путь к gcc
    set(ARM_TOOLCHAIN_DIR "/Users/../RUDironDistributive/components/gcc-arm-none-eabi/bin")
    set(BINUTILS_PATH ${ARM_TOOLCHAIN_DIR})
    set(TOOLCHAIN_PREFIX ${ARM_TOOLCHAIN_DIR}/arm-none-eabi-)
endif ()


if (WIN32)
    set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}gcc.exe")
    set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++.exe")
else ()
    set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}gcc")
    set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++")
endif ()

set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})

set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL ${TOOLCHAIN_PREFIX}size CACHE INTERNAL "size tool")

set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

