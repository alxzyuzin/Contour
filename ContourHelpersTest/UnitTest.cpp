#include "pch.h"
#include "CppUnitTest.h"
#include "..\ContourHelpers\Contour.h"
#include "..\ContourHelpers\Level.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace  ContourHelpers;

namespace ContourHelpersTest
{
    TEST_CLASS(LevelTest)
    {
	private:
		/*
		5x5 area of the same color pixels (pixel color 0xFE) 
		*/

		unsigned char OneColor[25] = {  0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
										0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
										0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
										0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
										0xFE, 0xFE, 0xFE, 0xFE, 0xFE };



    public:
		/*
		Look for first shape pixel with color 0xFE
		OK - if pixel found (function FindFirstContourPoint return true)
		*/
        TEST_METHOD(TestMethod1)
        {
			Level* level = new Level(5, 5, 0xFE, OneColor);
			Point firstContourPoint = Point(5, 5);
			Point ExpectedPoint = Point(0, 0);
			bool r = level->FindFirstContourPoint(nullptr, firstContourPoint, 0xFE);
			Assert::IsTrue(r);
        }

		/*
		Check coords first shape pixel with color 0xFE 
		OK - if expected coords == first contour point coords
		*/
		TEST_METHOD(TestMethod2)
		{
			Level* level = new Level(5, 5, 0xFE, OneColor);
			Point firstContourPoint = Point(5, 5);
			Point ExpectedPoint = Point(0, 0);
			level->FindFirstContourPoint(nullptr, firstContourPoint, 0xFE);
			bool r = firstContourPoint.X == ExpectedPoint.X && firstContourPoint.Y == ExpectedPoint.Y;
			Assert::IsTrue(r);
		}

    };
}