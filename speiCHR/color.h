#pragma once

#include <string>

class Color {
public:
	Color() : r(0), g(0), b(0) {}
	Color(int r, int g, int b) : r(r), g(g), b(b) {}
	Color(const Color& other) : r(other.r), g(other.g), b(other.b) {}

	int get_r() { return r; }
	int get_g() { return g; }
	int get_b() { return b; }

	std::string print() const {
		return "(" + std::to_string(r) + ", "
			+ std::to_string(g) + ", "
			+ std::to_string(b) + ")";
	}

	bool operator==(const Color& other) const {
		return r == other.r &&
			g == other.g &&
			b == other.b;
	}

	bool operator<(const Color& other) const {
		int avg = (r + g + b) / 3;
		int other_avg = (other.r + other.g + other.b) / 3;
		return avg < other_avg;
	}

	bool operator>(const Color& other) const {
		int avg = (r + g + b) / 3;
		int other_avg = (other.r + other.g + other.b) / 3;
		return avg > other_avg;
	}

private:
	int r, g, b;
};