// 3DxPoint.cpp : Defines the exported functions for the DLL application. Derivative of 3DxSample from 3DConnexion.
// SPDX-FileCopyrightText: © 2023 Kelvin Afolabi @glodigit
// SPDX-License-Identifier: Boost Software License 1.0

/* ToDo List:
	- 	Make Set functions for all variables in SpaceState
	*/

#include "pch.h"
#include "stdlib.h"
#include "stdio.h"
#include <complex>
#define LOGFILE_ENABLED (false)
#if LOGFILE_ENABLED
#include <string>
#endif

#define TXT_PATH L"C:\\Log 3DxPoint\\3DxPoint.txt"
// #define TXT_PATH L"C:\\Users\\[PUT_USERNAME_HERE]\\source\\repos\\3DxPoint\\3DxPoint\\3DxPoint.txt"
#define OPEN_LOGFILE_SUCCESSFUL (_wfopen_s(&fp, TXT_PATH, L"a,ccs=UTF-8") == 0 && fp != NULL)
#define INT_ARGS _wtoi(wcsrchr(args, L' '))
//		read like (int)*args

// Angle conversions
const double pi = std::acos(-1);
const double toRadians = pi / 180.0;
const double toDegrees = 180.0 / pi;

typedef struct SpaceState
{
	// int MouseX{};
	// int MouseY{};
	int Scroll{};
	std::complex<double> Mouse; // In the form y + x i
	double Speed = 1;
	//bool Button[8][2]{}; // A near and far set of virtual buttons
	std::complex<double> ButtonRing;
	bool MirrorScroll{};
	bool MirrorRing{};
} SpaceState;

SpaceState SpacePoint;

enum class MouseEvent
{
	x,
	y,
	xy,
	scroll,
	left,
	middle,
	right
};

/// DLL functions
#if 1
#define LOG_PROCESSINFO
#ifdef LOG_PROCESSINFO
typedef struct UdCfgDLLProcessInfo
{
	DWORD size;
	DWORD processInfoType;
	DWORD pid;
	DWORD syncID;
	wchar_t exeName[MAX_PATH];
} UdCfgDLLProcessInfo;
#endif


// DLL Initialization function.
// This gives you a chance to do any one time initialization immediately after this DLL is loaded.
// It is called immediately after the DllMain is called (which is called during LoadLibrary).
// Return anything other than ERROR_SUCCESS causes this DLL to be unloaded (w/o calling DllExit).
extern "C" __declspec(dllexport) LRESULT DllInit(LPVOID lpReserved)
{
#if LOGFILE_ENABLED
	FILE *fp;
	if OPEN_LOGFILE_SUCCESSFUL
	{

#ifdef LOG_PROCESSINFO
		UdCfgDLLProcessInfo *pInfo = (UdCfgDLLProcessInfo *)lpReserved;
		fwprintf(fp, L"DllInit: ProcessInfo: size=%d, type=%d, pid=%d, syncID=%d, exeName=%s\n", pInfo->size, pInfo->processInfoType, pInfo->pid, pInfo->syncID, pInfo->exeName);
#else
		fwprintf(fp, L"DllInit called\n");
#endif
		fclose(fp);
	}
#endif

	return ERROR_SUCCESS;
}

// DLL Termination function.
// This gives you a chance to do some cleanup immediately before this DLL is unloaded.
// Return anything other than ERROR_SUCCESS causes an error to be logged, but the DLL is unloaded anyway.
// This function is not called if DllInit returns non-ERROR_SUCCESS.
extern "C" __declspec(dllexport) LRESULT DllExit(LPVOID lpReserved)
{
#if LOGFILE_ENABLED
	FILE *fp;
	if OPEN_LOGFILE_SUCCESSFUL
	{

#ifdef LOG_PROCESSINFO
		UdCfgDLLProcessInfo *pInfo = (UdCfgDLLProcessInfo *)lpReserved;
		fwprintf(fp, L"DllExit: ProcessInfo: size=%d, type=%d, pid=%d, exeName=%s\n", pInfo->size, pInfo->processInfoType, pInfo->pid, pInfo->exeName);
#else
		fwprintf(fp, L"DllExit called\n");
#endif
		fclose(fp);
	}
#endif

	return ERROR_SUCCESS;
}
#endif

