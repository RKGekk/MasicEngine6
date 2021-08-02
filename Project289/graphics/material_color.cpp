#include "material_color.h"

MaterialColor::MaterialColor() : m_color(0) {}

MaterialColor::MaterialColor(unsigned int val) : m_color(val) {}

MaterialColor::MaterialColor(unsigned char r, unsigned char g, unsigned char b) {
	m_rgba[0] = r;
	m_rgba[1] = g;
	m_rgba[2] = b;
	m_rgba[3] = 255;
}

MaterialColor::MaterialColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	m_rgba[0] = r;
	m_rgba[1] = g;
	m_rgba[2] = b;
	m_rgba[3] = a;
}

MaterialColor::MaterialColor(const MaterialColor& rhv) {
	m_color = rhv.m_color;
}

MaterialColor& MaterialColor::operator=(const MaterialColor& rhv) {
	if (this == &rhv) { return *this; }
	m_color = rhv.m_color;
	return *this;
}

bool MaterialColor::operator==(const MaterialColor& rhv) const {
	return m_color == rhv.m_color;
}

bool MaterialColor::operator!=(const MaterialColor& rhv) const {
	return m_color != rhv.m_color;
}

unsigned char MaterialColor::GetR() const {
	return m_rgba[0];
}

void MaterialColor::SetR(unsigned char r) {
	m_rgba[0] = r;
}

unsigned char MaterialColor::GetG() const {
	return m_rgba[1];
}

void MaterialColor::SetG(unsigned char g) {
	m_rgba[1] = g;
}

unsigned char MaterialColor::GetB() const {
	return m_rgba[2];
}

void MaterialColor::SetB(unsigned char b) {
	m_rgba[2] = b;
}

unsigned char MaterialColor::GetA() const {
	return m_rgba[3];
}

void MaterialColor::SetA(unsigned char a) {
	m_rgba[3] = a;
}