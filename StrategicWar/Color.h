#ifndef COLOR_H
#define COLOR_H

class Color {
public:
	unsigned char r, g, b, a;
	Color() : r(0), g(0), b(0), a(1) {};
	Color(char _r, char _g, char _b) : r(_r), g(_g), b(_b), a(255) {};
	Color(float _r, float _g, float _b, float _a) : r(_r * 255), g(_g * 255), b(_b * 255), a(_a * 255) {};

	bool operator==(const Color &o) {
		return r == o.r && g == o.g && b == o.b;
	};

	~Color() { };
};

#endif