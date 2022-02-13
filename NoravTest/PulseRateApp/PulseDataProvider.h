#pragma once

#include <vector>
#include "../PulseRateDataProvider/PulseRateDataProvider.h"

class IPulseDataConsumer
{
public:
	virtual void OnPulseData(long val) = 0;
	virtual void OnTestFinished() = 0;
};

class CPulseDataProvider
{
public:
	static void Start(IPulseDataConsumer* pConsumer, long nInterval = PRT_PROBE_INTERVAL_0);
	static void Stop();
	static bool HasData();
	static long Last();
	static long Average();
	static bool IsNormal();
private:
	static void CALLBACK OnPulseData(long val);
	static IPulseDataConsumer* s_pConsumer;
	static std::vector<long> s_data;
};