#
# Copyright (c) 2024 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(guva-s12sd_uv-meter_beacon)

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
