﻿// 3DxPoint.cpp : Defines the exported functions for the DLL application. Derivative of 3DxSample from 3DConnexion.
// SPDX-FileCopyrightText: © 2023 Kelvin Afolabi @glodigit
// SPDX-License-Identifier: Boost Software License 1.0

/* ToDo List:
	- 	Implement averaging feature for mouse events
	*/

#include "pch.h"
#include "3DxPoint.h"

SpaceState SpacePoint;

/// DLL functions
#if 1
#if LOG_PROCESSINFO
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
	FILE* fp;
	if OPEN_LOGFILE_SUCCESSFUL
	{

#ifdef LOG_PROCESSINFO
		UdCfgDLLProcessInfo * pInfo = (UdCfgDLLProcessInfo*)lpReserved;
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
	FILE* fp;
	if OPEN_LOGFILE_SUCCESSFUL
	{

#ifdef LOG_PROCESSINFO
		UdCfgDLLProcessInfo * pInfo = (UdCfgDLLProcessInfo*)lpReserved;
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
void LogMessage(wchar_t* s)
{
#if LOGFILE_ENABLED
	FILE* fp;
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
	if (!doubleInRange(std::abs(SpacePoint.ButtonRing), 1, 10)) { // filter out weak noise, but keep 0's

#if LOGFILE_ENABLED
		FILE* fp;
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
}

/// @param realOrImag 0 for real, 1 for imaginary.
void LogMouseEvent(bool realOrImag)
{

#if LOGFILE_ENABLED
		FILE* fp;
		if OPEN_LOGFILE_SUCCESSFUL
		{
			fwprintf(fp, L"Mouse: %s Event: %4d ∠ %-4d  (%-4d + %4d i)\n",
					 realOrImag ? L"Imag" : L"Real",
					 (int)std::round(std::abs(SpacePoint.Mouse)),
					 (int)std::round(toDegrees * std::arg(SpacePoint.Mouse)),
					 (int)SpacePoint.Mouse.real(),
					 (int)SpacePoint.Mouse.imag());
			fclose(fp);
		}
#endif
}

/// @brief Call to log when a MirrorEvent has occurred.
/// @param scrollOrRing 0 for scroll, 1 for ring.
void LogMirrorEvent(bool scrollOrRing)
{
#if LOGFILE_ENABLED
	FILE* fp;
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
	FILE* fp;
	if OPEN_LOGFILE_SUCCESSFUL
	{
		fwprintf(fp, L"Speed Multiplier Event: %f\n", SpacePoint.Speed);
		fclose(fp);
	}
#endif
}

// This is an example of an exported function.
extern "C" __declspec(dllexport) void LogAxis(WCHAR * args)
{
#if LOGFILE_ENABLED
	FILE* fp;
	if OPEN_LOGFILE_SUCCESSFUL
	{
		fwprintf(fp, L"Axis Event: %s\n", args);
		fclose(fp);
	}
#endif
}

extern "C" __declspec(dllexport) void LogButton(WCHAR * args)
{
#if LOGFILE_ENABLED
	static int logButtonCount = 0;
	logButtonCount++;
	FILE* fp;
	if OPEN_LOGFILE_SUCCESSFUL
	{
		fwprintf(fp, L"Button Event %d: %s\n", logButtonCount, args);
		fclose(fp);
	}
#endif
}
#endif
/// Mirroring externals
#if 1 
extern "C" __declspec(dllexport) void MirrorRing(WCHAR * args)
{
	if (INT_ARGS)
	{
		SpacePoint.MirrorRing = !SpacePoint.MirrorRing;
		LogMirrorEvent(1);
	}
}

extern "C" __declspec(dllexport) void MirrorScroll(WCHAR * args)
{
	if (INT_ARGS)
	{
		SpacePoint.MirrorScroll = !SpacePoint.MirrorScroll;
		LogMirrorEvent(0);
	}
}

extern "C" __declspec(dllexport) void Mirror(WCHAR * args)
{
	MirrorRing(args);
	MirrorScroll(args);
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

	static bool reflectOn = false;
	if (eventType == MouseEvent::x || eventType == MouseEvent::y || eventType == MouseEvent::scroll) {
#if 0
		if (std::abs(eventValue) < 20) {
			reflectOn = false; 
			return;
		}
		else if (eventType != MouseEvent::scroll && std::abs(SpacePoint.Mouse) < std::abs(SpacePoint.PrevMouse) - 18) { reflectOn = true; }
		if (reflectOn) eventValue *= 0.25;
#endif
	}

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
		input.mi.dx = (long)std::round((SpacePoint.Mouse.imag() + SpacePoint.PrevMouse.imag())/2 * SpacePoint.Speed);
		input.mi.dy = (long)std::round((SpacePoint.Mouse.real() + SpacePoint.PrevMouse.real())/2 * SpacePoint.Speed);
		break;
		//default: return;
	}
	/*	This doesn't work if the window under the cursor is running as admin,
		but 3DxService is not run as admin! */
	status = SendInput(nInputs, &input, sizeof(INPUT));

	if (status != nInputs)
	{
#if LOGFILE_ENABLED
		FILE* fp;
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
			fwprintf(fp, L"%s SendInput Error = 0x%x\n", (wchar_t*)eventName.c_str(), error);
			fclose(fp);
		}
#endif
	}
}

/// Shortcuts
#if 1

/* Use https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes or run the below code that
	BingChat generated in a new C++ Console App project.

#include <iostream>
#include <windows.h>
int main() {
	while (true) {
		for (int i = 1; i < 256; i++) {
			if (GetAsyncKeyState(i) & 0x8000) {
				std::cout << "Virtual key code: " << i << std::endl;
				Sleep(500); // Wait for 500 milliseconds
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					return 0; // Exit the program if the Escape key is held
				}
			}
		}
	}
	return 0;
}

*/
//   

// Macros to make writing these shortcut macros faster / more concise
#define KB ki.wVk =// Key Button
#define KBUP ki.dwFlags = KEYEVENTF_KEYUP
#define KBSETUP ZeroMemory(k, sizeof(k)); for (int i = 0; i < ARRAYSIZE(k); i++)  {k[i].type = INPUT_KEYBOARD; }
#define KBSEND if(SendInput(ARRAYSIZE(k), k, sizeof(INPUT)) != ARRAYSIZE(k))

void TapKey(UINT8 virtualKey) {
	INPUT k[2] = {};
	KBSETUP;
	k[0].KB virtualKey;
	k[1].KBUP; k[1].KB virtualKey;
	KBSEND
	{
		//LogMessage((wchar_t*)L"Keyboard SendInput Error.\n");
	}
}

/// <summary>
/// A keyboard shortcut for snipping a section of the screen.
/// </summary>
/// <param name="pc">Optional override to the main PC selection.</param>
void Shortcut_Snip(PC pc = PC_Select) {
	if (pc == PC::Typical)
	{// Win + Shift + S (Snip + Sketch)
		INPUT k[6] = {};
		KBSETUP;
		k[0].KB VK_LWIN;
		k[1].KB VK_LSHIFT;
		k[2].KB 'S';
		k[3].KBUP; k[3].KB 'S';
		k[4].KBUP; k[4].KB VK_LSHIFT;
		k[5].KBUP; k[5].KB VK_LWIN;
		KBSEND{}
	}
	else if (pc == PC::Computer1)
	{// LCtrl + UK-Grave '`'
		INPUT k[4] = {};
		KBSETUP;
		k[0].KB VK_LCONTROL;
		k[1].KB VK_OEM_8;
		k[2].KBUP; k[2].KB VK_OEM_8;
		k[3].KBUP; k[3].KB VK_LCONTROL;
		KBSEND{}
	}
	else if (pc == PC::Computer2)
	{// RCtrl + ForwardSlash
		INPUT k[4] = {};
		KBSETUP;
		k[0].KB VK_RCONTROL;
		k[1].KB VK_OEM_2;
		k[2].KBUP; k[2].KB VK_OEM_2;
		k[3].KBUP; k[3].KB VK_RCONTROL;
		KBSEND{}
	}
}

void Shortcut_BrowserCloseTab(PC pc = PC_Select) {
	INPUT k[4] = {};
	KBSETUP;
	k[0].KB VK_LCONTROL;
	k[1].KB 'W';
	k[2].KBUP; k[2].KB 'W';
	k[3].KBUP; k[3].KB VK_LCONTROL;
	KBSEND{}
}

void Shortcut_BrowserNextTab(PC pc = PC_Select) {
	if (pc == PC::Minimal || pc == PC::Typical || pc==PC::Computer2) {
		INPUT k[4] = {};
		KBSETUP;
		k[0].KB VK_LCONTROL;
		k[1].KB VK_TAB;
		k[2].KBUP; k[2].KB VK_TAB;
		k[3].KBUP; k[3].KB VK_LCONTROL;
		KBSEND{}
	}
	else if (pc == PC::Computer1) { TapKey(VK_F9); }
}

void Shortcut_BrowserPrevTab(PC pc = PC_Select) {
	if (pc == PC::Minimal || pc == PC::Typical || pc == PC::Computer2) {
		INPUT k[6] = {};
		KBSETUP;
		k[0].KB VK_LCONTROL;
		k[1].KB VK_LSHIFT;
		k[2].KB VK_TAB;
		k[3].KBUP; k[3].KB VK_TAB;
		k[4].KBUP; k[4].KB VK_LSHIFT;
		k[5].KBUP; k[5].KB VK_LCONTROL;
		KBSEND{}
	}
	else if (pc == PC::Computer1) { TapKey(VK_F8); }
}



#endif
#endif

double MagnitueMultiplier(double angle) {
	// Number from 0 to 1. Diagonals produce magnitudes greater than 350, so this is set as the 
	// minimum value to multiply by to normalise the magnitudes
	const double DiagonalMultiplier = 348.0 / 424.0;
	const double x = 2 * (1 - DiagonalMultiplier) / (1 + DiagonalMultiplier);
	return (2 + x * std::cos(4 * angle)) / (2 + x);
}

void SelectButtonOnRing()
{
	static bool passedThreshold[3] = { false, false, false };
	bool onEntry = false, onExit = false;

	const UINT entryThreshold[3] = { 42, 124, 248 };
	const double exitThreshold = 10;
	const int cardinalAngle = 30, anglePadding = 1;
	const double triggerDifference = -1; // E.G: +10 will mean 39 -> 50 -> 59 -> trigger.
	const bool soundOnTrigger = false; // false = sound when threshold passed

#if 1
	double currentMagnitude = 
		std::abs(SpacePoint.ButtonRing) * MagnitueMultiplier(std::arg(SpacePoint.ButtonRing));
	double prevMagnitude = max(	
		std::abs(SpacePoint.PrevButtonRing[0]) * MagnitueMultiplier(std::arg(SpacePoint.PrevButtonRing[0])),
		std::abs(SpacePoint.PrevButtonRing[1]) * MagnitueMultiplier(std::arg(SpacePoint.PrevButtonRing[1]))
		);
#endif

#if 0
	double currentMagnitude =
		std::abs(SpacePoint.ButtonRing);
	double prevMagnitude = max(
		std::abs(SpacePoint.PrevButtonRing[0]),
		std::abs(SpacePoint.PrevButtonRing[1])
	);
#endif

	/// Rising / falling edge of Schmitt trigger
	if (currentMagnitude < prevMagnitude + triggerDifference &&
		currentMagnitude > exitThreshold &&
		prevMagnitude >= entryThreshold[0] &&
		std::abs(SpacePoint.ButtonEvent) == 0.0)
	{
#if LOGFILE_ENABLED
			FILE* fp;
			if OPEN_LOGFILE_SUCCESSFUL
			{
				fwprintf(fp, L"ButtonRing: Entry Event: Current Magnitude: %4d, Prev Magnitude: %4d\n",
						 (int)std::round(currentMagnitude),
						 (int)std::round(prevMagnitude));
				fclose(fp);
			}
#endif
		
		SpacePoint.ButtonEvent = 
			std::abs(SpacePoint.PrevButtonRing[0]) >= std::abs(SpacePoint.PrevButtonRing[1]) ?
			SpacePoint.PrevButtonRing[0] : SpacePoint.PrevButtonRing[1];
		SpacePoint.ButtonEvent *= MagnitueMultiplier(std::arg(SpacePoint.ButtonEvent));
		onEntry = true;
	}
	else if (currentMagnitude < exitThreshold)
	{
		onExit = true;
	}
	else if (std::abs(SpacePoint.ButtonEvent) == 0.0) {
		if (!passedThreshold[0] && currentMagnitude >= entryThreshold[0]) {
			if (!soundOnTrigger) PlaySound(NOTE_D_PATH, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
			passedThreshold[0] = true;
		}
		else if (!passedThreshold[1] && currentMagnitude >= entryThreshold[1]) {
			if (!soundOnTrigger) PlaySound(NOTE_E_PATH, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
			passedThreshold[1] = true;
		}
		else if (!passedThreshold[2] && currentMagnitude >= entryThreshold[2]) {
			if (!soundOnTrigger) PlaySound(NOTE_G_PATH, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
			passedThreshold[2] = true;
		}
	}
	
	SpacePoint.PrevButtonRing[1] = SpacePoint.PrevButtonRing[0];
	SpacePoint.PrevButtonRing[0] = SpacePoint.ButtonRing;

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
					LogMessage((wchar_t*)L"ButtonRing: South Edge: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: South Edge: Down\n");
					SendMouseEvent(MouseEvent::scroll, -600);
				}
			}
			else if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: Outer South: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: Outer South: Down\n");
					SendMouseEvent(MouseEvent::scroll, -240);
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: South: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: South: Down\n");
					SendMouseEvent(MouseEvent::scroll, -120);
				}
			}
		}
		else if (doubleInRange(angle, -135 - c / 2 + a, -135 + c / 2 - a))
		{ // South-west
			if (isEdge)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: South-west Edge: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: South-west Edge: Down\n");
				}
			}
			else if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: Outer South-west: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: Outer South-west: Down\n");
					TapKey(VK_BROWSER_BACK);
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: South-west: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: South-west: Down\n");
					Shortcut_BrowserPrevTab();
				}
			}
		}
		else if (doubleInRange(angle, -90 - b / 2 + a, -90 + b / 2 - a))
		{ // West
			if (isEdge)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: West Edge: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: West Edge: Down\n");
				}
			}
			else if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: Outer West: Up\n");
					SendMouseEvent(MouseEvent::right, 0);
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: Outer West: Down\n");
					SendMouseEvent(MouseEvent::right, 1);
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: West: Up\n");
					SendMouseEvent(MouseEvent::left, 0);
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: West: Down\n");
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
					LogMessage((wchar_t*)L"ButtonRing: North-west Edge: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: North-west Edge: Down\n");
				}
			}
			else if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: Outer North-west: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: Outer North-west: Down\n");
					if (PC_Select == PC::Computer1) TapKey(VK_RETURN); // Save snip
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: North-west: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: North-west: Down\n");
					Shortcut_Snip();
				}
			}
		}
		else if (doubleInRange(angle, 0 - b / 2 + a, 0 + b / 2 - a))
		{ // North
			if (isEdge)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: North Edge: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: North Edge: Down\n");
					SendMouseEvent(MouseEvent::scroll, 600);
				}
			}
			else if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: Outer North: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: Outer North: Down\n");
					SendMouseEvent(MouseEvent::scroll, 240);
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: North: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: North: Down\n");
					SendMouseEvent(MouseEvent::scroll, 120);
				}
			}
		}
		else if (doubleInRange(angle, 45 - c / 2 + a, 45 + c / 2 - a))
		{ // North East
			if (isEdge)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: North-east Edge: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: North-east Edge: Down\n");
				}
			}
			else if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: Outer North-east: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: Outer North-east: Down\n");
					TapKey(VK_BROWSER_FORWARD);
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: North-east: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: North-east: Down\n");
					Shortcut_BrowserNextTab();
				}
			}
		}
		else if (doubleInRange(angle, 90 - b / 2 + a, 90 + b / 2 - a))
		{ // East
			if (isEdge)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: East Edge: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: East Edge: Down\n");
				}
			}
			else if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: Outer East: Up\n");
					Mirror((WCHAR*)L" 1");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: Outer East: Down\n");
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: East: Up\n");
					SendMouseEvent(MouseEvent::middle, 0);
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: East: Down\n");
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
					LogMessage((wchar_t*)L"ButtonRing: South-east Edge: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: South-east Edge: Down\n");
				}
			}
			else if (isOuter)
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: Outer South-east: Up\n");
					Shortcut_BrowserCloseTab();
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: Outer South-east: Down\n");
				}
			}
			else
			{
				if (onExit)
				{
					LogMessage((wchar_t*)L"ButtonRing: South-east: Up\n");
				}
				else if (onEntry)
				{
					LogMessage((wchar_t*)L"ButtonRing: South-east: Down\n");
				}
			}
		}
		else
		{ // Gap between boundaries
			inGap = true;
			onExit = true;
#if LOGFILE_ENABLED
			FILE* fp;
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
			SpacePoint.ButtonEvent = (0.0, 0.0);
			if (!inGap) {
				PlaySound(NOTE_C_PATH, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
				for (int i = 0; i < 3; i++) passedThreshold[i] = false;
			}
		}
		else if (onEntry) {
			if (isEdge && soundOnTrigger) PlaySound(NOTE_G_PATH, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
			else if (isOuter && soundOnTrigger) PlaySound(NOTE_E_PATH, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
			else if (soundOnTrigger) PlaySound(NOTE_D_PATH, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
		}
	}
}

void MoveMouse(bool isImaginary) {
	// Todo: implement
	// if (isImaginary)
}
/// Set SpacePoint functions / exports
#if 1
extern "C" __declspec(dllexport) void SetMouseX(WCHAR * args)
{
	SpacePoint.Mouse.imag(INT_ARGS);
#if LOG_MOUSE_EVENT
	LogMouseEvent(0);
#endif

#if 1
	// Applies absolute zone to the centre of the spacemouse and for the end of any large moves.
	// Could use some kind of polling-based smoothing of the cursor.
	if (std::abs(SpacePoint.Mouse) <= SpacePoint.AbsRadius) {// || 
		//(SpacePoint.AbsZone < std::abs(SpacePoint.Mouse) && std::abs(SpacePoint.Mouse) < std::abs(SpacePoint.PrevMouse) * 0.90)) {
		if (std::abs(SpacePoint.Mouse.imag()) >= std::abs(SpacePoint.PrevMouse.imag()))
			SendMouseEvent(MouseEvent::x, (int)(SpacePoint.Mouse.imag() - SpacePoint.PrevMouse.imag()));
		//	SendMouseEvent(MouseEvent::xy,0);
	}
	else SendMouseEvent(MouseEvent::x, (int)(SpacePoint.Mouse.imag() - SpacePoint.AbsRadius * std::sin(std::arg(SpacePoint.Mouse))));

	SpacePoint.PrevMouse.imag(SpacePoint.Mouse.imag());
#endif
}

extern "C" __declspec(dllexport) void SetMouseY(WCHAR * args)
{
	SpacePoint.Mouse.real(INT_ARGS);
#if LOG_MOUSE_EVENT
	LogMouseEvent(1);
#endif
#if 1
	if (std::abs(SpacePoint.Mouse) <= SpacePoint.AbsRadius) {//|| 
		//(SpacePoint.AbsZone < std::abs(SpacePoint.Mouse) && std::abs(SpacePoint.Mouse) < std::abs(SpacePoint.PrevMouse) * 0.90)) {
		if (std::abs(SpacePoint.Mouse.real()) >= std::abs(SpacePoint.PrevMouse.real()))
			SendMouseEvent(MouseEvent::y, (int)(SpacePoint.Mouse.real() - SpacePoint.PrevMouse.real()));
		//SendMouseEvent(MouseEvent::xy, 0);
	}
	else SendMouseEvent(MouseEvent::y, (int)(SpacePoint.Mouse.real() - SpacePoint.AbsRadius * std::cos(std::arg(SpacePoint.Mouse))));

	SpacePoint.PrevMouse.real(SpacePoint.Mouse.real());
#endif
}

extern "C" __declspec(dllexport) void SetScroll(WCHAR * args)
{
	SpacePoint.Scroll =
		(int)std::round(INT_ARGS * (SpacePoint.MirrorScroll ? -1 : 1) * SpacePoint.Speed);

	SendMouseEvent(MouseEvent::scroll, SpacePoint.Scroll);
}

/// <summary>
/// For setting the speed of mouse / scroll events.
/// </summary>
/// <param name="args"></param>
extern "C" __declspec(dllexport) void SetSpeed(WCHAR * args)
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


extern "C" __declspec(dllexport) void SetButtonRingReal(WCHAR * args)
{
	// Get string that starts with ' ' -> convert to int -> set complex number
	SpacePoint.ButtonRing.real(INT_ARGS);
#if LOG_SET_BUTTONRING
	LogButtonRingEvent(0);
#endif
	SelectButtonOnRing();
}
extern "C" __declspec(dllexport) void SetButtonRingImag(WCHAR * args)
{
	SpacePoint.ButtonRing.imag(INT_ARGS * (SpacePoint.MirrorRing ? -1 : 1));
#if LOG_SET_BUTTONRING
	LogButtonRingEvent(1);
#endif
	SelectButtonOnRing();
}
/// <summary>
/// Set the ButtonRing value using a single function call for the _X, _Y, _Rx or _Ry axes.
/// Mainly for testing / debug purposes.
/// </summary>
/// <param name="args"></param>
extern "C" __declspec(dllexport) void SetButtonRing(WCHAR * args)
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

#endif

