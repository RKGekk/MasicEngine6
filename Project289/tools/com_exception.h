#pragma once

#include <comdef.h>

#include "string_utility.h"

#define COM_ERROR_IF_FAILED( hr, msg ) if( FAILED( hr ) ) throw COMException( hr, msg, __FILE__, __FUNCTION__, __LINE__ )

class COMException {
public:
	COMException(HRESULT hr, const std::string& msg, const std::string& file, const std::string& function, int line) {
		_com_error error(hr);
		m_whatmsg = L"Msg: " + s2w(msg) + L"\n";
		m_whatmsg += error.ErrorMessage();
		m_whatmsg += L"\nFile: " + s2w(file);
		m_whatmsg += L"\nFunction: " + s2w(function);
		m_whatmsg += L"\nLine: " + std::to_wstring(line);
	}

	const wchar_t* what() const { return m_whatmsg.c_str();	}

private:
	std::wstring m_whatmsg;
};