/// Logging functions/exports
#if 1
void LogMessage(wchar_t *s)
{
#if LOGFILE_ENABLED
	FILE *fp;
	if OPEN_LOGFILE_SUCCESSFUL
	{
		fwprintf(fp, s);
		fclose(fp);
	}
#endif
}
/// @brief Call to log when a ButtonRingEvent has occurred.
/// @param realOrImag 0 for real, 1 for imaginary.
void LogButtonRingEvent(bool realOrImag)
{
#if LOGFILE_ENABLED
	FILE *fp;
	if OPEN_LOGFILE_SUCCESSFUL
	{
		fwprintf(fp, L"ButtonRing %s Event: %4d ∠ %-4d  (%-4d + %4d i)\n",
				 realOrImag ? L"Imag" : L"Real",
				 (int)std::abs(SpacePoint.ButtonRing),
				 (int)std::round(toDegrees * std::arg(SpacePoint.ButtonRing)),
				 (int)SpacePoint.ButtonRing.real(),
				 (int)SpacePoint.ButtonRing.imag());
		fclose(fp);
	}
#endif
}

/// @brief Call to log when a MirrorEvent has occurred.
/// @param scrollOrRing 0 for scroll, 1 for ring.
void LogMirrorEvent(bool scrollOrRing)
{
#if LOGFILE_ENABLED
	FILE *fp;
	if OPEN_LOGFILE_SUCCESSFUL
	{
		fwprintf(fp, L"Mirror %s Event: %d\n",
				 scrollOrRing ? L"Ring" : L"Scroll",
				 scrollOrRing ? (int)SpacePoint.MirrorRing : (int)SpacePoint.MirrorScroll);
		fclose(fp);
	}
#endif
}

void LogSpeedEvent()
{
#if LOGFILE_ENABLED
	FILE *fp;
	if OPEN_LOGFILE_SUCCESSFUL
	{
		fwprintf(fp, L"Speed Multiplier Event: %f\n", SpacePoint.Speed);
		fclose(fp);
	}
#endif
}

// This is an example of an exported function.
extern "C" __declspec(dllexport) void LogAxis(WCHAR *args)
{
#if LOGFILE_ENABLED
	FILE *fp;
	if OPEN_LOGFILE_SUCCESSFUL
	{
		fwprintf(fp, L"Axis Event: %s\n", args);
		fclose(fp);
	}
#endif
}

#if LOGFILE_ENABLED
static int logCount = 0;
#endif
extern "C" __declspec(dllexport) void LogButton(WCHAR *args)
{
#if LOGFILE_ENABLED
	logCount++;
	FILE *fp;
	if OPEN_LOGFILE_SUCCESSFUL
	{
		fwprintf(fp, L"Button Event %d: %s\n", logCount, args);
		fclose(fp);
	}
#endif
}
#endif

/// SendInput functions
#if 1

