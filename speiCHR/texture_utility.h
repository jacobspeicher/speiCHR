#pragma once
#include <glad/glad.h>

#include <string>

struct Color {
	int r;
	int g;
	int b;

	Color() : r(0), g(0), b(0) {}
	Color(int r, int g, int b) : r(r), g(g), b(b) {}
	Color(const Color& other) : r(other.r), g(other.g), b(other.b) {}

	std::string print() const {
		return std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b);
	}

	bool operator==(const Color& other) const {
		return r == other.r &&
			g == other.g &&
			b == other.b;
	}

	bool operator<(const Color& other) const {
		return (r + g + b) < (other.r + other.g + other.b);
	}

	bool operator>(const Color& other) const {
		return (r + g + b) > (other.r + other.g + other.b);
	}
};

namespace TextureUtility {
	bool LoadImageDataFromFile(const char* filename, unsigned char** out_image_data, int* out_width, int* out_height);
	bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height, unsigned char** out_image_data);
	void LoadTextureFromData(unsigned char* data, GLuint* out_texture);
	void ProcessRGB(unsigned char* data, int r, int g, int b);

	void ProcessToPalatte(unsigned char* data, Color palatte[]);
	Color* ProcessGeneratePalatte(unsigned char* data);
}

