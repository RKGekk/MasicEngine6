#pragma once

#include <Windows.h>

#include "string_utility.h"
#include "com_exception.h"

class ErrorLogger {
public:
	static void Log(std::string message);
	static void Log(HRESULT hr, std::string message);
	static void Log(HRESULT hr, std::wstring message);
	static void Log(COMException& exception);
};