#include "pch.h"
#include "framework.h"
#include <numeric>
#include "PulseDataProvider.h"

IPulseDataConsumer* CPulseDataProvider::s_pConsumer = nullptr;
std::vector<long> CPulseDataProvider::s_data;

void CALLBACK CPulseDataProvider::OnPulseData(long val)
{
	if (val == PRT_FINISHED)
	{
		if (s_pConsumer)
			s_pConsumer->OnTestFinished();
	}
	else
	{
		s_data.push_back(val);
		if (s_pConsumer)
			s_pConsumer->OnPulseData(val);
	}
}

void CPulseDataProvider::Start(IPulseDataConsumer* pConsumer, long nInterval /*= PRT_PROBE_INTERVAL_0*/)
{
	s_data.clear();
	s_pConsumer = pConsumer;
	StartPulseRateMonitor(nInterval, OnPulseData);
}
void CPulseDataProvider::Stop()
{
	StopPulseRateMonitor();
}

bool CPulseDataProvider::HasData()
{
	return !s_data.empty();
}

long CPulseDataProvider::Last()
{
	return HasData() ? s_data.back() : 0;
}

long CPulseDataProvider::Average()
{
	if (HasData())
	{
		return std::accumulate(s_data.begin(), s_data.end(), 0) / s_data.size();
	}
	return 0;
}

bool CPulseDataProvider::IsNormal()
{
	long val = Last();
	return !HasData() || (val >= PRT_NORMAL_RATE_MIN && val <= PRT_NORMAL_RATE_MAX);
}
