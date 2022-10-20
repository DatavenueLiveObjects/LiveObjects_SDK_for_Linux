/*
 * Copyright (C) Orange
 *
 * This software is distributed under the terms and conditions of the 'MIT'
 * license which can be found in the file 'LICENSE.md' in this package distribution
 */

/**
 * @file  amb_dist.c
 * @brief An ambient light & distance user application using basic LiveObjects
 * iotsotbox-mqtt features
 */

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

/* Raspberry Pi GPIO */
#include <wiringPi.h>
#include <wiringPiI2C.h>
/* VL6180X */
#include <vl6180_pi.h>

/* User params/configs */
#include "config/liveobjects_dev_params.h"
#include "config/liveobjects_dev_config.h"

/* LiveObjects api */
#include "liveobjects_iotsoftbox_api.h"

/* Default LiveObjects device settings : name space and device identifier*/
#define LOC_CLIENT_DEV_NAME_SPACE            "Linux"
#define IFACE	"eth0"

/** Here, set your LiveObject Apikey. It is mandatory to run the application
 *
 * C_LOC_CLIENT_DEV_API_KEY_P1 must be the first sixteen char of the ApiKey
 * C_LOC_CLIENT_DEV_API_KEY_P1 must be the last sixteen char of the ApiKey
 *
 * If your APIKEY is 0123456789abcdeffedcba9876543210 then
 * it should look like this :
 *
 * #define C_LOC_CLIENT_DEV_API_KEY_P1			0x0123456789abcdef
 * #define C_LOC_CLIENT_DEV_API_KEY_P2			0xfedcba9876543210
 *
 * */

#define C_LOC_CLIENT_DEV_API_KEY_P1			0x0123456789abcdef
#define C_LOC_CLIENT_DEV_API_KEY_P2			0xfedcba9876543210

/* Debug */
#define DBG_DFT_MAIN_LOG_LEVEL 3
#define DBG_DFT_LOMC_LOG_LEVEL 3

// set 0x0F to have all message dump (text+hexa)
#define DBG_DFT_MSG_DUMP         0xf

/* Various tag */
#define APPV_VERSION "LINUX AMBIENT LIGHT & DISTANCE SAMPLE V01.1"
#define LOM_BUILD_TAG "BUILD LiveObjects IoT Ambient Light & Distance 1.1"

uint8_t appv_log_level = DBG_DFT_MAIN_LOG_LEVEL;

//API key validation
#if C_LOC_CLIENT_DEV_API_KEY_P1 == 0x0123456789abcdef || C_LOC_CLIENT_DEV_API_KEY_P2 == 0xfedcba9876543210
# error "API key has default value, which is just an placeholder, please change it to valid api key"
#elif (!defined(C_LOC_CLIENT_DEV_API_KEY_P1) || !defined(C_LOC_CLIENT_DEV_API_KEY_P2))
#  error "API key not defined"
#endif

// ==========================================================
//
// Live Objects IoT Client object (using iotsoftbox-mqtt library)
//
// - Send status information at connection
// - Send collected data each cycle
//

// ----------------------------------------------------------
// STATUS data
//

// Count the number of time the status is sent
int32_t appv_status_counter = 0;

// Message to display as a status in LO
char appv_status_message[150] = "READY";

// VL6180 sensor handle
vl6180 handle;

/// Set of status
LiveObjectsD_Data_t appv_set_status[] = {
		{ LOD_TYPE_STRING_C, "sample_version", APPV_VERSION, 1 },
		{ LOD_TYPE_INT32, "sample_counter", &appv_status_counter, 1 },
		{ LOD_TYPE_STRING_C, "sample_message", appv_status_message, 1 }
};
#define SET_STATUS_NB (sizeof(appv_set_status) / sizeof(LiveObjectsD_Data_t))

int appv_hdl_status = -1;

// ----------------------------------------------------------
// COLLECTED data
//

#define STREAM_PREFIX 0

uint8_t appv_measures_enabled = 1;
// Data
uint32_t appv_measures_counter = 0;
char appv_measures_amb_light[16] = "0.0";
uint32_t appv_measures_distance = -1;

/// Set of Collected data (published on a data stream)
LiveObjectsD_Data_t appv_set_measures[] = {
		{ LOD_TYPE_UINT32, "counter", &appv_measures_counter, 1 },
		{ LOD_TYPE_STRING_C, "ambient_light", appv_measures_amb_light, 1 },
		{ LOD_TYPE_UINT32, "distance", &appv_measures_distance, 1 }
};
#define SET_MEASURES_NB (sizeof(appv_set_measures) / sizeof(LiveObjectsD_Data_t))

int appv_hdl_data = -1;

