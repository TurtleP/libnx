/**
 * @file clkrst.h
 * @brief Clkrst service IPC wrapper.
 * @author p-sam
 * @copyright libnx Authors
 */
#pragma once
#include "../types.h"
#include "../services/sm.h"
#include "../services/pcv.h"

typedef struct {
    Service  s;
} ClkrstSession;

Result clkrstInitialize(void);
void clkrstExit(void);
Service* clkrstGetServiceSession(void);

/// Opens a ClkrstSession for the requested PcvModuleId, unk is set to 3 in official sysmodules.
Result clkrstOpenSession(ClkrstSession* session_out, PcvModuleId module_id, u32 unk);
void clkrstCloseSession(ClkrstSession* session);
Result clkrstSetClockRate(ClkrstSession* session, u32 hz);
Result clkrstGetClockRate(ClkrstSession* session, u32 *out_hz);