#pragma once

#include <string>

enum class IMPCHANNEL {
	RED,
	GREEN,
	BLUE,
	EQUAL,
};

class Color {
public:
	Color() : r(255), g(255), b(255), primary(IMPCHANNEL::EQUAL), secondary(IMPCHANNEL::EQUAL) {}
	Color(const Color& other) : r(other.r), g(other.g), b(other.b), 
		primary(other.primary), secondary(other.secondary) {}
	Color(int r, int g, int b) : r(r), g(g), b(b) {
		if (r > g && r > b) { primary = IMPCHANNEL::RED; }
		else if (g > r && g > b) { primary = IMPCHANNEL::GREEN; }
		else if (b > r && b > g) { primary = IMPCHANNEL::BLUE; }
		else { primary = IMPCHANNEL::EQUAL; }

		if ((primary == IMPCHANNEL::GREEN && r > b) || (primary == IMPCHANNEL::BLUE && r > g)) { secondary = IMPCHANNEL::RED; }
		else if ((primary == IMPCHANNEL::RED && g > b) || (primary == IMPCHANNEL::BLUE && b > r)) { secondary = IMPCHANNEL::GREEN; }
		else if ((primary == IMPCHANNEL::RED && b > g) || (primary == IMPCHANNEL::GREEN && b > r)) { secondary = IMPCHANNEL::BLUE; }
		else { secondary = IMPCHANNEL::EQUAL; }
	}

	int GetR() { return r; }
	int GetG() { return g; }
	int GetB() { return b; }

	float Distance(const Color& other) {
		float avg_r = (r + other.r) / 2;
		float delta_r = (r - other.r);
		float delta_g = (g - other.g);
		float delta_b = (b - other.b);

		float dist_r = (2 + (avg_r / 256)) * pow(delta_r, 2);
		float dist_g = 4 * pow(delta_g, 2);
		float dist_b = (2 + ((255 - avg_r) / 256)) * pow(delta_b, 2);

		return sqrt(dist_r + dist_g + dist_b);
	}

	std::string Print() const {
		return "(" + std::to_string(r) + ", "
			+ std::to_string(g) + ", "
			+ std::to_string(b) + ")";
	}

	IMPCHANNEL GetPrimary() {
		return primary;
	}

	IMPCHANNEL GetSecondary() {
		return secondary;
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
	IMPCHANNEL primary;
	IMPCHANNEL secondary;
};