/*---------------------------------------------------------------------------------
 * Copyright(c) 2023 Alexandr Ziuzin.
 *
 * This file is part of Contour project.
 *
 * This struct presents group of colors
 *
 ---------------------------------------------------------------------------------*/

#pragma once
#include "Color.h"

namespace ContourExtractorWindowsRuntimeComponent
{
	class ColorGroup
	{
	private:
		std::vector<Color> m_color_values;
		
		unsigned char m_maxColorRange;
		BaseColor m_MaxRangeColor;
		unsigned char m_r_min = 0;
		unsigned char m_r_max = 255;
		unsigned char m_g_min = 0;
		unsigned char m_g_max = 255;
		unsigned char m_b_min = 0;
		unsigned char m_b_max = 255;


		void CalcColorGroupParams();
	public:
		ColorGroup();
		ColorGroup(unsigned char* imgData, int bufferLength);
		ColorGroup(std::vector<Color>* color_values);
		unsigned char getBaseMaxColorRange();
		Color getMidleGroupColor();
		Color getMedianGroupColor();
		Color getAverageGroupColor();

		void Split(ColorGroup* group1, ColorGroup* group2);
		
		void AddColorValues(std::vector<Color> *color_values);
		bool Contain(unsigned char r, unsigned char g, unsigned char b);
		
	};
}
