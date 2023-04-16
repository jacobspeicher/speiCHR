#pragma once
#include "color.h"

namespace ColorUtility {
	void ProcessRGB(unsigned char* data, int r, int g, int b);
	Color* ProcessGeneratePalette(unsigned char* data);
	void ProcessToPalette(unsigned char* data, Color palatte[]);

	void ProcessNESColors();
	void ConvertToNESColors(unsigned char* data);
	int GetClosestNESColor(Color data_color);
}