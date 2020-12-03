/*
 * Copyright (C) Orange
 *
 * This software is distributed under the terms and conditions of the 'MIT'
 * license which can be found in the file 'LICENSE.md' in this package distribution
 */

/**
 * @file  MQTTLog.c
 * @brief Wrapper for MQTTPacket logs
 *        (see MQTTPacket/StackTrace.h)
 */

#include <stdio.h>

#define TR_OUPUT printf

void StackTrace_entry(const char *name, int line, int trace) {
#ifdef TR_OUPUT
	TR_OUPUT("MQTT_ENTRY:%s:%d\n", name, line);
#endif
}

void StackTrace_exit(const char *name, int line, void *return_value, int trace) {
#ifdef TR_OUPUT
	TR_OUPUT("MQTT_EXIT :%s:%d: return=%d\n", name, line,
			(return_value) ? *(int *) return_value : 0);
#endif
}

#if 0
void StackTrace_printStack(FILE* dest) {
}
char* StackTrace_get(unsigned long) {
}
#endif
