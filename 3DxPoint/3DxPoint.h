// SPDX-FileCopyrightText: © 2023 Kelvin Afolabi @glodigit
// SPDX-License-Identifier: Boost Software License 1.0
#pragma once
#include "pch.h"


#define LOG_PATH	L"C:\\ProgramData\\3Dconnexion\\3DxPoint\\3DxPoint.txt"
#define NOTE_C_PATH L"C:\\ProgramData\\3Dconnexion\\3DxPoint\\Pizzicato C.wav"
#define NOTE_D_PATH L"C:\\ProgramData\\3Dconnexion\\3DxPoint\\Pizzicato D.wav"
#define NOTE_E_PATH L"C:\\ProgramData\\3Dconnexion\\3DxPoint\\Pizzicato E.wav"
#define NOTE_G_PATH L"C:\\ProgramData\\3Dconnexion\\3DxPoint\\Pizzicato G.wav"

#define OPEN_LOGFILE_SUCCESSFUL (_wfopen_s(&fp, LOG_PATH, L"a,ccs=UTF-8") == 0 && fp != NULL)
#define INT_ARGS _wtoi(wcsrchr(args, L' '))
//		read like (int)*args

// Angle conversions
const double pi = std::acos(-1);
const double toRadians = pi / 180.0;
const double toDegrees = 180.0 / pi;

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


typedef enum class PC
{
	Minimal,
	Typical,
	Computer1,
	Computer2,
	Computer3
};

typedef struct SpaceState
{
	// MouseBuffer, ButtonRingBuffer in the form y + x i
#if 0
	UINT BufferSize[2] = { 8, 5 };
	std::vector<std::complex<double>> Buffers[2] =
	{
		std::vector<std::complex<double>>(BufferSize[0]),
		std::vector<std::complex<double>>(BufferSize[1])
	};
#endif
	//std::complex<double> Mouse; // In the form y + x i
	//std::vector<std::complex<double>> MouseBuffer = {0}, ButtonRingBuffer = {0};
	//UINT	MouseBufferSize = 8, 
		//	MouseSmoothingSize = 3,
			//ButtonRingBufferSize = 5;
	std::chrono::steady_clock::time_point LastMouseEvent = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::ratio<1, 60>> PollingRate;
	//std::chrono::steady_clock::time_point LastButtonRingEvent = std::chrono::high_resolution_clock::now();
	//std::complex<double> PrevMouse;
	//double AbsRadius = 20; // Absolute movement radius for mouse
	std::deque<std::complex<double>> Mouse;
	UINT
		MouseSize = 5,			// size of Mouse
		MouseSmoothingSize = 3, // values to consider for MouseResult
		MouseChangeSize = 3;	// values to reset if abrupt change detected
	std::complex<double> MouseResult;// MouseAverage;
	int Scroll{};
	double Speed = 1;
	std::complex<double> ButtonRing;
	std::complex<double> PrevButtonRing[2] = {};
	std::complex<double> ButtonEvent;
	bool MirrorScroll{}, MirrorRing{};
} SpaceState;

#include "settings.h"

#if LOGFILE_ENABLED
#include <string>
#endif