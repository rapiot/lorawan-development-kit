cmake_minimum_required(VERSION 3.19)

if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(application LANGUAGES MikroC)
else()
    project(application LANGUAGES C ASM)
endif()

include(mikroeUtils)

if (${MIKROSDK_TYPE} STREQUAL "legacy")
    legacy_cmake_path()
endif()

set(PROJECT_TYPE "mikrosdk" CACHE STRING "" FORCE)

add_executable(application
include/application.h
source/application.c
source/main.c
../../../rat_libraries/rat_indoor_sensor/include/rat_indoor_sensor.h
../../../rat_libraries/rat_indoor_sensor/source/rat_indoor_sensor.c
../../../rat_libraries/rat_utilities/include/rat_utils.h
../../../rat_libraries/rat_utilities/source/rat_utils.c
../../../rat_libraries/rat_utilities/include/rat_i2c.h
../../../rat_libraries/rat_utilities/source/rat_i2c.c
../../../rat_libraries/rat_microcontroller/include/rat_microcontroller.h
../../../rat_libraries/rat_microcontroller/source/rat_microcontroller.c
../../../rat_libraries/rat_radio_module/include/rat_radio_module.h
../../../rat_libraries/rat_radio_module/source/rat_radio_module.c
../../../rat_libraries/rat_utilities/include/rat_uart.h
../../../rat_libraries/rat_utilities/source/rat_uart.c
../../../rat_common/include/lorawan.h
)

############################ application GENERATED CODE START ###########################
find_package(MikroSDK.Driver.UART REQUIRED)
target_link_libraries(application PUBLIC MikroSDK.Driver.UART)
find_package(MikroSDK.Driver.I2C.Master REQUIRED)
target_link_libraries(application PUBLIC MikroSDK.Driver.I2C.Master)
find_package(MikroSDK.Driver.GPIO.Port REQUIRED)
target_link_libraries(application PUBLIC MikroSDK.Driver.GPIO.Port)
find_package(MikroSDK.Driver.GPIO.Out REQUIRED)
target_link_libraries(application PUBLIC MikroSDK.Driver.GPIO.Out)
find_package(MikroC.Core REQUIRED)
target_link_libraries(application PUBLIC MikroC.Core)
############################ application GENERATED CODE END ###########################
find_package(MikroC.Core REQUIRED)
target_link_libraries(application
    PUBLIC
        MikroC.Core
)

if (${MIKROSDK_TYPE} STREQUAL "legacy")
    find_package(MikroC.System REQUIRED)
    target_link_libraries(application
        PUBLIC
            MikroC.System
    )
endif()

if(COMPILER_REVISION)
    if(${COMPILER_REVISION} VERSION_GREATER_EQUAL "1.0")
        if(${TOOLCHAIN_LANGUAGE} STREQUAL "GNU")
            if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.meproject)
                fetch_startup_name(${CMAKE_CURRENT_SOURCE_DIR}/.meproject startupName)
                if(startupName)
                    add_startup_all_targets(. ${CMAKE_CURRENT_SOURCE_DIR} ${startupName})
                endif()
                fetch_linker_name(${CMAKE_CURRENT_SOURCE_DIR}/.meproject linkerName)
                if(linkerName)
                    add_ld_all_targets(. ${CMAKE_CURRENT_SOURCE_DIR} ${linkerName})
                endif()
            endif()
        endif()
    endif()
endif()