/// @brief Sends the relevant mouse-related button or ultra precision scroll event.
/// @param eventType
/// @param eventValue For mouse events, pass 0 for up button. For classic scroll, multiply by WHEEL_DATA.
static void SendMouseEvent(MouseEvent eventType, int eventValue)
{
	UINT status;
	INPUT input;
	const UINT nInputs = 1;

	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	switch (eventType)
	{
	case MouseEvent::left:
		input.mi.dwFlags = eventValue ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
		break;
	case MouseEvent::middle:
		input.mi.dwFlags = eventValue ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;
		break;
	case MouseEvent::right:
		input.mi.dwFlags = eventValue ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;
		break;
	case MouseEvent::scroll:
		input.mi.dwFlags = MOUSEEVENTF_WHEEL;
		input.mi.mouseData = eventValue; //  *WHEEL_DATA
		break;
	case MouseEvent::x:
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		input.mi.dx = (long)std::round(eventValue * SpacePoint.Speed);
		break;
	case MouseEvent::y:
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		input.mi.dy = (long)std::round(eventValue * SpacePoint.Speed);
		break;
	case MouseEvent::xy:
		// eventValue not used
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		input.mi.dx = (long)std::round(SpacePoint.Mouse.imag() * SpacePoint.Speed);
		input.mi.dy = (long)std::round(SpacePoint.Mouse.real() * SpacePoint.Speed);
		break;
	}
	/*	This doesn't work if the window under the cursor is running as admin,
		but 3DxService is not run as admin! */
	status = ::SendInput(nInputs, &input, sizeof(INPUT));

	if (status != nInputs)
	{
#if LOGFILE_ENABLED
		FILE *fp;
		if OPEN_LOGFILE_SUCCESSFUL
		{
			DWORD error = GetLastError();
			std::string eventName = "";
			switch (eventType)
			{
			case MouseEvent::left:
				eventName = "Left Button";
				break;
			case MouseEvent::middle:
				eventName = "Middle Button";
				break;
			case MouseEvent::right:
				eventName = "Right Button";
				break;
			case MouseEvent::scroll:
				eventName = "Scroll";
				break;
			case MouseEvent::x:
				eventName = "X";
				break;
			case MouseEvent::y:
				eventName = "Y";
				break;
			case MouseEvent::xy:
				eventName = "XY";
				break;
			default:
				eventName = "Unknown";
				break;
			}
			fwprintf(fp, L"%s SendInput Error = 0x%x\n", (wchar_t *)eventName.c_str(), error);
			fclose(fp);
		}
#endif
	}
}

void SendModifierKey(WORD wVk, bool pressed)
{
	UINT status;
	INPUT input;
	const UINT nInputs = 1;

	ZeroMemory(&input, sizeof(INPUT));

	input.type = INPUT_KEYBOARD;
	input.ki.wVk = wVk;
	input.ki.wScan = 0;
	input.ki.dwFlags = pressed ? 0 : KEYEVENTF_KEYUP;
	input.ki.time = 0;
	input.ki.dwExtraInfo = NULL; // hopefully don't need this

	status = ::SendInput(nInputs, &input, sizeof(INPUT));

#if 0 // This seems like an easy way to toggle bits of code on and off without having to comment it out
	if (status != nInputs)
	{
		DWORD error = GetLastError();
		//::LogMessageEx(LogLevelErrors, _T("SendModifierkey: Error %d from SendInput sending key %d\n"), error, wVk);
	}

	else
	{
		::LogMessageEx(LogLevelErrors, _T("SendModifierkey: NO Error from SendInput sending key %d %s\n"), wVk, bReleased ? L"release" : L"press");
	}
#endif
}

#endif

/// Set SpacePoint functions / exports
void SelectButtonOnRing(void);
#if 1
extern "C" __declspec(dllexport) void SetButtonRingReal(WCHAR *args)
{
	// Get string that starts with ' ' -> convert to int -> set complex number
	SpacePoint.ButtonRing.real(INT_ARGS);
	//LogButtonRingEvent(0);

	SelectButtonOnRing();
}
extern "C" __declspec(dllexport) void SetButtonRingImag(WCHAR *args)
{
	SpacePoint.ButtonRing.imag(INT_ARGS * (SpacePoint.MirrorRing ? -1 : 1));
	//LogButtonRingEvent(1);

	SelectButtonOnRing();
}
/// <summary>
/// Set the ButtonRing value using a single function call for the _X, _Y, _Rx or _Ry axes.
/// Mainly for testing / debug purposes.
/// </summary>
/// <param name="args"></param>
extern "C" __declspec(dllexport) void SetButtonRing(WCHAR *args)
{
	if (wcsstr(args, L"_Rx") != NULL || wcsstr(args, L"_Y") != NULL)
	{
		// Tilting / moving towards positive Y should result in a 0 degree
		// angle, instead of the traditional 90.
		SetButtonRingReal(args);
	}
	else if (wcsstr(args, L"_Ry") != NULL || wcsstr(args, L"_X") != NULL)
	{
		SetButtonRingImag(args);
	}
}

