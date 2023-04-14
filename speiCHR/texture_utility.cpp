#include "texture_utility.h"
#include "include/stb_image.h"

#include <map>
#include <spdlog/spdlog.h>

namespace TextureUtility {
	bool LoadImageDataFromFile(const char* filename, unsigned char** out_image_data, int* out_width, int* out_height) {
		int width, height, numChannels;
		unsigned char* data = stbi_load(filename, &width, &height, &numChannels, 4);
		if (data == NULL) {
			return false;
		}

		*out_width = width;
		*out_height = height;
		*out_image_data = data;
		
		return true;
	}

	bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height, unsigned char** out_image_data) {
		int width, height, numChannels;
		unsigned char* data = stbi_load(filename, &width, &height, &numChannels, 4);
		// data[0] = 1R, data[1] = 1G, data[2] = 1B, data[3] = 1A
		if (data == NULL) {
			return false;
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		*out_texture = texture;
		*out_width = width;
		*out_height = height;
		*out_image_data = data;

		return true;
	}

	void LoadTextureFromData(unsigned char* data, GLuint* out_texture) {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 240, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		*out_texture = texture;
	}
}