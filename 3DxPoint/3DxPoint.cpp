// 3DxPoint.cpp : Defines the exported functions for the DLL application. Derivative of 3DxSample from 3DConnexion.
// SPDX-FileCopyrightText: © 2023 Kelvin Afolabi @glodigit
// SPDX-License-Identifier: Boost Software License 1.0

/* ToDo List:
	- 	Clean up old comments and unused variables
	-	change triggerEvent to SpaceState.ButtonEvent
	*/

#include "pch.h"
#include "stdlib.h"
#include "stdio.h"
#include <complex>
#include <future>
#define LOGFILE_ENABLED (false)
#if LOGFILE_ENABLED
#include <string>
#endif
#pragma comment(lib,"winmm.lib")
#include "mmsystem.h"

#define LOG_PATH L"C:\\ProgramData\\3Dconnexion\\3DxPoint\\3DxPoint.txt"
#if 1 // Change to 0 to mute
#define NOTE_C_PATH  L"C:\\ProgramData\\3Dconnexion\\3DxPoint\\Pizzicato C.wav"
#define NOTE_D_PATH  L"C:\\ProgramData\\3Dconnexion\\3DxPoint\\Pizzicato D.wav"
#define NOTE_E_PATH  L"C:\\ProgramData\\3Dconnexion\\3DxPoint\\Pizzicato E.wav"
#define NOTE_G_PATH  L"C:\\ProgramData\\3Dconnexion\\3DxPoint\\Pizzicato G.wav"
#endif

// #define TXT_PATH L"C:\\Users\\[PUT_USERNAME_HERE]\\source\\repos\\3DxPoint\\3DxPoint\\3DxPoint.txt"
#define OPEN_LOGFILE_SUCCESSFUL (_wfopen_s(&fp, LOG_PATH, L"a,ccs=UTF-8") == 0 && fp != NULL)
#define INT_ARGS _wtoi(wcsrchr(args, L' '))
//		read like (int)*args




enum class PC
{
	none,
	// Add / rename PC names / identifiers below
	Teti,
	Q3ti
};
/// <summary>
/// The PC to create the DLL for.
/// Usually used to compile different virtual buttons for different PCs.
/// </summary>
const PC PC_Select = PC::Teti;

// Angle conversions
const double pi = std::acos(-1);
const double toRadians = pi / 180.0;
const double toDegrees = 180.0 / pi;

