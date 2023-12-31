#pragma once
#include "pch.h"


#define LOGFILE_ENABLED			false
#define LOG_PROCESSINFO			true
#define LOG_SET_BUTTONRING		false
#define LOG_MOUSE_EVENT			false
#define LOG_MOUSE_DIAGNOSTICS	true

// Todo: add more settings, such as thresholds 

/// <summary>
/// The PC to create the DLL for.
/// Usually used to compile different virtual buttons for different PCs.
/// </summary>
constexpr PC PC_Select = PC::Computer1;