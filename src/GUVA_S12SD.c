/*
 * Copyright (C) 2024  dxcfl
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * GUVA-S12SD auxilliary functions
 *
 */

#include <GUVA_S12SD.h> 

/* GUVA_S12SD_UV_INDEX
Function to convert sensor output voltage (mV) to UV index
for a GUVA-S12SD based analog UV sensor based on a conversion table.
See http://www.esp32learning.com/code/esp32-and-guva-s12sd-uv-sensor-example.php
for conversion table ...
*/
float GUVA_S12SD_uv_index(float mv)
{
    const float mv_uvi[12] = {50, 227, 318, 408, 503, 606, 696, 795, 881, 976, 1079, 1170};
    int i = 0;
    float uvi = 0;

    if ( mv < 1 ) return 0; // if mv is less than 1 mV, return 0

    for (i = 0; i < 12; i++)
    {
        if (mv < mv_uvi[i])
            break;
    }
    if (i < 11)
    {
        uvi = mv / mv_uvi[i] * (i + 1);
    }
    else
    {
        uvi = 11;
    }

    return uvi;
}

/* GUVA_S12SD_UV_LEVEL
Function to convert sensor output voltage (mV) to mW/m^2
for a GUVA-S12SD based analog UV sensor: "The output voltage is: Vo = 4.3 * Diode-Current-in-uA.
So if the photocurrent is 1uA (9 mW/cm^2), the output voltage is 4.3V."
*/
float GUVA_S12SD_uv_level(float mv)
{
    float uv_level = mv / 43 * 9; // 1 uA per 9 mW/cm^2 at 4.3V
    return uv_level;
}
