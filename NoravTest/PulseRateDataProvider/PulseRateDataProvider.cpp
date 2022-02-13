#include "pch.h"
#include <cstdlib>
#include <ctime>

#define _PulseDataProviderDef
#include "PulseRateDataProvider.h"

#pragma comment(linker, "/export:StartPulseRateMonitor=_StartPulseRateMonitor@8")
#pragma comment(linker, "/export:StopPulseRateMonitor=_StopPulseRateMonitor@0")


static class CRandomGenerator
{
public:
	CRandomGenerator(long min, long max) : m_min(min), m_max(max) { std::srand(static_cast<unsigned>(std::time(0))); }
	long Get() { return m_min + (std::rand() % (m_max - m_min + 1)); }
private:
	long m_min, m_max;
} _rand(PRT_PULSE_RATE_MIN, PRT_PULSE_RATE_MAX);

static UINT_PTR _idTimer = 0;
static PF_ONPULSERATEDATA _pfCallback = nullptr;
static long _interval = PRT_PROBE_INTERVAL_0;
static int _count = 0;

static void CALLBACK _TimerProc(HWND hwnd, UINT uMsg, UINT_PTR timerId, DWORD dwTime)
{
	if (_pfCallback)
		_pfCallback(_rand.Get());
	if (++_count == PRT_TEST_DURATION / _interval)
	{
		if (_pfCallback)
			_pfCallback(PRT_FINISHED);
		StopPulseRateMonitor();
	}
}

extern "C" __declspec(dllexport) void WINAPI StartPulseRateMonitor(LONG nInterval, PF_ONPULSERATEDATA pfCallback)
{
	if (_idTimer)
		KillTimer(NULL, _idTimer);
	_interval = nInterval;
	_pfCallback = pfCallback;
	_pfCallback(_rand.Get());
	_idTimer = SetTimer(NULL, 1, _interval * 1000, _TimerProc);
}

extern "C" __declspec(dllexport) void WINAPI StopPulseRateMonitor()
{
	if (_idTimer)
	{
		KillTimer(NULL, _idTimer);
		_idTimer = 0;
	}
	_pfCallback = nullptr;
	_count = 0;
}