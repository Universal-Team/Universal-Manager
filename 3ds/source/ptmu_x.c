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