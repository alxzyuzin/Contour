#include "pch.h"
#include "CppUnitTest.h"
#include "..\ContourHelpers\Contour.h"
#include "..\ContourHelpers\Level.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace  ContourHelpers;

namespace ContourHelpersTest
{
	const unsigned char E = 0xFF;
	const unsigned char B = 0x00;
	const unsigned char W = 0xFE;

	TEST_CLASS(LevelTest_ServiceFunctions)
	{
		unsigned char EmptySquare[25] =
		{  
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E 
		};
		
		unsigned char ReferenceEmptySquareExpanded[100] =
		{   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF
		};

		

		TEST_METHOD(ExpandLevelData_Test_1)
		{
			unsigned char EmptySquareExpanded[100];

			Level::ExpandLevelData(5, 5, E, EmptySquare, EmptySquareExpanded);
			int i = 0;
			for (unsigned char c : ReferenceEmptySquareExpanded)
			{
				Assert::IsTrue(c == EmptySquareExpanded[i++], L"Points is equal.");
			}
		}

			unsigned char FibbonachiSquare[25] =
		{
			B, E, B, E, B,
			B, E, E, B, B,
			B, E, E, E, B,
			B, B, B, B, E,
			E, E, E, E, B
		};

		unsigned char ReferenceFibbonachiSquareExpanded[100] =
		{   0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF,
			0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
			0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF,
			0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF
		};

		TEST_METHOD(ExpandLevelData_Test_2)
		{
			wchar_t Message[100];
			unsigned char FibbonachiSquareExpanded[100];

			Level::ExpandLevelData(5, 5, B, FibbonachiSquare, FibbonachiSquareExpanded);
			
			for (int i = 0; i<100; i++)
			{
				unsigned char c = ReferenceFibbonachiSquareExpanded[i];
				unsigned char f = FibbonachiSquareExpanded[i];
				swprintf(Message, 100, L"Point %i isn't equal to reference. Point color %X should be %X", i,f,c);
				Assert::IsTrue(c == f, Message);
				++i;
			}
		}
		
	}; // LevelTest_ServiceFunctions

	TEST_CLASS(LevelTest_FunctionsParams)
	{
		TEST_METHOD(Constructor_Test_1)
		{
			unsigned char pixelBuffer[100];
			Level* level = new Level(5, 5, W, pixelBuffer);
			Assert::IsNotNull(level);
		}

		TEST_METHOD(Constructor_Test_2)
		{
			Assert::ExpectException< std::invalid_argument, Level*>([]()
				{
					unsigned char pixelBuffer[100];
					return new Level(0, 5, W, pixelBuffer);
				}, L"Parametr width == 0 and exception not thrown.");
		}

		TEST_METHOD(Constructor_Test_3)
		{
			Assert::ExpectException< std::invalid_argument, Level*>([]()
				{
					unsigned char pixelBuffer[100];
					return new Level(5, 0, W, pixelBuffer);
				}, L"Parametr height == 0 and exception not thrown.");
		}

		TEST_METHOD(Constructor_Test_4)
		{
			Assert::ExpectException< std::invalid_argument, Level*>([]()
				{
					unsigned char pixelBuffer[100];
					return new Level(5, 5, E, pixelBuffer);
				}, L"Parametr color == 0xFF and exception not thrown.");
		}

		TEST_METHOD(Constructor_Test_5)
		{
			Assert::ExpectException< std::invalid_argument, Level*>([]()
				{
					return new Level(5, 5, E, nullptr);
				}, L"Parametr pixelBuffer is null and exception not thrown.");
		}


	};

