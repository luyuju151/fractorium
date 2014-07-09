#pragma once

#include "EmberDefines.h"

/// <summary>
/// Timing and CriticalSection classes.
/// </summary>

namespace EmberNs
{
/// <summary>
/// Since the algorithm is so computationally intensive, timing and benchmarking are an integral portion
/// of both the development process and the execution results. This class provides an easy way to time
/// things by simply calling its Tic() and Toc() member functions. It also assists with formatting the
/// elapsed time as a string.
/// </summary>
class EMBER_API Timing
{
public:
	/// <summary>
	/// Constructor that takes an optional precision argument which specifies how many digits after the decimal place should be printed for seconds.
	/// As a convenience, the Tic() function is called automatically.
	/// </summary>
	/// <param name="precision">The precision of the seconds field of the elapsed time. Default: 2.</param>
	Timing(int precision = 2)
	{
		m_Precision = precision;
		Init();
		Tic();
	}

	/// <summary>
	/// Set the begin time.
	/// </summary>
	/// <returns>The quad part of the begin time cast to a double</returns>
	double Tic()
	{
		QueryPerformanceCounter(&m_BeginTime);
		return BeginTime();
	}
 
	/// <summary>
	/// Set the end time and optionally output a string showing the elapsed time.
	/// </summary>
	/// <param name="str">The string to output. Default: NULL.</param>
	/// <param name="fullString">If true, output the string verbatim, else output the text " processing time: " in between str and the formatted time.</param>
	/// <returns>The elapsed time in milliseconds as a double</returns>
	double Toc(const char* str = NULL, bool fullString = false)
	{
		QueryPerformanceCounter(&m_EndTime);
		double ms = ElapsedTime();

		if (str != NULL)
		{
			cout << string(str) << (fullString ? "" : " processing time: ") << Format(ms) << endl;
		}

		return ms;
	}

	/// <summary>
	/// Return the quad part of the begin time as a double.
	/// </summary>
	/// <returns></returns>
	double BeginTime() { return (double)m_BeginTime.QuadPart; }

	/// <summary>
	/// Return the quad part of the end time as a double.
	/// </summary>
	/// <returns></returns>
	double EndTime() { return (double)m_EndTime.QuadPart; }

	/// <summary>
	/// Return the elapsed time in milliseconds.
	/// </summary>
	/// <returns>The elapsed time in milliseconds as a double</returns>
	double ElapsedTime() { return double(m_EndTime.QuadPart - m_BeginTime.QuadPart) * 1000.0 / double(m_Freq.QuadPart); }

	/// <summary>
	/// Formats a specified milliseconds value as a string.
	/// This uses some intelligence to determine what to return depending on how much time has elapsed.
	/// Days, hours and minutes are only included if 1 or more of them has elapsed. Seconds are always
	/// included as a decimal value with the precision the user specified in the constructor.
	/// </summary>
	/// <param name="ms">The ms</param>
	/// <returns>The formatted string</returns>
	string Format(double ms)
	{
		stringstream ss;
			
		double x = ms / 1000;
		double secs = fmod(x, 60);
		x /= 60;
		double mins = fmod(x, 60);
		x /= 60;
		double hours = fmod(x, 24);
		x /= 24;
		double days = x;

		if (days >= 1)
			ss << (int)days << "d ";

		if (hours >= 1)
			ss << (int)hours << "h ";

		if (mins >= 1)
			ss << (int)mins << "m ";

		ss << std::fixed << std::setprecision(m_Precision) << secs << "s";
		return ss.str();
	}

	/// <summary>
	/// Return the frequency of the clock as a double.
	/// </summary>
	/// <returns></returns>
	static double Freq()
	{
		Init();
		return (double)m_Freq.QuadPart;
	}

	/// <summary>
	/// Return the number of cores in the system.
	/// </summary>
	/// <returns>The number of cores in the system</returns>
	static int ProcessorCount()
	{
		Init();
		return m_ProcessorCount;
	}

private:
	/// <summary>
	/// Query and store the performance info of the system.
	/// Since it will never change it only needs to be queried once.
	/// This is achieved by keeping static state and performance variables.
	/// </summary>
	static void Init()
	{
		if (!m_TimingInit)
		{
			SYSTEM_INFO sysinfo;
 
			QueryPerformanceFrequency(&m_Freq);
			GetSystemInfo(&sysinfo);
			m_ProcessorCount = sysinfo.dwNumberOfProcessors;
			m_TimingInit = true;
		}
	}

	int m_Precision;//How many digits after the decimal place to print for seconds.
	LARGE_INTEGER m_BeginTime;//The start of the timing, set with Tic().
	LARGE_INTEGER m_EndTime;//The end of the timing, set with Toc().
	static bool m_TimingInit;//Whether the performance info has bee queried.
	static int m_ProcessorCount;//The number of cores on the system, set in Init().
	static LARGE_INTEGER m_Freq;//The clock frequency, set in Init().
};

/// <summary>
/// Cross platform critical section class which can be used for thread locking.
/// </summary>
class EMBER_API CriticalSection
{
#ifdef _WIN32

public:
	/// <summary>
	/// Constructor which initialized the underlying CRITICAL_SECTION object.
	/// </summary>
	CriticalSection() { InitializeCriticalSection(&m_CriticalSection); }

	/// <summary>
	/// Constructor which initialized the underlying CRITICAL_SECTION object
	/// with the specified spin count value.
	/// </summary>
	/// <param name="spinCount">The spin count.</param>
	CriticalSection(DWORD spinCount) { InitializeCriticalSectionAndSpinCount(&m_CriticalSection, spinCount); }
	
	/// <summary>
	/// Deletes the underlying CRITICAL_SECTION object.
	/// </summary>
	~CriticalSection() { DeleteCriticalSection(&m_CriticalSection); }

	/// <summary>
	/// Lock the critical section.
	/// </summary>
	void Enter() { EnterCriticalSection(&m_CriticalSection); }

	/// <summary>
	/// Unlock the critical section.
	/// </summary>
	void Leave() { LeaveCriticalSection(&m_CriticalSection); }

private:
	CRITICAL_SECTION m_CriticalSection;//The Windows specific critical section object.

#else

	/// <summary>
	/// Constructor which initialized the underlying pthread_mutex_t object.
	/// </summary>
	CriticalSection()
	{
		pthread_mutexattr_t attr;
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
		pthread_mutex_init(&m_CriticalSection, &attr);
		pthread_mutexattr_destroy(&attr);
	}

	/// <summary>
	/// Deletes the underlying pthread_mutex_t object.
	/// </summary>
	~CriticalSection() { pthread_mutex_destroy(&m_CriticalSection); }

	/// <summary>
	/// Lock the critical section.
	/// </summary>
	void Enter() { pthread_mutex_lock(&m_CriticalSection); }

	/// <summary>
	/// Unlock the critical section.
	/// </summary>
	void Leave() { pthread_mutex_unlock(&m_CriticalSection); }

private:
	pthread_mutex_t m_CriticalSection;//The *nix/pthread specific critical section object.

#endif
};
}