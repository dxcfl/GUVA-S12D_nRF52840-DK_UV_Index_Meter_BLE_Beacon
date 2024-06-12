#
# Copyright (c) 2024 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(GUVA-S12D_UV_Index_Meter_BLE_Beacon)

target_include_directories(app PRIVATE 
    src
)

target_sources(app PRIVATE 
    src/main.c 
    src/beacon.c 
    src/beacon.h
    src/GUVA_S12SD.c
    src/GUVA_S12SD.h
)