    TEST_CLASS(LevelTest_ExternalContour)
    {
	private:

		
		/*
		5x5 area of the same color pixels (pixel color 0xFE) 
		Each pixel presented by 4 bytes
		*/

		// Empty square size 5x5 with black point in center
		unsigned char EmptySquareWithBlackPoint[100] =
		{   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF, 0xFF
		};

		// White 5x5 square
		unsigned char WhiteSquare[25] =
			{	W, W, W, W, W,
				W, W, W, W, W,
				W, W, W, W, W,
				W, W, W, W, W,
				W, W, W, W, W
			};

    public:


		/*
		Analize 5x5 square area filled by white color (0xFE) 
		Look for first shape pixel with color 0xFE
		OK - if pixel found (function FindFirstContourPoint return true)
		*/
        TEST_METHOD(FindFirstExternaLContourPoint_Test_1)
        {
			unsigned char WhiteSquareExpanded[100];
			Level::ExpandLevelData(5, 5, W, WhiteSquare, WhiteSquareExpanded);
			
			Level* level = new Level(5, 5, W, WhiteSquareExpanded);
			Point firstContourPoint = Point(5, 5);
			Point ExpectedPoint = Point(0, 0);
			bool r = level->FindFirstContourPoint(nullptr, firstContourPoint, W);
			Assert::IsTrue(r);
        }

		/*
		Check coords first shape pixel with color 0xFE 
		OK - if expected coords == first contour point coords
		*/
		TEST_METHOD(FindFirstExternaLContourPoint_Test_2)
		{
			unsigned char WhiteSquareExpanded[100];
			Level::ExpandLevelData(5, 5, W, WhiteSquare, WhiteSquareExpanded);

			Level* level = new Level(5, 5, 0xFE, WhiteSquareExpanded);
			Point firstContourPoint = Point(5, 5);
			Point ExpectedPoint = Point(0, 0);
			level->FindFirstContourPoint(nullptr, firstContourPoint, 0xFE);
			Assert::IsTrue(firstContourPoint == ExpectedPoint);
		}

		/*
		Find next pixel if current pixel lays in upper right corner of OneColor area 0xFE
		OK - if expected coords == X = 4 Y = 1
		*/
		TEST_METHOD(FindNextExternalContourPoint_Test_1)
		{
			unsigned char WhiteSquareExpanded[100];
			Level::ExpandLevelData(5, 5, W, WhiteSquare, WhiteSquareExpanded);
			
			Level* level = new Level(5, 5, 0xFE, WhiteSquareExpanded);
			Point currentPoint = Point(4, 0);
			Point nextExpectedPoint = Point(4, 1);
			level->FindNextContourPoint(nullptr, currentPoint, Direction::S, 0xFE);
			Assert::IsTrue(currentPoint == nextExpectedPoint);
		}

		// Empty square size 5x5
		unsigned char EmptySquare[25] =
		{
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E
		};

		/*
		Find contour in 5x5 empty square area ( filled with color 0xFF)
		Expected result: contour not found
		*/
		TEST_METHOD(FindExternalContour_Test_0)
		{
			unsigned char EmptySquareExpanded[100];
			Level::ExpandLevelData(5, 5, B, EmptySquare, EmptySquareExpanded);
			
			Level* level = new Level(5, 5, B, EmptySquareExpanded);
			Contour* contour = level->FindContour(nullptr, B);
			Assert::IsNull(contour, L"contour is not null");
		}


		/*
		Find contour of square area filled with white color (0xFE)
		Check contour length
		*/
		TEST_METHOD(FindExternalContour_Test_1)
		{
			wchar_t Message[100];
			unsigned char WhiteSquareExpanded[100];
			Level::ExpandLevelData(5, 5, W, WhiteSquare, WhiteSquareExpanded);
			
			// use this expression to find array size
			//int size = *(&arr + 1) - arr;
			int expectedContourLength = 16;
			Point ExpectedContourPoints[16] =
					{	Point(0,0),Point(1,0),Point(2,0),Point(3,0),Point(4,0),
						Point(4,1),Point(4,2),Point(4,3),Point(4,4),
						Point(3,4), Point(2,4),Point(1,4),Point(0,4),
						Point(0,3),Point(0,2), Point(0,1) };
			
			Level* level = new Level(5, 5, 0xFE, WhiteSquareExpanded);
			Contour* contour = level->FindContour(nullptr, 0xFE);
			swprintf(Message, 100, L"Contour length %i should be %i", contour->Size(), 16);
			Assert::IsTrue(contour->Size() == expectedContourLength, Message);

			for (int i = 0; i < expectedContourLength; i++)
			{
				Point p = *contour->GetPoint(i);
				swprintf(Message, 100, L"Point %i isn't equal to reference. Point coords X=%i,Y=%i. Should be X=%i,Y=%i", i, p.X, p.Y, ExpectedContourPoints[i].X, ExpectedContourPoints[i].Y);
				Assert::IsTrue(ExpectedContourPoints[i] == p, L"Contour points does not match.");
			}
		}

		

	
	}; // class LevelTest_ExternalContour

