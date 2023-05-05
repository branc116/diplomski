cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/stm32_gcc.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

project(stm32-bt C CXX ASM)

find_package(CMSIS COMPONENTS STM32L476G REQUIRED)
find_package(HAL COMPONENTS STM32L476G REQUIRED)

set(PS
  app/cube_hal_l4.c
  app/app_bluenrg_ms.c
  app/main.c
  app/main.cpp
  app/sample_service.c
  app/stm32l4xx_hal_msp.c
  app/stm32l4xx_it.c

  bluenrg/bluenrg_gap_aci.c
  bluenrg/bluenrg_gatt_aci.c
  bluenrg/bluenrg_hal_aci.c
  bluenrg/bluenrg_IFR.c
  bluenrg/bluenrg_l2cap_aci.c
  bluenrg/bluenrg_updater_aci.c
  bluenrg/bluenrg_utils_small.c
  bluenrg/hci_le.c
  bluenrg/hci_tl.c
  bluenrg/hci_tl_interface.c

  SensorTile/SensorTile_accelero.c
  SensorTile/SensorTile_audio_in.c
  SensorTile/SensorTile_BlueNRG.c
  SensorTile/SensorTile.c
  SensorTile/SensorTile_gyro.c
  SensorTile/SensorTile_humidity.c
  SensorTile/SensorTile_magneto.c
  SensorTile/SensorTile_pressure.c
  SensorTile/SensorTile_sd.c
  SensorTile/SensorTile_temperature.c

  lsm6dsm/LSM6DSM_ACC_GYRO_driver.c
  lsm6dsm/LSM6DSM_ACC_GYRO_driver_HL.c

  hts221/HTS221_Driver.c
  hts221/HTS221_Driver_HL.c

  lsm303agr/LSM303AGR_ACC_driver.c
  lsm303agr/LSM303AGR_ACC_driver_HL.c
  lsm303agr/LSM303AGR_MAG_driver.c
  lsm303agr/LSM303AGR_MAG_driver_HL.c

  lps22hb/LPS22HB_Driver.c
  lps22hb/LPS22HB_Driver_HL.c

  syscalls.c
)


set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

add_executable(stm32-bt ${PS})
target_compile_options(stm32-bt PRIVATE -Wno-missing-braces)
target_compile_features(stm32-bt PRIVATE cxx_std_20)

target_include_directories(stm32-bt PRIVATE "app")
target_include_directories(stm32-bt PRIVATE "bluenrg")
target_include_directories(stm32-bt PRIVATE "SensorTile")
target_include_directories(stm32-bt PRIVATE "lps22hb")
target_include_directories(stm32-bt PRIVATE "lsm303agr")
target_include_directories(stm32-bt PRIVATE "lsm6dsm")
target_include_directories(stm32-bt PRIVATE "hts221")
target_include_directories(stm32-bt PRIVATE "common_sensors")
target_include_directories(stm32-bt PRIVATE "usb")

target_link_libraries(stm32-bt CMSIS::STM32::L476JG
  HAL::STM32::L4::ADC
  HAL::STM32::L4::CORTEX
  HAL::STM32::L4::DMA
  HAL::STM32::L4::DFSDM
  HAL::STM32::L4::EXTI
  HAL::STM32::L4::FLASH
  HAL::STM32::L4::GPIO
  HAL::STM32::L4::I2C
  HAL::STM32::L4::PCD
  HAL::STM32::L4::PCDEx
  HAL::STM32::L4::PWR
  HAL::STM32::L4::PWREx
  HAL::STM32::L4::RCC
  HAL::STM32::L4::RCCEx
  HAL::STM32::L4::RTC
  HAL::STM32::L4::SPI
  HAL::STM32::L4::TIM
  HAL::STM32::L4::TIMEx
  HAL::STM32::L4::LL_USB
  STM32::NoSys)
stm32_print_size_of_target(stm32-bt)