extern "C" __declspec(dllexport) void MirrorRing(WCHAR *args)
{
	if (INT_ARGS)
	{
		SpacePoint.MirrorRing = !SpacePoint.MirrorRing;
		LogMirrorEvent(1);
	}
}

extern "C" __declspec(dllexport) void MirrorScroll(WCHAR *args)
{
	if (INT_ARGS)
	{
		SpacePoint.MirrorScroll = !SpacePoint.MirrorScroll;
		LogMirrorEvent(0);
	}
}

extern "C" __declspec(dllexport) void Mirror(WCHAR *args)
{
	MirrorRing(args);
	MirrorScroll(args);
}

// #define USE_XY
extern "C" __declspec(dllexport) void SetMouseX(WCHAR *args)
{
	SpacePoint.Mouse.imag(INT_ARGS);
#ifdef USE_XY
	SendMouseEvent(MouseEvent::xy, 0); // eventValue not used
#else
	SendMouseEvent(MouseEvent::x, (int)SpacePoint.Mouse.imag());
#endif
}

extern "C" __declspec(dllexport) void SetMouseY(WCHAR *args)
{
	SpacePoint.Mouse.real(INT_ARGS);
#ifdef USE_XY
	SendMouseEvent(MouseEvent::xy, 0); // eventValue not used
#else
	SendMouseEvent(MouseEvent::y, (int)SpacePoint.Mouse.real());
#endif
}

extern "C" __declspec(dllexport) void SetScroll(WCHAR *args)
{
	SpacePoint.Scroll =
		(int)INT_ARGS * (SpacePoint.MirrorScroll ? -1 : 1)
		//* std::pow(SpacePoint.Speed, 0.75);
		* SpacePoint.Speed;

	SendMouseEvent(MouseEvent::scroll, SpacePoint.Scroll);
}

/// <summary>
/// For setting the speed of mouse / scroll events.
/// </summary>
/// <param name="args"></param>
extern "C" __declspec(dllexport) void SetSpeed(WCHAR *args)
{
	const UINT axisMax = 350;
	double speed = 1 + (double)INT_ARGS / axisMax;
	const double minSpeed = 0.03;
	const double maxSpeed = 6;
	const double exponent = 3; // Keep greater than 0

	// m = 0.8 x^2 + 0.2 (for example)
	// m = (1-y)x^2 + y
	SpacePoint.Speed = //(speed < 1) ?
		(1 - minSpeed) * std::pow(speed, exponent) + minSpeed
	//:
#if 0
		(minSpeed - 1)
		* std::pow(2 - speed, exponent)
		+ (2 - minSpeed)

		(1 - maxSpeed)
		* std::pow(2 - speed, exponent)
		+ maxSpeed
#endif
		;

	// LogSpeedEvent();
}

#endif


// @brief Function generated by BingChat to find if p is between v1 and v2
bool doubleInRange(double p, double v1, double v2) {
	return v1 <= v2 ? 
		v1 <= p && p <= v2: 
		v2 <= p && p <= v1;
}

#if 0
// The one I wrote
bool doubleInRange(double value, double minValue, double maxValue)
{
	return (minValue <= value) && (value <= maxValue);
}
#endif


