#pragma once

#include <string>
#include <locale>
#include <codecvt>
#include <iomanip>
#include <sstream>

#include <DirectXMath.h>

std::string w2s(const std::wstring& var);
std::wstring s2w(const std::string& var);

std::string fixedfloat(float value, int precision);
std::ostream& operator<<(std::ostream& os, const DirectX::XMFLOAT2& v);
std::ostream& operator<<(std::ostream& os, const DirectX::XMFLOAT3& v);
std::ostream& operator<<(std::ostream& os, DirectX::FXMVECTOR v);
std::ostream& operator<<(std::ostream& os, DirectX::XMFLOAT4X4 m);
std::ostream& operator<<(std::ostream& os, DirectX::CXMMATRIX m);