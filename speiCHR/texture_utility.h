#pragma once
#include <glad/glad.h>
#include "color.h"

#include <string>

namespace TextureUtility {
	bool LoadImageDataFromFile(const char* filename, unsigned char** out_image_data, int* out_width, int* out_height);
	bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height, unsigned char** out_image_data);
	void LoadTextureFromData(unsigned char* data, GLuint* out_texture);
}