typedef struct SpaceState
{
	std::complex<double> Mouse; // In the form y + x i
	int Scroll{};
	double Speed = 1;
	std::complex<double> ButtonRing;
	std::complex<double> PrevButtonRing;
	std::complex<double> ButtonEvent;
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
		fwprintf(fp, L"ButtonRing: %s Event: %4d ∠ %-4d  (%-4d + %4d i)\n",
				 realOrImag ? L"Imag" : L"Real",
				 (int)std::round(std::abs(SpacePoint.ButtonRing)),
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

extern "C" __declspec(dllexport) void LogButton(WCHAR *args)
{
#if LOGFILE_ENABLED
	static int logButtonCount = 0;
	logButtonCount++;
	FILE *fp;
	if OPEN_LOGFILE_SUCCESSFUL
	{
		fwprintf(fp, L"Button Event %d: %s\n", logButtonCount, args);
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
		input.mi.mouseData = eventValue;
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

// @brief Function generated by BingChat to find if p is between v1 and v2
bool doubleInRange(double p, double v1, double v2)
{
	return v1 <= v2 ? v1 <= p && p <= v2 : v2 <= p && p <= v1;
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
	static bool passedThreshold[3] = { false, false, false };

	const UINT entryThreshold[3] = {45, 135, 270};
	const double exitThreshold = 18;
	const int cardinalAngle = 20, anglePadding = 1;
	const double triggerDifference = 2; // E.G: +10 will mean 39 -> 50 -> 59 -> trigger.

	double currentMagnitude = std::abs(SpacePoint.ButtonRing);
	double prevMagnitude = std::abs(SpacePoint.PrevButtonRing);
	bool onEntry = false, onExit = false;

	/// Rising / falling edge of Schmitt trigger
	if (currentMagnitude < prevMagnitude + triggerDifference &&
		currentMagnitude > exitThreshold &&
		prevMagnitude >= entryThreshold[0] &&
		std::abs(SpacePoint.ButtonEvent) == 0.0)
	{

#if LOGFILE_ENABLED
		FILE *fp;
		if OPEN_LOGFILE_SUCCESSFUL
		{
			fwprintf(fp, L"ButtonRing: Entry Event: Current Magnitude: %4d, Prev Magnitude: %4d\n",
					 (int)std::round(currentMagnitude),
					 (int)std::round(prevMagnitude));
			fclose(fp);
		}
#endif
		SpacePoint.ButtonEvent = SpacePoint.ButtonRing;
		onEntry = true;
	}
	else if (currentMagnitude < exitThreshold)
	{
		onExit = true;
	}
	else if (std::abs(SpacePoint.ButtonEvent) == 0.0) {
		if (!passedThreshold[0] && currentMagnitude >= entryThreshold[0]) {
			PlaySound(NOTE_D_PATH, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
			passedThreshold[0] = true;
		}
		else if (!passedThreshold[1] && currentMagnitude >= entryThreshold[1]) {
			PlaySound(NOTE_E_PATH, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
			passedThreshold[1] = true;
		}
		else if (!passedThreshold[2] && currentMagnitude >= entryThreshold[2]) {
			PlaySound(NOTE_G_PATH, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
			passedThreshold[2] = true;
		}
	}

	SpacePoint.PrevButtonRing = SpacePoint.ButtonRing;

	/// Dealing with an event
	double triggerMagnitude = std::abs(SpacePoint.ButtonEvent);
	if (triggerMagnitude != 0) // A trigger point has been obtained
	{
		bool isEdge = (triggerMagnitude >= entryThreshold[2]);
		bool isOuter = (triggerMagnitude >= entryThreshold[1]);
		bool inGap = false;
		double angle = toDegrees * std::arg(SpacePoint.ButtonEvent);
		const int a = anglePadding, b = cardinalAngle, c = 90 - b;

		if (doubleInRange(angle, 180 - b / 2 + a, 180) ||
			doubleInRange(angle, -180, -180 + b / 2 - a))
		{ // South
			if (isEdge)
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: South Edge: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: South Edge: Down\n");
				}
			}
			else if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: Outer South: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: Outer South: Down\n");
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: South: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: South: Down\n");
				}
			}
		}
		else if (doubleInRange(angle, -135 - c / 2 + a, -135 + c / 2 - a))
		{ // South-west
			if (isEdge)
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: South-west Edge: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: South-west Edge: Down\n");
				}
			}
			else if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: Outer South-west: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: Outer South-west: Down\n");
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: South-west: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: South-west: Down\n");
				}
			}
		}
		else if (doubleInRange(angle, -90 - b / 2 + a, -90 + b / 2 - a))
		{ // West
			if (isEdge)
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: West Edge: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: West Edge: Down\n");
				}
			}
			else if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: Outer West: Up\n");
					SendMouseEvent(MouseEvent::right, 0);
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: Outer West: Down\n");
					SendMouseEvent(MouseEvent::right, 1);
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: West: Up\n");
					SendMouseEvent(MouseEvent::left, 0);
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: West: Down\n");
					SendMouseEvent(MouseEvent::left, 1);
				}
			}
		}
		else if (doubleInRange(angle, -45 - c / 2 + a, -45 + c / 2 - a))
		{ // North-west
			if (isEdge)
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: North-west Edge: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: North-west Edge: Down\n");
				}
			}
			else if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: Outer North-west: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: Outer North-west: Down\n");
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: North-west: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: North-west: Down\n");
				}
			}
		}
		else if (doubleInRange(angle, 0 - b / 2 + a, 0 + b / 2 - a))
		{ // North
			if (isEdge)
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: North Edge: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: North Edge: Down\n");
				}
			}
			else if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: Outer North: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: Outer North: Down\n");
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: North: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: North: Down\n");
				}
			}
		}
		else if (doubleInRange(angle, 45 - c / 2 + a, 45 + c / 2 - a))
		{ // North East
			if (isEdge)
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: North-east Edge: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: North-east Edge: Down\n");
				}
			}
			else if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: Outer North-east: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: Outer North-east: Down\n");
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: North-east: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: North-east: Down\n");
				}
			}
		}
		else if (doubleInRange(angle, 90 - b / 2 + a, 90 + b / 2 - a))
		{ // East
			if (isEdge)
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: East Edge: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: East Edge: Down\n");
				}
			}
			else if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: Outer East: Up\n");
					Mirror((WCHAR *)L" 1");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: Outer East: Down\n");
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: East: Up\n");
					SendMouseEvent(MouseEvent::middle, 0);
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: East: Down\n");
					SendMouseEvent(MouseEvent::middle, 1);
				}
			}
		}
		else if (doubleInRange(angle, 135 - c / 2 + a, 135 + c / 2 - a))
		{ // South-east
			if (isEdge)
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: South-east Edge: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: South-east Edge: Down\n");
				}
			}
			else if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: Outer South-east: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: Outer South-east: Down\n");
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t *)L"ButtonRing: South-east: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t *)L"ButtonRing: South-east: Down\n");
				}
			}
		}
		else
		{ // Gap between boundaries
			inGap = true;
			onExit = true;
#if LOGFILE_ENABLED
			FILE *fp;
			if OPEN_LOGFILE_SUCCESSFUL
			{
				fwprintf(fp, L"ButtonRing: Gap: %4d ∠ %-4d\n",
						 (int)std::abs(SpacePoint.ButtonEvent),
						 (int)std::round(toDegrees * std::arg(SpacePoint.ButtonEvent)));
				fclose(fp);
			}
#endif
		}
		if (onExit)
		{
			if (!inGap) {
				PlaySound(NOTE_C_PATH, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
				for (int i = 0; i < 3; i++) {
					passedThreshold[i] = false;
				}
			}
			SpacePoint.ButtonEvent = (0.0, 0.0);			
		}
	}
}

extern "C" __declspec(dllexport) void SetButtonRingReal(WCHAR *args)
{
	// Get string that starts with ' ' -> convert to int -> set complex number
	SpacePoint.ButtonRing.real(INT_ARGS);
	// LogButtonRingEvent(0);

	SelectButtonOnRing();
}
extern "C" __declspec(dllexport) void SetButtonRingImag(WCHAR *args)
{
	SpacePoint.ButtonRing.imag(INT_ARGS * (SpacePoint.MirrorRing ? -1 : 1));
	// LogButtonRingEvent(1);

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

/// Archived code
#if 1
#endif