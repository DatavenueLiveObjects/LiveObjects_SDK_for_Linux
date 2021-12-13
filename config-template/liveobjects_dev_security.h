/*
 * Copyright (C) 2020 Orange
 *
 * This software is distributed under the terms and conditions of the 'MIT'
 * license which can be found in the file 'LICENSE.md' in this package distribution
 *
 * This file is a part of LiveObjects iotsoftbox-mqtt library.
 */

/**
 * @file  liveobjects_dev_security.h
 *
 * @brief Define certificates and TLS mode
 *
 */
#ifndef __liveobjects_dev_security_H_
#define __liveobjects_dev_security_H_

#define VERIFY_MODE   0

/*#define SERVER_CERTS          NULL*/
#define CLIENT_CERT           NULL
#define CLIENT_PKEY           NULL
#define CLIENT_PKEY_PASSWORD  0

#define SERVER_CERTIFICATE_COMMON_NAME     "mqtt.liveobjects.orange-business.com"

#ifndef SERVER_CERTS
#define SERVER_CERTS "/etc/ssl/certs/ca-certificates.crt"
#endif

#ifndef CLIENT_CERT
#define CLIENT_CERT "-----BEGIN CERTIFICATE-----\n" \
        "-----END CERTIFICATE-----\n"
#endif

#ifndef CLIENT_PKEY

#if CLIENT_PKEY_PASSWORD
const char* getLomPkeyPassword(void);
#endif

#define CLIENT_PKEY[] "-----BEGIN PRIVATE KEY-----\n" \
        "-----END PRIVATE KEY-----\n"

#endif /* CLIENT_PKEY*/

#endif /* __liveobjects_dev_security_H_*/
