/* main.c - UV index meter beacon w/ GUVA-S12SD UV sensor */

/* Copyright (c) 2024 dxcfl
 * Based on the Nordic SDK examples Copyright (c) 2024 Nordic Semiconductor ASA 
 * 
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/adc.h>

#include <GUVA_S12SD.h>
#include <beacon.h>

/* Auxiliary function: Format UV index value as a string.
 *
 * @param uv_index UV index value.
 *
 * @return UV index value as a string.
 */
static const char *uv_index_str(float uv_index)
{
	static char buf[16];
	snprintf(buf, sizeof(buf), "UV index: %.1f", uv_index);
	return buf;
}

/* Auxiliary function: Format UV index value as a string.
 *
 * @param uv_index UV index value.
 *
 * @return UV index value as a string.
 */
static const char *uv_index_short_str(float uv_index)
{
	static char buf[16];
	snprintf(buf, sizeof(buf), "UVI=%.1f", uv_index);
	return buf;
}

/* a variable of type adc_dt_spec for each channel ...*/
static const struct adc_dt_spec adc_channel = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

LOG_MODULE_REGISTER(UV_meter_beacon, LOG_LEVEL_DBG);

int main(void)
{
	int err;
	uint32_t count = 0;

	/* a variable of type adc_sequence and a buffer of type uint16_t ...*/
	int16_t buf;
	struct adc_sequence sequence = {
		.buffer = &buf,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(buf),
		// Optional
		//.calibrate = true,
	};

	/* validate that the ADC peripheral (SAADC) is ready ...*/
	if (!adc_is_ready_dt(&adc_channel))
	{
		LOG_ERR("ADC controller devivce %s not ready", adc_channel.dev->name);
		return 0;
	}
	/* setup the ADC channel ...*/
	err = adc_channel_setup_dt(&adc_channel);
	if (err < 0)
	{
		LOG_ERR("Could not setup channel #%d (%d)", 0, err);
		return 0;
	}
	/* initialize the ADC sequence ...*/
	err = adc_sequence_init_dt(&adc_channel, &sequence);
	if (err < 0)
	{
		LOG_ERR("Could not initalize sequnce");
		return 0;
	}

	/* start the BLE beacon ...*/
	LOG_INF("Starting BLE beacon ...");
	beacon_start();

	LOG_INF("Starting read & update loop ...");
	while (1)
	{
		int val_mv;

		/* read a sample from the ADC ...*/
		err = adc_read(adc_channel.dev, &sequence);
		if (err < 0)
		{
			LOG_ERR("Could not read (%d)", err);
			continue;
		}

		val_mv = (int)buf;
		LOG_INF("ADC reading[%u]: %s, channel %d: Raw: %d", count++, adc_channel.dev->name,
				adc_channel.channel_id, val_mv);

		/* convert raw value to mV ...*/
		err = adc_raw_to_millivolts_dt(&adc_channel, &val_mv);
		/* conversion to mV may not be supported, skip if not */
		if (err < 0)
		{
			LOG_WRN(" (value in mV not available)\n");
		}
		else
		{
			float uv_index;
			const char *beacon_name_short;
			const char *beacon_name_complete;

			LOG_INF(" = %d mV", val_mv);
			/* calculate UV index ...*/
			uv_index = GUVA_S12SD_uv_index(val_mv);
			LOG_INF("UV index = %.1f", uv_index);

			/* update the beacon's name ...*/
			beacon_name_short = uv_index_short_str(uv_index);
			beacon_name_complete = uv_index_str(uv_index);
			LOG_INF("Updating beacon name ...");
			beacon_update_names(beacon_name_complete,strlen(beacon_name_complete),beacon_name_short,strlen(beacon_name_short));
		}

		k_sleep(K_MSEC(1000));
	}
	return 0;
}
