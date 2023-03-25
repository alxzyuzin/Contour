#pragma once
#include "..\ContourExtractorWindowsRuntimeComponent\Contour.h"
#include "..\ContourExtractorWindowsRuntimeComponent\Level.h"

using namespace  ContourExtractorWindowsRuntimeComponent;

namespace ContourTestVS2019
{
	class TestUtils
	{
	public:
		static bool CompareContours(Contour* contour, Point* points, int expectedPointsNumber, wchar_t *message, int messageLength);
		static bool CompareLevelData(unsigned char levelData[], unsigned char referenceData[], int dataLength, wchar_t *message, int messageLength);

	public:
		static bool CompareArays(unsigned int* a, unsigned int* b, int len)
		{
			for (int i = 0; i < len; i++)
				if (a[i] != b[i])
					return false;
			return true;
		}

		TestUtils();
		~TestUtils();
	};
} // namespace ContourHelpersTest

