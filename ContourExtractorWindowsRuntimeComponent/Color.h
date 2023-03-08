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
	public enum class TypeOfConvertion { Grayscale = 0, ReducedColors = 1 };
	public enum class ContourColors { Black, White, Red, Green, Blue };
	public enum class ContourType {External, Internal, Both};

	struct Color
	{
		
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		{
			red = r;
			green = g;
			blue = b;
			alfa = a;
		}
		
		Color() {}
		
		unsigned char red = 0;
		unsigned char green = 0;
		unsigned char blue = 0;
		unsigned char alfa = 0xFF;
	};
	
	union CharToIntColor
	{
		CharToIntColor() {}
		
			
		CharToIntColor(unsigned int color)
		{
			intcolor =  color;
		}

		CharToIntColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		{
			charcolor.alfa = a;
			charcolor.red = r;
			charcolor.green = g;
			charcolor.blue = b;
		}
		unsigned int intcolor;
		Color charcolor;
	};
	
	struct GrayColorRange
	{
		unsigned char bottom = 0;
		unsigned char top = 0;
		unsigned char value = 127;

		bool ContainsColor(unsigned char r, unsigned char g, unsigned char b)
		{
			unsigned char c = (unsigned char)((r + g + b) / 3);
			if (bottom <= c && c <= top)
				return true;
			return false;
		}

		bool  ContainsColor(CharToIntColor color)
		{
			unsigned char c = (unsigned char)((color.charcolor.red + color.charcolor.green + color.charcolor.blue) / 3);
			if (bottom <= c && c <= top)
				return true;
			return false;
		}

		unsigned int Value()
		{
			unsigned char v = (unsigned char)((bottom + top) / 2);
			CharToIntColor cc = {v, v, v, 0xFF };
			return cc.intcolor;
		}
	};

	

}

