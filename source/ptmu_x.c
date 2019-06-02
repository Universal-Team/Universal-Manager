/*
*   This file is part of Universal-Manager
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

// Based on ctrulib's ptmu.c.
#include <stdlib.h>
#include <3ds/types.h>
#include <3ds/result.h>
#include <3ds/svc.h>
#include <3ds/srv.h>
#include <3ds/synchronization.h>
#include <3ds/services/ptmu.h>
#include <3ds/ipc.h>

#include "ptmu_x.h"

#include <3ds.h>

static Handle mcuhwcHandle;

Result mcuInit(void)
{
    return srvGetServiceHandle(&mcuhwcHandle, "mcu::HWC");
}

Result mcuExit(void)
{
    return svcCloseHandle(mcuhwcHandle);
}

static Handle ptmuxHandle;
static int ptmuxRefCount;

Result ptmuxInit(void)
{
	if (AtomicPostIncrement(&ptmuxRefCount)) return 0;
	Result res = srvGetServiceHandle(&ptmuxHandle, "ptm:u");
	if (R_FAILED(res)) AtomicDecrement(&ptmuxRefCount);
	return res;
}

void ptmuxExit(void)
{
	if (AtomicDecrement(&ptmuxRefCount)) return;
	svcCloseHandle(ptmuxHandle);
}

Result PTMUX_GetAdapterState(u8 *out)
{
	Result ret=0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x5,0,0); // 0x50000

	if(R_FAILED(ret = svcSendSyncRequest(ptmuxHandle)))return ret;

	*out = (u8)cmdbuf[2] & 0xFF;

	return (Result)cmdbuf[1];
}

Result mcuGetBatteryLevel(u8* out)
{
    u32* ipc = getThreadCommandBuffer();
    ipc[0] = 0x50000;
    Result ret = svcSendSyncRequest(mcuhwcHandle);
    if(ret < 0) return ret;
	*out = ipc[2];
    return ipc[1];
}