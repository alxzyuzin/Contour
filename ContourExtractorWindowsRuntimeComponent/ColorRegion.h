/*---------------------------------------------------------------------------------
 * Copyright(c) 2023 Alexandr Ziuzin.
 *
 * This file is part of Contour project.
 *
 * This class presents colour region used to reduce number of colors in image
 *
 ---------------------------------------------------------------------------------*/

#pragma once
#include "Color.h"

namespace ContourExtractorWindowsRuntimeComponent
{
	
	/*
	struct CrColor
	{
		unsigned char red;
		unsigned char green;
		unsigned char blue;
		unsigned char alfa;
	};
	*/
	class ColorRegion
	{
	private:
		std::vector<unsigned char> m_r_values;
		std::vector<unsigned char> m_g_values;
		std::vector<unsigned char> m_b_values;

		unsigned char m_maxBaseColorRange;
		BaseColor m_MaxRangeColor;
		unsigned char m_r_min = 0;
		unsigned char m_r_max = 255;
		unsigned char m_g_min = 0;
		unsigned char m_g_max = 255;
		unsigned char m_b_min =0 ;
		unsigned char m_b_max = 255;
		

		void CalcColorRegionParams();

	public:
		ColorRegion();
		ColorRegion(unsigned char* imgData, int bufferLength);
		ColorRegion(std::vector<unsigned char>* r_values, std::vector<unsigned char>* g_values, std::vector<unsigned char>* b_values);
		//ColorRegion(unsigned char rmin, unsigned char rmax, unsigned char gmin, unsigned char gmax, unsigned char bmin, unsigned char bmax);

		void setColorValues(std::vector<unsigned char>* r_values, std::vector<unsigned char>* g_values, std::vector<unsigned char>* b_values);

		unsigned char ColorRegion::getBaseMaxColorRange();
		Color getMidleRegionColor();
		Color getMediandRegionColor();
		Color getAverageRegionColor();
		
		void Split(ColorRegion *reg1, ColorRegion *reg2);
		bool Contain(unsigned char r, unsigned char g, unsigned char b);
		BaseColor SplitColor(std::vector<unsigned char>* range1, std::vector<unsigned char>* range2);
	};
}