void SelectButtonOnRing()
{
	/*
	 * so if magnitude isn't 0, start a timer, else stop and reset the timer
	 * alternatively, magnitude - maxMagnitude <= 0
	 * if the value is greater than entryThreshold, check each i = [0, 45, 90...] +/- angleTolerance
	 *	if i > 180, switch to j = ( i - 360 ) else just use i
	 * if j >= eTol[1] then k = outer else if >= eTol[0] then k = inner else k = none
	 * 00 -> 35 -> 34 should capture a triggerEvent, then the angle should be determined and
	 * 27 -> 17 -> 00 should sendInput a pressUp event, set triggerEvent to 0 and octal = 0
	 */

	enum class RingType
	{
		inner,
		outer,
		none
	};
	enum class Octant
	{
		N,
		NE,
		E,
		SE,
		S,
		SW,
		W,
		NW,
	};
	//Octant octant;

	static double prevMagnitude = 0;
	static std::complex<double> triggerEvent;
	// static bool onExit = false;
	//static bool buttonAlreadyPressed = false;

	const UINT entryThreshold[2] = {45, 135};
	const double exitThreshold = 15;
	const int anglePadding = 2;

	double currentMagnitude = std::abs(SpacePoint.ButtonRing);
	bool onEntry = false, onExit = false;

	if (currentMagnitude < prevMagnitude &&
		currentMagnitude > exitThreshold &&
		prevMagnitude >= entryThreshold[0] &&
		std::abs(triggerEvent) == 0)
	{
		triggerEvent = SpacePoint.ButtonRing;
		onEntry = true;
	}
	else if (currentMagnitude < exitThreshold) { 
		onExit = true;
	}
	

#if 0 //LOGFILE_ENABLED
	FILE* fp;
	if OPEN_LOGFILE_SUCCESSFUL
	{
		fwprintf(fp, L"Prev Magnitude: %4d\n",
				 (int)std::round(prevMagnitude));
		fclose(fp);
	}
#endif

	double triggerMagnitude = std::abs(triggerEvent);
	if (triggerMagnitude != 0) // A trigger point has been obtained
	{
		bool isOuter = (triggerMagnitude >= entryThreshold[1]);
		double angle = toDegrees * std::arg(triggerEvent);
		const int a = anglePadding;

		// Ordered clockwise from south
		if (doubleInRange(angle, 180 - 45 / 2 + a, 180) ||
			doubleInRange(angle, -180, -180 + 45 / 2 - a))
		{
			// octant = Octant::S;
			if (isOuter)
			{ 
				if (onExit)
				{
					LogMessage((wchar_t*)L"Button Ring: Outer South: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"Button Ring: Outer South: Down\n");

				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"Button Ring: South: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"Button Ring: South: Down\n");
				}
			}
		}
		else if (doubleInRange(angle, -135 - 45 / 2 + a, -135 + 45 / 2 - a))
		{
			//octant = Octant::SW;
			if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"Button Ring: Outer South-west: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"Button Ring: Outer South-west: Down\n");
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"Button Ring: South-west: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"Button Ring: South-west: Down\n");
				}
			}
		}
		else if (doubleInRange(angle, -90 - 45 / 2 + a, -90 + 45 / 2 - a))
		{
			//octant = Octant::W;
			if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"Button Ring: Outer West: Up\n");
					SendMouseEvent(MouseEvent::right, 0);

				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"Button Ring: Outer West: Down\n");
					SendMouseEvent(MouseEvent::right, 1);

				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"Button Ring: West: Up\n");
					SendMouseEvent(MouseEvent::left, 0);

				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"Button Ring: West: Down\n");
					SendMouseEvent(MouseEvent::left, 1);


				}
			}
		}
		else if (doubleInRange(angle, -45 - 45 / 2 + a, -45 + 45 / 2 - a))
		{
			//octant = Octant::NW;
			if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"Button Ring: Outer North-west: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"Button Ring: Outer North-west: Down\n");
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"Button Ring: North-west: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"Button Ring: North-west: Down\n");
				}
			}
		}
		else if (doubleInRange(angle, 0 - 45 / 2 + a, 0 + 45 / 2 - a))
		{
			//octant = Octant::N;
			if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"Button Ring: Outer North: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"Button Ring: Outer North: Down\n");
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"Button Ring: North: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"Button Ring: North: Down\n");
				}
			}
		}
		else if (doubleInRange(angle, 45 - 45 / 2 + a, 45 + 45 / 2 - a))
		{
			//octant = Octant::NE;
			if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"Button Ring: Outer North-east: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"Button Ring: Outer North-east: Down\n");
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"Button Ring: North-east: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"Button Ring: North-east: Down\n");
				}
			}
		}
		else if (doubleInRange(angle, 90 - 45 / 2 + a, 90 + 45 / 2 - a))
		{
			//octant = Octant::E;
			if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"Button Ring: Outer East: Up\n");
					Mirror((WCHAR*)L" 1");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"Button Ring: Outer East: Down\n");
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"Button Ring: East: Up\n");
					SendMouseEvent(MouseEvent::middle, 0);

				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"Button Ring: East: Down\n");
					SendMouseEvent(MouseEvent::middle, 1);

				}
			}
		}
		else if (doubleInRange(angle, 135 - 45 / 2 + a, 135 + 45 / 2 - a))
		{
			//octant = Octant::SE;
			if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"Button Ring: Outer South-east: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"Button Ring: Outer South-east: Down\n");
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"Button Ring: South-east: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"Button Ring: South-east: Down\n");
				}
			}
		}
		else // Likely padding gap.
		{
			// LogMessage(L"Trigger Event: Gap");
			onExit = true;
#if LOGFILE_ENABLED
			FILE *fp;
			if OPEN_LOGFILE_SUCCESSFUL
			{
				fwprintf(fp, L"Trigger Gap Event: %4d ∠ %-4d\n",
						 (int)std::abs(triggerEvent),
						 (int)std::round(toDegrees * std::arg(triggerEvent)));
				fclose(fp);
			}
#endif

			// triggerEvent = (0.0, 0.0);
			// return;
		}
		// debug to test octants
		// triggerEvent = (0.0, 0.0);

		// Commands to execute at the end, no matter the ButtonRing angle, 
		// instead of having to copy/paste these lines 8 times over. It's like
		// end GCODEs in 3D printing.
		if (onExit)
		{
			// Reset variables
			triggerEvent = (0.0, 0.0);
			//buttonAlreadyPressed = false;

		}
		//else if (!buttonAlreadyPressed) { buttonAlreadyPressed = true; }
	}
	//else {
		prevMagnitude = currentMagnitude;