	TEST_CLASS(LevelTest_InternalContour)
	{
	private:
		// 5x5 Square with black border and black point inside
	unsigned char BlackBorderAndBlackPointInside[100] =
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		};

		/*
		Check if function FindInternalContour return a value
		Check contour length
		*/
	public:
		// Try to find first internal contour point
		TEST_METHOD(TestMethod_1_1)
		{
			Point point = Point(MAXINT, MAXINT);
		
			//Create level filled with test data
			Level* level = new Level(5, 5, 0x00, BlackBorderAndBlackPointInside);
			// Find external contour
			Contour* contour = level->FindContour(nullptr, 0x00);
			Assert::IsNotNull(contour, L"Contour not found.");
			bool result  = level->FindFirstInternalContourPoint(contour, point);
			Assert::IsTrue(result);
		}

		// Check if first internal contour point has expected coords
		TEST_METHOD(TestMethod_1_2)
		{
			Point point = Point(MAXINT, MAXINT);
			Point expectedPoint = Point(1, 1);
			//Create level filled with test data
			Level* level = new Level(5, 5, 0x00, BlackBorderAndBlackPointInside);
			// Find external contour
			Contour* contour = level->FindContour(nullptr, 0x00);

			bool result = level->FindFirstInternalContourPoint(contour, point);
			Assert::IsTrue(result, L"First point not found.");
			
			Assert::IsTrue(point == expectedPoint,L"Points does not match.");

		}


	//	TEST_METHOD(TestMethod1)
	//	{
	//		Level* level = new Level(5, 5, 0x00, BlackBorderAndBlackPointInside);
	//		// Find external contour
	//		Contour* externalContour = level->FindContour(nullptr, 0x00);
	//		Contour* internalContour = level->FindInternalContour(nullptr);
	//		Assert::IsNotNull(internalContour);

	//	}

	///*
	//	Find contour of square area filled with white color (0xFE)
	//	Check contour length
	//*/

	//TEST_METHOD(TestMethod2)
	//{
	//	int expectedContourLength = 16;
	//	Point ExpectedContourPoints[16] =
	//	{ Point(0,0),Point(1,0),Point(2,0),Point(3,0),Point(4,0),
	//		Point(4,1),Point(4,2),Point(4,3),Point(4,4),
	//		Point(3,4), Point(2,4),Point(1,4),Point(0,4),
	//		Point(0,3),Point(0,2), Point(0,1) };

	//	//// Create level filled with test data
	//	//Level* level = new Level(5, 5, 0x00, BlackBorderAndBlackPointInside);
	//	//// Find external contour
	//	//Contour* contour1 = level->FindContour(nullptr, 0x00);
	//	//// Find internal contour of shape inside contour 1
	//	//Contour* contour2 = level->FindInternalContour(contour1);
	//	//// 
	//	//Assert::IsTrue(contour2->Size() == expectedContourLength, L"Number of points in contour not to expected");

	//	////for (int i = 0; i < expectedContourLength; i++)
	//	////{
	//	////	if (!(ExpectedContourPoints[i] == *contour->GetPoint(i)))
	//	////		Assert::IsTrue(false, L"Contour points does not match.");
	//	////}
	//}


	}; // class LevelTest_InternalContour

}  // namespace ContourHelpersTest