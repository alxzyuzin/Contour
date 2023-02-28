/*---------------------------------------------------------------------------------
 * Copyright(c) 2023 Alexandr Ziuzin.
 *
 * This file is part of Contour project.
 *
 * This struct presents single color 
 *
 ---------------------------------------------------------------------------------*/

#pragma once

namespace ContourExtractorWindowsRuntimeComponent
{
	public enum class BaseColor { Red, Green, Blue, Alfa };

	struct Color
	{
		unsigned char red = 0;
		unsigned char green = 0;
		unsigned char blue = 0;
		unsigned char alfa = 0xFF;
	};

	struct GrayColorRange
	{
		unsigned char bottom = 0;
		unsigned char top = 0;
		unsigned char value = 127;

		boolean ContainColor(unsigned char r, unsigned char g, unsigned char b)
		{
			unsigned char c = (unsigned char)((r + g + b) / 3);
			if (bottom <= c && c <= top)
				return true;
			return false;
		}
	};

	union PixelBuffer
	{
		unsigned char* charBuffer;
		unsigned int* intBuffer;

	};

}