//	}

}

/// Archived code
#if 1

/// Old SendMouseButton functions
#if 0
static void SendLeftMouseButton(bool press)
{
	UINT status;
	INPUT input;
	const UINT nInputs = 1;

	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = press ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;

	/******** This doesn't work if the window under the cursor is running as admin, but 3DxService is not run as admin *********/
	status = ::SendInput(nInputs, &input, sizeof(INPUT));

	if (status != nInputs)
	{
		DWORD error = GetLastError();
#ifdef TRACE
		// fwprintf(fp, L"SendLeftMouseButton SendInput Error = 0x%x\n", error);
#endif
	}
}

static void SendRightMouseButton(bool press)
{
	UINT status;
	INPUT input;
	const UINT nInputs = 1;

	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = press ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;

	/******** This doesn't work if the window under the cursor is running as admin, but 3DxService is not run as admin *********/
	status = ::SendInput(nInputs, &input, sizeof(INPUT));

	if (status != nInputs)
	{
		DWORD error = GetLastError();
#ifdef TRACE
		// fwprintf(fp, L"SendRightMouseButton SendInput Error = 0x%x\n", error);
#endif
	}
}

static void SendMiddleMouseButton(bool press)
{
	UINT status;
	INPUT input;
	const UINT nInputs = 1;

	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = press ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;

	/******** This doesn't work if the window under the cursor is running as admin, but 3DxService is not run as admin *********/
	status = ::SendInput(nInputs, &input, sizeof(INPUT));

	if (status != nInputs)
	{
		DWORD error = GetLastError();
#ifdef TRACE
		// fwprintf(fp, L"SendMiddleMouseButton SendInput Error = 0x%x\n", error);
#endif
	}
}

static void SendMouseWheel(int increment)
{
	UINT status;
	INPUT input;
	const UINT nInputs = 1;

	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_WHEEL;
	input.mi.mouseData = increment; //  *WHEEL_DATA??

	/******** This doesn't work if the window under the cursor is running as admin, but 3DxService is not run as admin *********/
	status = ::SendInput(nInputs, &input, sizeof(INPUT));

	if (status != nInputs)
	{
		DWORD error = GetLastError();
#ifdef TRACE
		// fwprintf(fp, L"SendRightMouseButton SendInput Error = 0x%x\n", error);
#endif
	}
}

#endif

