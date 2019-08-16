#pragma once

#include"windows.h"

extern "C" __declspec(dllexport) void init_Dataread();
extern "C" __declspec(dllexport) int ReturnLabel(INT64 time_begin, INT64 time_end);
extern "C" __declspec(dllexport) int Return_Hr();