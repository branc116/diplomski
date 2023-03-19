cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/stm32_gcc.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

project(stm32-bt C ASM)
add_definitions( -D__FPU_PRESENT=1 ) 

find_package(CMSIS COMPONENTS STM32F767ZI REQUIRED)
# Find CMSIS for all F4 devices:
#find_package(CMSIS COMPONENTS STM32F4 REQUIRED)
# Find CMSIS for all devices:
#find_package(CMSIS REQUIRED)

set(PROJECT_SOURCES
    main.c
    sys.c
)

add_executable(stm32-bt ${PROJECT_SOURCES})
target_link_libraries(stm32-bt CMSIS::STM32::F767ZI STM32::NoSys)
stm32_print_size_of_target(stm32-bt)
