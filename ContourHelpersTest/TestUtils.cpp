#include "TestUtils.h"

using namespace  ContourHelpers;

namespace ContourHelpersTest
{
	
	TestUtils::TestUtils()
	{
	}

	bool TestUtils::CompareContours(Contour* contour, Point* points, int expectedPointsNumber, wchar_t *message, int messageLength)
	{
		if (contour->Size() != expectedPointsNumber)
		{
			swprintf(message, messageLength, L"External Contur length (%i) does not match reference length(%i).",
				contour->Size(), expectedPointsNumber);
			return false;
		}
		for (int i = 0; i < expectedPointsNumber; i++)
		{
			
			Point point = *contour->GetPoint(i);
			if (!(point == points[i]))
			{
				swprintf(message, messageLength, L"Point %i isn't equal to reference. Point coords X=%i,Y=%i. Should be X=%i,Y=%i",
					i, point.X, point.Y,  points[i].X, points[i].Y);
				return false;
			}
		}
		return true;
	}

	bool TestUtils::CompareLevelData(unsigned char levelData[], unsigned char referenceData[], int dataLength, wchar_t *message, int messageLength)
	{
		for (int i = 0; i < dataLength; i++)
			if (levelData[i] != referenceData[i])
			{
				swprintf(message, messageLength, L"Dat at offset %i isn't equal to reference. Data value is %i. Should be %i",
					i, levelData[i], referenceData[i]);
				return false;
			}
		return true;
	}


	TestUtils::~TestUtils()
	{
	}
}