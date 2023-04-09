#pragma once
#include <glad/glad.h>

struct Color {
	int r;
	int g;
	int b;
};

namespace TextureUtility {
	bool LoadImageDataFromFile(const char* filename, unsigned char** out_image_data, int* out_width, int* out_height);
	bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height, unsigned char** out_image_data);
	void LoadTextureFromData(unsigned char* data, GLuint* out_texture, int width, int height);
	void ProcessRGB(unsigned char* data, int width, int height, int r, int g, int b);

	void ProcessToPalatte(unsigned char* data, int width, int height, Color palatte[]);
}

