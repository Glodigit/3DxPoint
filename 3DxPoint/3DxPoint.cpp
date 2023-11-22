﻿// 3DxPoint.cpp : Defines the exported functions for the DLL application. Derivative of 3DxSample from 3DConnexion.
// SPDX-FileCopyrightText: © 2023 Kelvin Afolabi @glodigit
// SPDX-License-Identifier: Boost Software License 1.0

/* ToDo List:
	- 	Implement averaging feature for mouse events
	*/

#include "pch.h"
#include "3DxPoint.h"

SpaceState SpacePoint;


/// Functions generated by BingChat
#if 1
// @brief Find if p is between v1 and v2.
bool doubleInRange(double p, double v1, double v2)
{
	return v1 <= v2 ? v1 <= p && p <= v2 : v2 <= p && p <= v1;
}

/// <summary>
/// Obtains the absolute angle between two complex numbers.
/// </summary>
/// <param name="z2">The first complex number to test.</param>
/// <param name="z1">The second complex number to test.</param>
/// <returns></returns>
double angleBetweenComplex(std::complex<double> z1, std::complex<double> z2)
{
	// calculate the phase angles of z1 and z2
	double a1 = std::arg(z1);
	double a2 = std::arg(z2);

	// calculate the difference between the angles
	double diff = a2 - a1;

	// normalize the difference to the range -pi to pi
	diff = std::atan2(std::sin(diff), std::cos(diff));

	// return the absolute value of the difference
	return std::abs(diff);
}


// returns the average of the last (or first) n elements in a deque of complex<double>
// @param d the deque
// @param n the number of elements to consider. Use -1 to consider all elements
// @param fromEnd a boolean to choose between averaging from the last (default) or the first element
std::complex<double> avgComplexQueue(const std::deque<std::complex<double>>& d, int n, bool fromEnd = true)
{
	// check if the vector is empty or n is zero
	if (d.empty() || n == 0)
	{
		return 0.0; // return zero as the average
	}

	// check if n is larger than the vector size or -1,
	if (n > d.size() || n == -1)
	{
		n = (int)d.size(); // use the whole vector
	}

	// calculate the sum of the last/first n elements using std::accumulate
	std::complex<double> sum = 0;
	if (fromEnd) {
		for (size_t i = d.size() - n; i < d.size(); i++) {
			sum.real(sum.real() + d[i].real());
			sum.imag(sum.imag() + d[i].imag());
		};
	}
	else {
		for (size_t i = 0; i < n; i++) {
			sum.real(sum.real() + d[i].real());
			sum.imag(sum.imag() + d[i].imag());
		}
	}
	//std::accumulate(d.end() - n, d.end(), 0.0) :
	//std::accumulate(d.begin(), d.begin() + n, 0.0);

// return the average by dividing the sum by n
	return sum / std::complex<double>(n);

}


// Returns the number of 0 + 0i elements at the end of a deque of complex numbers
int zeroEnd(const std::deque<std::complex<double>>& d) {
	// Initialize the count to zero
	int count = 0;

	// Use an iterator to traverse the deque from the end to the beginning
	auto it = d.rbegin(); // get a reverse iterator to the last element of the deque
	while (it != d.rend()) // loop until the beginning of the deque
	{
		// Check if the element is zero
		if (it->real() == 0 && it->imag() == 0)
		{
			// Increment the count
			count++;
		}
		else
		{
			// Break the loop when a non-zero element is encountered
			break;
		}
		// Move the iterator to the previous element
		++it;
	}

	// Return the final count
	return count;
}


// A function that scales a complex number to a given magnitude
std::complex<double> scaleComplex(const std::complex<double>& z, double n)
{
	// Find the current magnitude of the complex number
	double m = std::abs(z);

	// Divide the complex number by its current magnitude to get a unit complex number
	std::complex<double> u = z / m;

	// Multiply the unit complex number by the desired magnitude to get the scaled complex number
	std::complex<double> s = n * u;

	// Return the scaled complex number as the result
	return s;
}

