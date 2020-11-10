/*
 * Copyright (C) 2020 Orange
 *
 * This software is distributed under the terms and conditions of the 'MIT'
 * license which can be found in the file 'LICENSE.md' in this package distribution
 *
 * This file is a part of LiveObjects iotsoftbox-mqtt library.
 */

 /**
  * @file  liveobjects_dev_params.h
  * @brief Default user parameters used to configure this device as a Live Objects Device
  */

 #ifndef __liveobjects_dev_params_H_
 #define __liveobjects_dev_params_H_

 /* Set to 1 to enable TLS feature*/
 /* (warning: check that LOC_SERV_PORT is the correct port in this case)*/
 #define SECURITY_ENABLED                     1

 /* Only used to overwrite the LiveOjects Server settings :*/
 /* IP address, TCP port, Connection timeout in milliseconds.*/
 //#define LOC_SERV_IP_ADDRESS                  "84.39.42.214"
 //#define LOC_SERV_PORT                        8883
 //#define LOC_SERV_TIMEOUT                     2000

 /* 0 -> standard output, 1 -> syslog output /var/log/syslog */
 #define SYSLOG 0

 #if SECURITY_ENABLED
 /* If security is enabled to establish connection to the LiveObjects platform,*/
 /* include certificates file*/
 #include "liveobjects_dev_security.h"
 #endif

 #endif /*__liveobjects_dev_params_H_*/