// ----------------------------------------------------------

int read_vl6180x_data(vl6180 handle) {
	int ret = 0;

	appv_measures_distance = get_distance(handle);
	float ambient_light = get_ambient_light(handle, GAIN_1);

	if ((appv_measures_distance >= 0) & (ambient_light >= 0.0)) {
		sprintf(appv_measures_amb_light, "%.2f", ambient_light);	// send as string w/ 2 dec. places
		printf("\nData are Ok, updating values\n\n");
		ret = 1;
	} else {
		printf("\nData aren't good, keeping the previous values\n\n");
		ret = 0;
	}
	return ret;
}

// ----------------------------------------------------------

uint32_t loop_cnt = 0;

void appli_sched(void) {
	++loop_cnt;
	appv_measures_counter = loop_cnt;
	if (appv_log_level > 1) {
		printf("thread_appli: %u\r\n", loop_cnt);
	}

	/* Read data from the VL6180X */
	/* if data are ok, copy them, then push */
	if (read_vl6180x_data(handle)) {
		if (appv_log_level > 2) {
			printf("thread_appli: %u - %s PUBLISH - distance=%d mm, ambient light=%s lx\r\n", loop_cnt,
					appv_measures_enabled ? "DATA" : "NO", appv_measures_distance, appv_measures_amb_light);
		}
		if (appv_measures_enabled) {
			printf("LiveObjectsClient_PushData...\n");
			LiveObjectsClient_PushData(appv_hdl_data);
		}
	}
}

// ----------------------------------------------------------

bool mqtt_start(void *ctx) {

	char mac[12];
	get_mac(IFACE, mac);

	LiveObjectsClient_SetDbgLevel(appv_log_level);
	LiveObjectsClient_SetDevId(mac);
	LiveObjectsClient_SetNameSpace(LOC_CLIENT_DEV_NAME_SPACE);

	unsigned long long apikey_p1 = C_LOC_CLIENT_DEV_API_KEY_P1;
	unsigned long long apikey_p2 = C_LOC_CLIENT_DEV_API_KEY_P2;

	printf("mqtt_start: LiveObjectsClient_Init ...\n");
	if(LiveObjectsClient_Init(NULL, apikey_p1, apikey_p2)) {
		printf("mqtt_start: ERROR returned by LiveObjectsClient_Init\n");
		return false;
	}

	// Attach my local STATUS data to the LiveObjects Client instance
	// --------------------------------------------------------------
	appv_hdl_status = LiveObjectsClient_AttachStatus(appv_set_status, SET_STATUS_NB);
	if (appv_hdl_status) {
		printf(" !!! ERROR (%d) to attach status !\r\n", appv_hdl_status);
	} else {
		printf("mqtt_start: LiveObjectsClient_AttachStatus -> OK\n");
	}

	// Attach one set of collected data to the LiveObjects Client instance
	// --------------------------------------------------------------------
	appv_hdl_data = LiveObjectsClient_AttachData(STREAM_PREFIX, "LO_sample_measures", "mV1", "\"Test\"",
			NULL, appv_set_measures, SET_MEASURES_NB);
	if (appv_hdl_data < 0) {
		printf(" !!! ERROR (%d) to attach a collected data stream !\r\n", appv_hdl_data);
	} else {
		printf("mqtt_start: LiveObjectsClient_AttachData -> OK\n");
	}

	// Connect to the LiveObjects Platform and update the status
	// ---------------------------------------------------------
	printf("mqtt_start: LiveObjectsClient_Connect ...\n");

	if (LiveObjectsClient_Connect()) {
		printf("mqtt_start: ERROR returned by LiveObjectsClient_Connect\n");
		return false;
	}

	if (LiveObjectsClient_PushStatus(appv_hdl_status)) {
		printf("mqtt_start: ERROR returned by LiveObjectsClient_PushStatus\n");
	}

	printf("mqtt_start: OK\n");
	return true;
}

// ----------------------------------------------------------
/// Entry point to the program

int main() {
	int ret = 0;

	printf("Starting LiveObject Client Example\n");
	LiveObjectsClient_InitDbgTrace(appv_log_level);
	LiveObjectsClient_SetDbgMsgDump(DBG_DFT_MSG_DUMP);

	if (wiringPiSetup() == -1) {
		printf("Failed to setup wiringPi, exiting now !");
		ret = -1;
	}

	if ((handle = vl6180_initialise(1)) < 0) {
		ret -2;
	}

	if (ret == 0 && mqtt_start(NULL)) {
		while (1) {
			appli_sched();
			/* Launch the cycle every 5 sec */
			LiveObjectsClient_Cycle(5000);
		}
	}

	return ret;
}