#endif

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
				 (int)std::round(std::abs(SpacePoint.Mouse[1])),
				 (int)std::round(toDegrees * std::arg(SpacePoint.Mouse[1])),
				 (int)SpacePoint.Mouse[1].real(),
				 (int)SpacePoint.Mouse[1].imag());
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
static void SendMouseEvent(MouseEvent eventType, int eventValue = 0)
{
	UINT status;
	INPUT input;
	const UINT nInputs = 1;

	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;

#if 0
	static bool reflectOn = false;
	if (eventType == MouseEvent::x || eventType == MouseEvent::y || eventType == MouseEvent::scroll) {

		if (std::abs(eventValue) < 20) {
			reflectOn = false;
			return;
		}
		else if (eventType != MouseEvent::scroll && std::abs(SpacePoint.Mouse) < std::abs(SpacePoint.PrevMouse) - 18) { reflectOn = true; }
		if (reflectOn) eventValue *= 0.25;
}
#endif

	double fractionalPart, integerPart;

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
		//input.mi.dx = (long)std::round(eventValue * SpacePoint.Speed);
		input.mi.dx = eventValue;
		break;
	case MouseEvent::y:
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		//input.mi.dy = (long)std::round(eventValue * SpacePoint.Speed);
		input.mi.dy = eventValue;
		break;
	case MouseEvent::xy:
		// eventValue not used

		// Make sure that there's at least some applicable change
		if ((int)SpacePoint.MouseResult.real() == 0 &&
			(int)SpacePoint.MouseResult.imag() == 0) return;

		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		//input.mi.dx = (long)std::round((SpacePoint.Mouse.imag() + SpacePoint.PrevMouse.imag()) / 2 * SpacePoint.Speed);
		//input.mi.dy = (long)std::round((SpacePoint.Mouse.real() + SpacePoint.PrevMouse.real()) / 2 * SpacePoint.Speed);
		fractionalPart = std::modf(SpacePoint.MouseResult.imag(), &integerPart);
		input.mi.dx = (long)integerPart; SpacePoint.MouseResult.imag(fractionalPart);
		fractionalPart = std::modf(SpacePoint.MouseResult.real(), &integerPart);
		input.mi.dy = (long)integerPart; SpacePoint.MouseResult.real(fractionalPart);
		if (input.mi.dx == 0 && input.mi.dy == 0) return;
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
	if (pc == PC::Minimal || pc == PC::Typical || pc == PC::Computer2) {
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

/// <summary>
/// Corrects for the non-circularity of the x or y inputs.
/// </summary>
/// <param name="n"> - The complex number to obtain the multiplier for.</param>
/// <returns></returns>
double MagnitueMultiplier(std::complex<double> n) {
	// Number from 0 to 1. Diagonals produce magnitudes greater than 350, so this is set as the 
	// minimum value to multiply by to normalise the magnitudes
	const double DiagonalMultiplier = 348.0 / 424.0;
	const double x = 2 * (1 - DiagonalMultiplier) / (1 + DiagonalMultiplier);
	return (2 + x * std::cos(4 * std::arg(n))) / (2 + x);
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
		std::abs(SpacePoint.ButtonRing) * MagnitueMultiplier(SpacePoint.ButtonRing);
	double prevMagnitude = max(
		std::abs(SpacePoint.PrevButtonRing[0]) * MagnitueMultiplier(SpacePoint.PrevButtonRing[0]),
		std::abs(SpacePoint.PrevButtonRing[1]) * MagnitueMultiplier(SpacePoint.PrevButtonRing[1])
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
		SpacePoint.ButtonEvent *= MagnitueMultiplier(SpacePoint.ButtonEvent);
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
		const double a = anglePadding, b = cardinalAngle, c = 90 - b;

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

void UpdateMouse(void) {
	std::complex<double> averagedBeginning, averagedEnding, scaledMouse;

	// scale out non-circularity
	SpacePoint.Mouse[0] *= MagnitueMultiplier(SpacePoint.Mouse[0]);

	// get averages
	SpacePoint.MouseAverage = avgComplexQueue(SpacePoint.Mouse, -1);
	averagedBeginning = avgComplexQueue(SpacePoint.Mouse, SpacePoint.MouseSmoothingSize, false);
	averagedEnding = avgComplexQueue(SpacePoint.Mouse, SpacePoint.MouseSmoothingSize);


	// check if magnitude drop or abrupt angle change and, if so, reset the last n values
	if (std::abs(averagedBeginning) < std::abs(averagedEnding) ||
		angleBetweenComplex(averagedBeginning, averagedEnding) > 22.5)
		std::fill(SpacePoint.Mouse.end() - SpacePoint.MouseInvalidSize, SpacePoint.Mouse.end(), 0);

	// Add % amount of MouseAverage if MouseResult was incomplete
	UINT z = zeroEnd(SpacePoint.Mouse);
	if (z > 0) {
		if (z >= SpacePoint.MouseSmoothingSize) averagedEnding = SpacePoint.MouseAverage;
		else {
			double x = (double)z / SpacePoint.MouseSmoothingSize;
			averagedEnding = averagedEnding * (1 - x) + SpacePoint.MouseAverage * x;
		}
	}

	// Scale MouseResult based on line->cos->line
	const double slowLine[2] = { 0.3334, 15 }; // send value[0] between 0 and value[1]
	const double fastLine[2] = { 120, 120 };

	double MouseResultMagnitude = std::abs(averagedEnding);
	if (MouseResultMagnitude <= slowLine[1]) scaledMouse = scaleComplex(averagedEnding, slowLine[0]);
	else if (MouseResultMagnitude <= fastLine[1]) {
		scaledMouse = scaleComplex(averagedEnding,
			0.5 *
			(
				(slowLine[0] - fastLine[0]) *
				std::cos(pi * (MouseResultMagnitude - slowLine[2]) / (fastLine[1] - slowLine[1])) +
				slowLine[0] + fastLine[0]
				)
		);
	}
	else scaledMouse = scaleComplex(averagedEnding, fastLine[0]);

	scaledMouse *= SpacePoint.Speed;

	SpacePoint.MouseResult += scaledMouse;
	SendMouseEvent(MouseEvent::xy);

#if 0
	SpacePoint.MouseResult += scaledMouse;
	if (SpacePoint.MouseResult.real() >= 1 && SpacePoint.MouseResult.imag() >= 1) {
		// mouse moved enough to change both values at the same time
		SendMouseEvent(MouseEvent::xy, 0);
	}
	else if (scaledMouse.real() >= 1) {
		// store the imaginary component and send the re
		SpacePoint.MouseResult.imag(scaledMouse.imag());
		SendMouseEvent(MouseEvent::y, scaledMouse.real());
	}
	//SpacePoint.MouseResult.real(SpacePoint.MouseResult.real() - (int)SpacePoint.MouseResult.real());
	//SpacePoint.MouseResult.imag(SpacePoint.MouseResult.imag() - (int)SpacePoint.MouseResult.imag());
	SpacePoint.MouseResult.real(std::modf())
#endif

}

/// <summary>
/// Adds an integer value to the mouse queue
/// </summary>
/// <param name="isImaginary"> - 1 if setting the imaginary component, 0 if setting the real component.</param>
/// <param name="bufferValue"> - Value to add to the mouse queue.</param>
void AddToMouse(bool isImaginary, int value) {
	// Todo: implement

	/*
	The plan (for mouse):
		- Ensure there's at least 1 element in the buffers
			-> It might be the case that the amount of vectors is about equal to the buffersize at all times
		- always add a new element if the current element is 0 + 0i
		- If the real or imaginary component != 0, make a new element and add value there
			-> Apply MagnitudeMultiplier (potentially others) and then create new element
		- increase the size until at BufferSize set in SpacePoint
		- 2 averages
			-> Full average is all but the last element (as that one may still be worked on)
			-> Smoothing average is what's actually sent to the mouse
				- Take 4 samples instead of 3 if oldest value hasn't been removed yet (to prevent duplicate values being sent)
			-> Thus, the buffersize may need to be larger than 5
		- Some way of moving the mouse pixel by pixel without having to rely on the speed setting
			-> potentially multiply values such that their magnitude is at least 10, then divide by 20
			-> potential equation: magnitude = 0.5 for values under 10 and an exponent curve from 10 to 350
			-> potentially done in setmouse and not here
		- Some way of responding to abrupt changes in either the magnitude or angle
			-> then, you want those look-ahead values to be the values actually sent out (via the smoothing average)
			-> issue is that the buffer needs time to recover too
			-> abrupt magnitude referring to stopping, so probably would be fine with resetting the buffer
			-> abrupt angle referring to a change in direction, so maybe something like junction deviation would be needed
			-> potential idea is to 0 the values responsible for smoothingAverage and if smoothingAverage = 0, use fullAverage
				- issue is how to hand off fullAverage to smoothingAverage
				- maybe something like [percentage of smoothingAverage values that are 0], so if all values are present, use
					all smoothingAverage no fullAvg, no values are present use no smoothAvg all fullAvg
				- something like
					if (beginningZeroes(buffer) - smoothingAvg < 0) diff = 0.0
					diff = diff / smoothingAvg
					finalAvg = diff * smoothingAvg + (1 - diff) * fullAvg
			-> angle needs to work across the discontinuity (-179.9 to +180 is actually only 0.1 degree away)
				-> done. use angleBetweenComplex()
	*/

	// check if the vector is too small
	if (SpacePoint.Mouse.empty() || SpacePoint.Mouse.size() < SpacePoint.MouseSize)
	{
		SpacePoint.Mouse.resize(SpacePoint.MouseSize);
	}

	if (SpacePoint.Mouse[0] == 0.0) { // 0 + 0 i
		// Use the first element if the second element is also 0, else put in a new element
		if (SpacePoint.Mouse[1] == 0.0) isImaginary ? SpacePoint.Mouse[0].imag(value) : SpacePoint.Mouse[0].real(value);
		else SpacePoint.Mouse.push_front(isImaginary ? (0, value) : (value));
	}
	else if (SpacePoint.Mouse[0].imag() == 0.0) // n + 0 i
		if (isImaginary) SpacePoint.Mouse[0].imag(value);
		else { // finished working on element, so scale it and then add the next working element to the front
			UpdateMouse();
			SpacePoint.Mouse.push_front((value));
			SpacePoint.Mouse.pop_back();
		}
	else if (SpacePoint.Mouse[0].real() == 0.0) // 0 + m i
		if (!isImaginary) SpacePoint.Mouse[0].real(value);
		else {
			UpdateMouse();
			SpacePoint.Mouse.push_front((0, value));
			SpacePoint.Mouse.pop_back();
		}
	else { // the number must be n + m i, this element is complete
		UpdateMouse();
		SpacePoint.Mouse.push_front(isImaginary ? (0, value) : (value));
		SpacePoint.Mouse.pop_back();
	}

}

/// Set SpacePoint functions / exports
#if 1
extern "C" __declspec(dllexport) void SetMouseX(WCHAR * args)
{
	AddToMouse(true, INT_ARGS);
#if LOG_MOUSE_EVENT
	LogMouseEvent(0);
#endif

#if 0
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
	AddToMouse(false, INT_ARGS);
#if LOG_MOUSE_EVENT
	LogMouseEvent(1);
#endif
#if 0
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

