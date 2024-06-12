/*
 * Copyright (C) 2024  dxcfl
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * GUVA-S12SD auxilliary functions
 *
 */

#ifndef _GUVA_S12SD_H
#define _GUVA_S12SD_H

/* GUVA_S12SD_UV_INDEX
Function to convert sensor output voltage (mV) to UV index
for a GUVA-S12SD based analog UV sensor based on a conversion table.
See http://www.esp32learning.com/code/esp32-and-guva-s12sd-uv-sensor-example.php
for conversion table ...
*/
float GUVA_S12SD_uv_index(float mv);

/* GUVA_S12SD_UV_LEVEL
Function to convert sensor output voltage (mV) to mW/m^2
for a GUVA-S12SD based analog UV sensor: "The output voltage is: Vo = 4.3 * Diode-Current-in-uA.
So if the photocurrent is 1uA (9 mW/cm^2), the output voltage is 4.3V."
*/
float GUVA_S12SD_uv_level(float mv);

#endif // _GUVA_S12SD_H