/*
 * Copyright (C) Orange
 *
 * This software is distributed under the terms and conditions of the 'MIT'
 * license which can be found in the file 'LICENSE.md' in this package distribution
 */

/**
 * @file   mqtt_network_interface.h
 * @brief  Define structure used/required by the MQTTClient soft package
 */

#ifndef __MQTT_NETWORK_INTERFACE_H_
#define __MQTT_NETWORK_INTERFACE_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include "liveobjects-sys/MQTTLinux.h"
#include "liveobjects-sys/socket_defs.h"

typedef struct Network Network;

#if defined(__cplusplus)
}
#endif

#endif /* __MQTT_NETWORK_INTERFACE_H_ */
