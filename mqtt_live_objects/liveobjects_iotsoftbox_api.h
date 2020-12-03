/*
 * Copyright (C) Orange
 *
 * This software is distributed under the terms and conditions of the 'MIT'
 * license which can be found in the file 'LICENSE.md' in this package distribution
 *
 * This file is a part of LiveObjects iotsoftbox-mqtt library.
 */

/**
 * @file  liveobjects_iotsoftbox_api.h
 *
 * @brief
 *
 */

#ifndef __liveobjects_iotsoftbox_api_H__
#define __liveobjects_iotsoftbox_api_H__

#ifndef __liveobjects_dev_params_H_
#error  "Parameter not defined, which means liveobjects_dev_params.h file not included"
#endif

/* LiveObjects Library interface*/
#include "LiveObjects-iotSoftbox-mqtt-core/liveobjects-client/LiveObjectsClient_Config.h"
#include "LiveObjects-iotSoftbox-mqtt-core/liveobjects-client/LiveObjectsClient_Defs.h"
#include "LiveObjects-iotSoftbox-mqtt-core/liveobjects-client/LiveObjectsClient_Core.h"
#include "LiveObjects-iotSoftbox-mqtt-core/liveobjects-client/LiveObjectsClient_Security.h"

/* Definitions set for this board or os.*/
#include "platforms/linux/liveobjects-sys/LiveObjectsClient_Platform.h"

#endif /*__liveobjects_iotsoftbox_api_H__*/
