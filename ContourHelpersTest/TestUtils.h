#pragma once
#include "..\ContourHelpers\Contour.h"
#include "..\ContourHelpers\Level.h"

using namespace  ContourHelpers;

namespace ContourHelpersTest
{
	class TestUtils
	{
	public:
		static bool CompareContours(Contour* contour, Point* points, int expectedPointsNumber, wchar_t *message, int messageLength);
		static bool CompareLevelData(unsigned char levelData[], unsigned char referenceData[], int dataLength, wchar_t *message, int messageLength);



		TestUtils();
		~TestUtils();
	};
} // namespace ContourHelpersTest

