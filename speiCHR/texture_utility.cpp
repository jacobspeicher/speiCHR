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

	void ProcessRGB(unsigned char* data, int r, int g, int b) {
		int size_of_data = 256 * 240 * 4;
		for (int i = 0; i < size_of_data; i += 4) {
			data[i] *= r;
			data[i + 1] *= g;
			data[i + 2] *= b;
		}
	}

	void ProcessToPalatte(unsigned char* data, Color palatte[]) {
		int size_of_data = 256 * 240 * 4;
		for (int i = 0; i < size_of_data; i += 4) {
			if (data[i + 3] < 255) {
				data[i] = 0;
				data[i + 1] = 0;
				data[i + 2] = 0;
				continue;
			}

			Color new_color = { 0, 0, 0 };
			float average = (data[i] + data[i + 1] + data[i + 2]) / 3;

			if (average >= 0 && average < 64) {
				new_color = palatte[0];
			}
			else if (average >= 64 && average < 128) {
				new_color = palatte[1];
			}
			else if (average >= 128 && average < 192) {
				new_color = palatte[2];
			}
			else if (average >= 191 && average < 256) {
				new_color = palatte[3];
			}

			data[i] = new_color.r;
			data[i + 1] = new_color.g;
			data[i + 2] = new_color.b;
		}
	}

	Color* ProcessGeneratePalatte(unsigned char* data) {
		int size_of_data = 256 * 240 * 4;
		std::map<const Color, int> color_frequency;

		for (int i = 0; i < size_of_data; i += 4) {
			const Color pixel_color(data[i], data[i + 1], data[i + 2]);

			if (color_frequency.count(pixel_color) == 0) {
				color_frequency[pixel_color] = 1;
			}
			else {
				color_frequency[pixel_color]++;
			}
		}

		std::vector<Color> colors;
		std::vector<std::pair<int, int>> sorted_color_frequency;
		for (auto const &kv : color_frequency) {
			colors.push_back(kv.first);
			sorted_color_frequency.push_back(std::make_pair(colors.size() - 1, kv.second));
		}
		std::sort(sorted_color_frequency.begin(), sorted_color_frequency.end(),
			[](const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
				return p1.second < p2.second;
			});
		Color output_colors[4];
		int sorted_size = sorted_color_frequency.size() - 500;
		for (int i = 0; i < 4; ++i) {
			int idx = sorted_size - i;
			spdlog::info(std::to_string(i) + " : "
				+ colors[sorted_color_frequency[idx].first].print() + " : "
				+ std::to_string(sorted_color_frequency[idx].second));

			output_colors[i] = colors[sorted_color_frequency[idx].first];
		}

		return output_colors;
	}
}