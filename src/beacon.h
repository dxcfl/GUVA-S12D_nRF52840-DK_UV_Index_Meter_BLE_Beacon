/* beacon.h - BLE beacon */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 * Copyright (c) 2021 dxcfl
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _BEACON_H
#define _BEACON_H

#ifdef __cplusplus
extern "C"
{
#endif

    /*
     * Beacon start ...
     */
    void beacon_start();

    /*
     * Beacon update ...
     */
    void beacon_update(const char *name);

#ifdef __cplusplus
}
#endif

#endif /* _BEACON_H */