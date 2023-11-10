#pragma once
#include "pch.h"


#define LOGFILE_ENABLED		true
#define LOG_PROCESSINFO		true
#define LOG_SET_BUTTONRING	false
#define LOG_MOUSE_EVENT		false

/// <summary>
/// The PC to create the DLL for.
/// Usually used to compile different virtual buttons for different PCs.
/// </summary>
constexpr PC PC_Select = PC::Typical;