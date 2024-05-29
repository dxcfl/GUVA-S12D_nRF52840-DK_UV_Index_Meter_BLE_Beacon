/* beacon.c - BLE beacon */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 * Copyright (c) 2021 dxcfl
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>

#include <zephyr/logging/log.h>

#include "beacon.h"

LOG_MODULE_REGISTER(beacon, LOG_LEVEL_DBG);

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

/* Bluetooth beacon setup ...
 * "stolen" from the Zephyr bluetooth beacon example
 * (zephyr/samples/bluetooth/beacon/main.c).
 * Setup a non-connectable Eddystone beacon.
 * Later we will "abuse" the name data in the scan
 * resonse to transport the measure UV index.
 */

/*
 * Set Advertisement data. Based on the Eddystone specification:
 * https://github.com/google/eddystone/blob/master/protocol-specification.md
 * https://github.com/google/eddystone/tree/master/eddystone-url
 */
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0xaa, 0xfe),
	BT_DATA_BYTES(BT_DATA_SVC_DATA16,
				  0xaa, 0xfe, /* Eddystone UUID */
				  0x10,		  /* Eddystone-URL frame type */
				  0x00,		  /* Calibrated Tx power at 0m */
				  0x00,		  /* URL Scheme Prefix http://www. */
				  'e', 'x', 'a', 'm', 'p', 'l', 'e',
				  0x08) /* .org */
};

/* Set Scan Response data */
static const struct bt_data sd[] = {
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
	BT_DATA(BT_DATA_NAME_SHORTENED, DEVICE_NAME, DEVICE_NAME_LEN),
};

static void bt_ready(int err)
{
	char addr_s[BT_ADDR_LE_STR_LEN];
	bt_addr_le_t addr = {0};
	size_t count = 1;

	if (err)
	{
		LOG_ERR("Bluetooth init failed (err %d)", err);
		return;
	}
	LOG_INF("Bluetooth initialized");

	/* Start advertising */
	err = bt_le_adv_start(BT_LE_ADV_NCONN_IDENTITY, ad, ARRAY_SIZE(ad),
						  sd, ARRAY_SIZE(sd));
	if (err)
	{
		LOG_ERR("Advertising failed to start (err %d)", err);
		return;
	}

	/* For connectable advertising you would use
	 * bt_le_oob_get_local().  For non-connectable non-identity
	 * advertising an non-resolvable private address is used;
	 * there is no API to retrieve that.
	 */

	bt_id_get(&addr, &count);
	bt_addr_le_to_str(&addr, addr_s, sizeof(addr_s));

	LOG_INF("Bluetooth ready, advertising as %s", addr_s);
}

/*
 * Beacon start ...
 */
void beacon_start()
{
	/* Setup and start Bluetooth beacon
	 */
	int bt_err;
	LOG_INF("Starting beacon ...");
	bt_err = bt_enable(bt_ready);
	if (bt_err)
	{
		LOG_ERR("Beacon initiialization failed (err %d)", bt_err);
	}
}

/*
 * Beacon update ...
 */
void beacon_update(const char *name)
{
	int bt_err;

	struct bt_data new_sd[] = {
		BT_DATA(BT_DATA_NAME_COMPLETE, name, strlen(name)),
		BT_DATA(BT_DATA_NAME_SHORTENED, DEVICE_NAME, DEVICE_NAME_LEN),
	};
	LOG_DBG("Updating beacon with name %s", name);
	bt_err = bt_le_adv_update_data(ad, ARRAY_SIZE(ad),
								   new_sd, ARRAY_SIZE(new_sd));
	if (bt_err)
	{
		LOG_ERR("Advertising beacon update failed (err %d)", bt_err);
	}
}