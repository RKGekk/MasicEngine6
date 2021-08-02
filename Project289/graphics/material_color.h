#pragma once

class MaterialColor {
public:
	MaterialColor();
	MaterialColor(unsigned int val);
	MaterialColor(unsigned char r, unsigned char g, unsigned char b);
	MaterialColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	MaterialColor(const MaterialColor& rhv);

	MaterialColor& operator=(const MaterialColor& rhv);
	bool operator==(const MaterialColor& rhv) const;
	bool operator!=(const MaterialColor& rhv) const;

	unsigned char GetR() const;
	void SetR(unsigned char r);

	unsigned char GetG() const;
	void SetG(unsigned char g);

	unsigned char GetB() const;
	void SetB(unsigned char b);

	unsigned char GetA() const;
	void SetA(unsigned char a);

private:
	union {
		unsigned char m_rgba[4];
		unsigned int m_color;
	};
};

namespace MaterialColors {
	const MaterialColor UnloadedTextureColor(100, 100, 100);
	const MaterialColor UnhandeledTextureColor(255, 0, 255);
}