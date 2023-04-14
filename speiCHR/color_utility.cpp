#include <spdlog/spdlog.h>

#include "color_utility.h"

#include <map>
#include <fstream>

namespace ColorUtility {
	static std::vector<Color> nes_colors;

	void ProcessRGB(unsigned char* data, int r, int g, int b) {
		int size_of_data = 256 * 240 * 4;
		for (int i = 0; i < size_of_data; i += 4) {
			data[i] *= r;
			data[i + 1] *= g;
			data[i + 2] *= b;
		}
	}

	Color* ProcessGeneratePalette(unsigned char* data) {
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
		for (auto const& kv : color_frequency) {
			colors.push_back(kv.first);
			sorted_color_frequency.push_back(std::make_pair(colors.size() - 1, kv.second));
		}
		std::sort(sorted_color_frequency.begin(), sorted_color_frequency.end(),
			[](const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
				return p1.second < p2.second;
			});
		Color output_colors[4];
		size_t sorted_size = sorted_color_frequency.size() - 500;
		for (int i = 0; i < 4; ++i) {
			size_t idx = sorted_size - i;
			spdlog::info(std::to_string(i) + " : "
				+ colors[sorted_color_frequency[idx].first].print() + " : "
				+ std::to_string(sorted_color_frequency[idx].second));

			output_colors[i] = colors[sorted_color_frequency[idx].first];
		}

		return output_colors;
	}

	void ProcessToPalette(unsigned char* data, Color palatte[]) {
		int size_of_data = 256 * 240 * 4;
		for (int i = 0; i < size_of_data; i += 4) {
			if (data[i + 3] < 255) {
				data[i] = 0;
				data[i + 1] = 0;
				data[i + 2] = 0;
				continue;
			}

			Color new_color = { 0, 0, 0 };
			int average = (data[i] + data[i + 1] + data[i + 2]) / 3;

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

			data[i] = new_color.get_r();
			data[i + 1] = new_color.get_g();
			data[i + 2] = new_color.get_b();
		}
	}

	void ProcessNESColors() {
		std::ifstream color_file("nes_colors.csv");
		std::string line;

		while (std::getline(color_file, line, '\n')) {

			std::stringstream ss;
			int r;
			ss << std::hex << line.substr(0, 4);
			spdlog::warn(ss.str());
			ss >> r;
			std::stringstream().swap(ss);
			int g;
			ss << std::hex << line.substr(5, 4);
			spdlog::warn(ss.str());
			ss >> g;
			std::stringstream().swap(ss);
			int b;
			ss << std::hex << line.substr(10, 4);
			spdlog::warn(ss.str());
			ss >> b;
			std::stringstream().swap(ss);

			Color color(r, g, b);
			nes_colors.push_back(color);
		}

		for (auto color : nes_colors) {
			spdlog::info(color.print());
		}
	}
}