// Unused code for holding mouse buttons with the axes from <Axis/> in the XML
#if 0
static bool HoldingLMB = false;
extern "C" __declspec(dllexport) void LeftMouseButton_PressAndHold(WCHAR *args)
{
	// TO BE SAFE, SHOULD CALL THIS WITH 0 on DLLEXIT.
	// It also doesn't detect if the focus is changed to another app.  It will leave the button pressed.  Which may or may not be desired.

	int threshold = 300; // positive, expecting AxisAction to carve out where this is executed, and the abs(axisValue) to be used below

#ifdef TRACE
	FILE *fp;
	if (_wfopen_s(&fp, TXT_PATH, L"a") == 0 && fp != NULL)
	{
		fwprintf(fp, L"LeftMouseButton_PressAndHold(%d) Event: %s\n", threshold, args);
	}
#endif

	WCHAR *axisValueStr = wcsrchr(args, L' ');
	int axisValue = _wtoi(axisValueStr);
	axisValue = abs(axisValue);

	if (HoldingLMB == true && axisValue > threshold)
	{
// Button is pressed, wait until it is released
#ifdef TRACE
		fwprintf(fp, L"LeftMouseButton_PressAndHold: HoldingLMB while axisValue > %d)\n", threshold);
		fclose(fp);
#endif
		return;
	}

	else if (HoldingLMB == true && axisValue < threshold)
	{
// Button is pressed, release it
#ifdef TRACE
		fwprintf(fp, L"LeftMouseButton_PressAndHold: axisValue < %d, SendLeftMouseButton(false)\n", threshold);
		fclose(fp);
#endif
		HoldingLMB = false;
		SendLeftMouseButton(false);
	}

	else if (HoldingLMB == false && axisValue > threshold)
	{
// Button is not pressed, press and hold it
#ifdef TRACE
		fwprintf(fp, L"LeftMouseButton_PressAndHold: SendLeftMouseButton(true)\n");
		fclose(fp);
#endif
		HoldingLMB = true;
		SendLeftMouseButton(true);
	}

#ifdef TRACE
	fclose(fp);
#endif
}

static bool HoldingRMB = false;
extern "C" __declspec(dllexport) void RightMouseButton_PressAndHold(WCHAR *args)
{
	// TO BE SAFE, SHOULD CALL THIS WITH 0 on DLLEXIT
	// It also doesn't detect if the focus is changed to another app.  It will leave the button pressed.  Which may or may not be desired.

	int threshold = 300; // positive, expecting AxisAction to carve out where this is executed, and the abs(axisValue) to be used below
						 // #define TRACE
#ifdef TRACE
	FILE *fp;
	if (_wfopen_s(&fp, TXT_PATH, L"a") == 0 && fp != NULL)
	{
		// fwprintf(fp, L"RightMouseButton_PressAndHold(%d) Event: %s\n", threshold, args);
	}
#endif

	WCHAR *axisValueStr = wcsrchr(args, L' ');
	int axisValue = _wtoi(axisValueStr);
	axisValue = abs(axisValue);

	if (HoldingRMB == true && axisValue > threshold)
	{
		// Button is pressed, wait until it is released
#ifdef TRACE
		fwprintf(fp, L"RightMouseButton_PressAndHold: HoldingRMB while axisValue > %d)\n", threshold);
		fclose(fp);
#endif
		return;
	}

	else if (HoldingRMB == true && axisValue < threshold)
	{
		// Button is pressed, release it
#ifdef TRACE
		fwprintf(fp, L"RightMouseButton_PressAndHold: axisValue < %d, SendRightMouseButton(false)\n", threshold);
		fclose(fp);
#endif
		HoldingRMB = false;
		SendRightMouseButton(false);
	}

	else if (HoldingRMB == false && axisValue > threshold)
	{
		// Button is not pressed, press and hold it
#ifdef TRACE
		fwprintf(fp, L"RightMouseButton_PressAndHold: SendRightMouseButton(true)\n");
		fclose(fp);
#endif
		HoldingRMB = true;
		SendRightMouseButton(true);
	}

#ifdef TRACE
	fclose(fp);
#endif
}
#endif
#endif