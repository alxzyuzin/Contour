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
}

