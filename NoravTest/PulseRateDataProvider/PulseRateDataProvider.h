#pragma once

#include "../Shared/inc/GlobalDefs.h"

#define PRT_FINISHED	INT_MIN

typedef void (CALLBACK *PF_ONPULSERATEDATA)(long);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _PulseDataProviderDef
	__declspec(dllexport)
#endif
void WINAPI StartPulseRateMonitor(LONG nInterval, PF_ONPULSERATEDATA pfCallback);

#ifdef _PulseDataProviderDef
	__declspec(dllexport)
#endif
void WINAPI StopPulseRateMonitor();

#ifdef __cplusplus
}
#endif
