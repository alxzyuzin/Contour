#include "pch.h"
#include "CppUnitTest.h"
#include "..\ContourHelpers\Contour.h"
#include "..\ContourHelpers\Level.h"
#include "TestUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace  ContourHelpers;

namespace ContourHelpersTest
{
	const unsigned char E = 0xFF;
	const unsigned char B = 0x00;
	const unsigned char W = 0xFE;

	

	// 5x5 Square with black border inside (inverted DataSet_1)
	unsigned char DataSet_2[25] =
	{
		E, E, E, E, E,
		E, B, B, B, E,
		E, B, E, B, E,
		E, B, B, B, E,
		E, E, E, E, E,
	};


	// 5x5 Square border
	Point SquareBorderContourPoints_5x5[16] =
	{ 
		Point(0,0),Point(1,0),Point(2,0),Point(3,0),Point(4,0),
		Point(4,1),Point(4,2),Point(4,3),Point(4,4),
		Point(3,4), Point(2,4),Point(1,4),Point(0,4),
		Point(0,3),Point(0,2), Point(0,1) 
	};

	// 3x3 Square border
	Point SquareBorderContourPoints_3x3[8] =
	{
		Point(1,1),Point(2,1),Point(3,1),Point(3,2),Point(3,3),
		Point(3,3),Point(1,3),Point(1,2) 
	};

	Direction AllDirections[8] = 
	{ 
		Direction::N,
		Direction::NE,
		Direction::E,
		Direction::SE,
		Direction::S,
		Direction::SW,
		Direction::W,
		Direction::NW
	};


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
		wchar_t Message[100];

		/*
		5x5 area of the same color pixels (pixel color 0xFE) 
		Each pixel presented by 4 bytes
		*/

		//// Empty square size 5x5 with black point in center
		//unsigned char EmptySquareWithBlackPoint[100] =
		//{   E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
		//	E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
		//	E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
		//	E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
		//	E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E
		//};

		// White 5x5 square

    public:
		unsigned char WhiteSquare[25] =
		{ W, W, W, W, W,
			W, W, W, W, W,
			W, W, W, W, W,
			W, W, W, W, W,
			W, W, W, W, W
		};

		/*
		Analize 5x5 square area filled by white color (0xFE) 
		Look for first shape pixel with color 0xFE
		OK - if pixel found (function FindFirstContourPoint return true)
		*/
        TEST_METHOD(Test_01_FindFirstExternalContourPoint)
        {
			unsigned char WhiteSquareExpanded[100];
			Level::ExpandLevelData(5, 5, W, WhiteSquare, WhiteSquareExpanded);
			Level* level = new Level(5, 5, W, WhiteSquareExpanded);
			Point firstContourPoint = Point(5, 5);
			Point ExpectedPoint = Point(0, 0);
			bool r = level->FindFirstExternalContourPoint(firstContourPoint);
			Assert::IsTrue(r);
        }

		/*
		Find first external contour point if parent contour is null
		Check coords first shape pixel with color 0xFE
		OK - if expected coords == first contour point coords
		*/
		TEST_METHOD(Test_02_FindFirstExternalContourPoint)
		{
			unsigned char WhiteSquareExpanded[100];
			Level::ExpandLevelData(5, 5, W, WhiteSquare, WhiteSquareExpanded);

			Level* level = new Level(5, 5, W, WhiteSquareExpanded);
			Point firstContourPoint = Point(5, 5);
			Point ExpectedPoint = Point(0, 0);
			level->FindFirstExternalContourPoint(firstContourPoint);
			Assert::IsTrue(firstContourPoint == ExpectedPoint);
		}

		
		/*
		Find next pixel if current pixel lays in upper right corner of OneColor area 0xFE
		Parent contour is null
		OK - if expected coords == X = 4 Y = 1
		*/
		TEST_METHOD(Test_03_FindNextExternalContourPoint)
		{
			unsigned char WhiteSquareExpanded[100];
			Level::ExpandLevelData(5, 5, W, WhiteSquare, WhiteSquareExpanded);
			
			Level* level = new Level(5, 5, 0xFE, WhiteSquareExpanded);
			Point currentPoint = Point(4, 0);
			Point nextExpectedPoint = Point(4, 1);
			level->FindNextExternalContourPoint(currentPoint, Direction::S);
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
		TEST_METHOD(Test_04_FindExternalContour)
		{
			unsigned char EmptySquareExpanded[100];
			Level::ExpandLevelData(5, 5, B, EmptySquare, EmptySquareExpanded);
			
			Level* level = new Level(5, 5, B, EmptySquareExpanded);
			Contour* contour = level->FindExternalContour();
			Assert::IsNull(contour, L"contour is not null");
		}



		unsigned char Dataset_05[25] =
		{ W, W, W, W, W,
			W, W, W, W, W,
			W, W, W, W, W,
			W, W, W, W, W,
			W, W, W, W, W
		};
		/*
		Find contour of square area filled with white color (0xFE)
		Check contour length
		*/
		TEST_METHOD(Test_05_FindExternalContour)
		{
			unsigned char WhiteSquareExpanded[100];
			Level::ExpandLevelData(5, 5, W, Dataset_05, WhiteSquareExpanded);
			
			// use this expression to find array size
			//int size = *(&arr + 1) - arr;
			int expectedContourLength = 16;
						
			Level* level = new Level(5, 5, W, WhiteSquareExpanded);
			Contour* contour = level->FindExternalContour();
			swprintf(Message, 100, L"Contour length %i should be %i", contour->Size(), 16);
			Assert::IsTrue(contour->Size() == expectedContourLength, Message);

			for (int i = 0; i < expectedContourLength; i++)
			{
				Point p = *contour->GetPoint(i);
				swprintf(Message, 100, L"Point %i isn't equal to reference. Point coords X=%i,Y=%i. Should be X=%i,Y=%i",
					i, p.X, p.Y, SquareBorderContourPoints_5x5[i].X, SquareBorderContourPoints_5x5[i].Y);
				Assert::IsTrue(SquareBorderContourPoints_5x5[i] == p, Message);
			}
		}


		// 5x5 Square with black border and black point inside
		unsigned char DataSet_06[25] =
		{
			B, B, B, B, B,
			B, E, E, E, B,
			B, E, B, E, B,
			B, E, E, E, B,
			B, B, B, B, B,
		};

		/*
		Find contour of square area having black border and black point in center
		Check contour length
		*/
		TEST_METHOD(Test_06_FindExternalContour)
		{
			wchar_t Message[100];
			unsigned char DataSetExpanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_06, DataSetExpanded);

			// use this expression to find array size
			//int size = *(&arr + 1) - arr;
			int expectedContourLength = 16;

			Level* level = new Level(5, 5, B, DataSetExpanded);
			Contour* contour = level->FindExternalContour();
			swprintf(Message, 100, L"Contour length %i should be %i", contour->Size(), 16);
			Assert::IsTrue(contour->Size() == expectedContourLength, Message);

			for (int i = 0; i < expectedContourLength; i++)
			{
				Point p = *contour->GetPoint(i);
				swprintf(Message, 100, L"Point %i isn't equal to reference. Point coords X=%i,Y=%i. Should be X=%i,Y=%i",
					i, p.X, p.Y, SquareBorderContourPoints_5x5[i].X, SquareBorderContourPoints_5x5[i].Y);
				Assert::IsTrue(SquareBorderContourPoints_5x5[i] == p, L"Contour points does not match.");
			}
		}

		unsigned char DataSet_07[231] =
		{   E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, B, B, B, E, B, B, B, B, B, B, B, B, E, E, E, B, B, B, E,
			E, E, B, B, B, B, B, B, B, B, B, B, B, B, E, E, E, E, B, B, E,
			E, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, E, B, B, E, E,
			E, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, E, B, B, E, E,
			E, B, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, B, B, E, E,
			E, E, B, B, B, E, B, B, B, B, B, B, B, B, E, B, B, B, B, E, E,
			E, E, B, B, E, E, B, B, B, B, B, B, B, E, E, B, B, B, B, E, E,
			E, E, E, E, E, B, B, B, B, E, B, B, B, E, E, B, B, B, B, E, E,
			E, E, E, E, B, B, B, B, E, E, E, B, B, E, E, B, B, B, B, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E
		};

		TEST_METHOD(Test_07_FindExternalContour)
		{
			wchar_t Message[100];
			unsigned char DataSetExpanded[924];
			Level::ExpandLevelData(21, 11, B, DataSet_07, DataSetExpanded);
			Level* level = new Level(21, 11, B, DataSetExpanded);
			Contour* contour = level->FindExternalContour();
			swprintf(Message, 100, L"Contour length should be %i", 16);
			Assert::AreEqual(64, contour->Length, Message);
		}

	
	}; // class LevelTest_ExternalContour

	TEST_CLASS(LevelTest_InternalContour)
	{
	private:
		wchar_t Message[100];
	public:

		// 5x5 Square with black border and black point inside
		unsigned char DataSet_1[25] =
		{
			B, B, B, B, B,
			B, E, E, E, B,
			B, E, B, E, B,
			B, E, E, E, B,
			B, B, B, B, B,
		};

		// Try to find first internal contour point
		TEST_METHOD(Test_01_FindFirstInternalContourPoint)
		{
			unsigned char DataSetExpanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_1, DataSetExpanded);
			Point point = Point(MAXINT, MAXINT);
			Level* level = new Level(5, 5, B, DataSetExpanded);
			Contour* contour = level->FindExternalContour();
			Assert::IsNotNull(contour, L"Contour not found.");
			bool result  = level->FindFirstInternalContourPoint(contour, point);
			Assert::IsTrue(result);
		}

		// 5x5 Square with black border and black point inside
		unsigned char DataSet_2[25] =
		{
			B, B, B, B, B,
			B, E, E, E, B,
			B, E, B, E, B,
			B, E, E, E, B,
			B, B, B, B, B,
		};

		// Check if first internal contour point has expected coords
		TEST_METHOD(Test_02_FindFirstInternalContourPoint)
		{
			unsigned char DataSetExpanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_2, DataSetExpanded);

			Point point = Point(MAXINT, MAXINT);
			Point expectedPoint = Point(1, 1);
			//Create level filled with test data
			Level* level = new Level(5, 5, B, DataSetExpanded);
			// Find external contour
			Contour* contour = level->FindExternalContour();

			bool result = level->FindFirstInternalContourPoint(contour, point);
			Assert::IsTrue(result, L"First point not found.");
			
			Assert::IsTrue(point == expectedPoint,L"Points does not match.");
		}


		unsigned char DataSet_03[25] =
		{
			E, E, E, E, E,
			E, B, B, B, E,
			E, B, E, B, E,
			E, B, B, B, E,
			E, E, E, E, E,
		};
		// Check if next internal contour point if defined directions 
		// can not be part of contour
		TEST_METHOD(Test_03_CheckNextInternalContourPoint)
		{
			unsigned char DataSetExpanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_03, DataSetExpanded);
			//Create level filled with test data
			Level* level = new Level(5, 5, B, DataSetExpanded);
			// Find external contour
			Contour* contour = level->FindExternalContour();
			for (int i = 0; i < 8; i++)
			{
				Point currentPoint = Point(2, 2);
				bool result = level->CheckNextInternalContourPoint(contour, currentPoint, AllDirections[i]);
				swprintf(Message, 100, L"Neibor point in direction %i can be internal contour point", i);
				Assert::IsFalse(result, Message);
			}
		}

		// Check if neibor point in all defined direction can be next internal contour point
		TEST_METHOD(CheckNextInternalContourPoint_Test_2)
		{
			unsigned char DataSetExpanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_1, DataSetExpanded);
			//Create level filled with test data
			Level* level = new Level(5, 5, B, DataSetExpanded);
			// Find external contour
			Contour* contour = level->FindExternalContour();

			for (int i = 0; i < 8; i++)
			{
				Point currentPoint = Point(2, 2);
				bool result = level->CheckNextInternalContourPoint(contour, currentPoint, AllDirections[i]);
				swprintf(Message, 100, L"Neibor point in direction %i cannot be internal contour point", i);
				Assert::IsTrue(result, Message);
			}
		}


		unsigned char DataSet_0[25] =
		{
			B, B, B, B, B,
			B, E, E, E, B,
			B, E, B, E, B,
			B, E, E, E, B,
			B, B, B, B, B,
		};
		Point ContourPoints[8] =
		{
			Point(1,1), Point(2,1), Point(3,1), Point(3,2),
			Point(3,3), Point(2,3), Point(1,3),	Point(1,2)
		};

		// Check function parameter value control
		TEST_METHOD(Test_0_FindInternalContour)
		{
			/*Assert::ExpectException< std::invalid_argument, Contour*>([]()
				{
					unsigned char DataSetExpanded[100];
					Level::ExpandLevelData(5, 5, B, DataSet_0, DataSetExpanded);
					Level* level = new Level(5, 5, B, DataSetExpanded);
					return level->FindInternalContour(nullptr);
				}, L"Parametr parentContour is null and exception not thrown.");*/
		}

		// Find internal contour
		TEST_METHOD(Test_05_FindInternalContour)
		{
			unsigned char DataSetExpanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_1, DataSetExpanded);
			//Create level filled with test data
			Level* level = new Level(5, 5, B, DataSetExpanded);
			// Find external contour
			Contour* externalCountour = level->FindExternalContour();

			Contour* internalContour = level->FindInternalContour(externalCountour);
			Assert::AreEqual(8, internalContour->Size(), L"Contour size does not match expected");

			for (int i = 0; i < 8; i++)
			{
				Point* point = internalContour->GetPoint(i);
				swprintf(Message, 100, L"Point %i isn't equal to reference. Point coords X=%i,Y=%i. Should be X=%i,Y=%i",
					i, point->X, point->Y, ContourPoints[i].X, ContourPoints[i].Y);
				Assert::IsTrue(*point == ContourPoints[i]);
			}
		
		}

		TEST_METHOD(FindNextInternalContourPoint_Test_0)
		{
			unsigned char DataSetExpanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_1, DataSetExpanded);
			//Create level filled with test data
			Level* level = new Level(5, 5, B, DataSetExpanded);
			// Find external contour
			Contour* contour = level->FindExternalContour();

			Point firstPoint = Point(1, 1);
			Point NextPoint = Point(2, 1);
			Direction initialDirection = Direction::E;
			bool res = level->FindNextInternalContourPoint(contour, firstPoint, initialDirection);
			Assert::IsTrue(res);
		}

		unsigned char DataSet_3[81] =
		{
			B, B, B, B, B, B, B, B, B,
			B, E, E, E, E, E, E, E, B,
			B, B, E, B, E, B, B, E, B,
			B, B, E, E, E, E, B, E, B,
			B, E, E, E, E, E, E, E, B,
			B, E, B, B, B, B, B, E, B,
			B, E, B, E, E, E, B, E, B,
			B, E, B, E, B, E, B, E, B,
			B, B, B, E, B, E, B, B, B,
		};

		// Test looking for next point 
		TEST_METHOD(FindNextInternalContourPoint_Test_1)
		{
			unsigned char DataSetExpanded[324];
			Level::ExpandLevelData(9, 9, B, DataSet_3, DataSetExpanded);
			//Create level filled with test data
			Level* level = new Level(9, 9, B, DataSetExpanded);
			// Find external contour
			Contour* contour = level->FindExternalContour();

			Point CurrentPoint = Point(1, 5);
			Point ExpectedPoint = Point(1, 6);
			Direction direction = Direction::SW;
			bool res = level->FindNextInternalContourPoint(contour, CurrentPoint, direction);
			swprintf(Message, 100, L"Point isn't equal to reference. Point coords X=%i,Y=%i. Should be X=%i,Y=%i",
				CurrentPoint.X, CurrentPoint.Y, ExpectedPoint.X, ExpectedPoint.Y);
			Assert::IsTrue(CurrentPoint == ExpectedPoint, Message);
		}

		
		unsigned char DataSet_9[25] =
		{
			E, E, E, E, E,
			E, B, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E

			
		};
		Point Contour_9_External[1] = {	Point(1,1) };

		// Find internal contour in external contour if external contour length is 1
		TEST_METHOD(InternalContour_Test_9)
		{
			unsigned char DataSetExpanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_9, DataSetExpanded);
			//Create level filled with test data
			Level* level = new Level(5, 5, B, DataSetExpanded);
			// Find external contour
			Contour* externalCountour = new Contour(Contour_9_External, 1);

			Contour* internalContour = level->FindInternalContour(externalCountour);
			Assert::IsNull(internalContour, L" Internal contour iz not null");

		}

		unsigned char DataSet_10[81] =
		{
			B, B, B, B, B, B, B, B, B,
			B, B, B, B, B, B, B, B, B,
			B, B, B, B, B, E, E, B, B,
			B, B, B, B, E, E, B, B, B,
			B, B, B, E, E, E, B, B, B,
			B, B, B, E, E, B, B, B, B,
			B, B, B, E, E, B, B, B, B,
			B, B, B, B, B, B, B, B, B,
			B, B, B, B, B, B, B, B, B
		};
		

		Point Contour_10_Internal[10] = 
		{
			Point(5,2), Point(6,2), Point(5,3), Point(5,4),Point(4,5), Point(4,6),
			Point(3,6), Point(3,5), Point(3,4), Point(4,3)
		
		};

		TEST_METHOD(Test_10_InternalContour)
		{
			unsigned char DataSetExpanded[324];
			Level::ExpandLevelData(9, 9, B, DataSet_10, DataSetExpanded);
			Level* level = new Level(9, 9, B, DataSetExpanded);
			Contour* externalCountour = level->FindExternalContour();
			Contour* internalContour = level->FindInternalContour(externalCountour);
			bool res = TestUtils::CompareContours(internalContour, Contour_10_Internal,10, Message, 100);
			Assert::IsTrue(res, Message);
		}

		unsigned char DataSet_11[231] =
		{
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, B, B, B, E, B, B, B, B, B, B, B, B, E, E, E, B, B, B, E,
			E, E, B, B, B, B, B, B, B, B, B, B, B, B, E, E, E, E, B, B, E,
			E, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, E, B, B, E, E,
			E, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, E, B, B, E, E,
			E, B, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, B, B, E, E,
			E, E, B, B, B, E, B, B, B, B, B, B, B, B, E, B, B, B, B, E, E,
			E, E, B, B, E, E, B, B, B, B, B, B, B, E, E, B, B, B, B, E, E,
			E, E, E, E, E, B, B, B, B, E, B, B, B, E, E, B, B, B, B, E, E,
			E, E, E, E, B, B, B, B, E, E, E, B, B, E, E, B, B, B, B, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E
		};

		
		TEST_METHOD(Test_11_FindInternalContour)
		{
			unsigned char DataSetExpanded[924];
			Level::ExpandLevelData(21, 11, B, DataSet_11, DataSetExpanded);
			Level* level = new Level(21, 11, B, DataSetExpanded);
			Contour* externalCountour = level->FindExternalContour();
			Contour* internalContour = level->FindInternalContour(externalCountour);
			Assert::IsNull(internalContour,L"Internal contour is not null");
		}

		unsigned char DataSet_12[231] =
		{
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, B, B, B, E, B, E, B, B, B, B, B, B, E, E, E, B, B, B, E,
			E, E, B, B, B, B, B, B, B, B, B, B, B, B, E, E, E, E, B, B, E,
			E, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, E, B, B, E, E,
			E, B, B, B, B, B, B, B, B, E, B, B, B, B, E, E, E, B, B, E, E,
			E, B, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, B, B, E, E,
			E, E, B, B, B, E, B, B, B, B, B, B, B, B, E, B, B, B, B, E, E,
			E, E, B, B, E, E, B, B, B, B, B, B, B, E, E, B, B, B, B, E, E,
			E, E, E, E, E, B, B, B, B, E, B, B, B, E, E, B, B, B, B, E, E,
			E, E, E, E, B, B, B, B, E, E, E, B, B, E, E, B, B, B, B, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E
		};

		TEST_METHOD(Test_12_FindInternalContour)
		{
			unsigned char DataSetExpanded[924];
			Level::ExpandLevelData(21, 11, B, DataSet_12, DataSetExpanded);
			Level* level = new Level(21, 11, B, DataSetExpanded);
			Contour* externalCountour = level->FindExternalContour();
			Contour* internalContour = level->FindInternalContour(externalCountour);
			Assert::AreEqual(1,internalContour->Length, L"Invalid internal contour length");
		}

	}; // class LevelTest_InternalContour

	TEST_CLASS(LevelTest_FindAllContours)
	{
		private:
			wchar_t Message[100];
			array<wchar_t, 100> Msg;
			unsigned char DataSet_0[81] =
			{
				B, B, B, B, B, B, B, B, B,
				B, E, E, E, E, E, E, E, B,
				B, B, E, B, E, B, B, E, B,
				B, B, E, E, E, E, B, E, B,
				B, E, E, E, E, E, E, E, B,
				B, E, B, B, B, B, B, E, B,
				B, E, B, E, E, E, B, E, B,
				B, E, B, E, B, E, B, E, B,
				B, B, B, E, B, E, B, B, B,
			};

			Point Contour_0_1_External[36] =
			{
				Point(0,0), Point(1,0), Point(2,0), Point(3,0), Point(4,0),	Point(5,0),
				Point(6,0), Point(7,0), Point(8,0), Point(8,1), Point(8,2), Point(8,3),
				Point(8,4), Point(8,5), Point(8,6), Point(8,7), Point(8,8), Point(7,8),
				Point(6,8), Point(6,7), Point(6,6), Point(5,5), Point(4,5), Point(3,5), 
				Point(2,6),	Point(2,7), Point(2,8), Point(1,8), Point(0,8), Point(0,7),
				Point(0,6),	Point(0,5), Point(0,4),	Point(0,3), Point(0,2),	Point(0,1)
			};

			Point Contour_0_2_Internal[28] =
			{
				Point(1,1), Point(2,1), Point(3,1), Point(4,1), Point(5,1),	Point(6,1),
				Point(7,1), Point(7,2), Point(7,3), Point(7,4), Point(7,5),	Point(7,6),
				Point(7,7), Point(7,6), Point(7,5),	Point(6,4), Point(5,4),	Point(4,4),
				Point(3,4), Point(2,4), Point(1,5),	Point(1,6), Point(1,7),	Point(1,6),
				Point(1,5), Point(1,4), Point(2,3),	Point(2,2)
			};

			Point Contour_0_3_External[1] =
			{
				Point(3,2)
			};

			Point Contour_0_4_External[3] =
			{
				Point(5,2), Point(6,2), Point(6,3)
			};

			Point Contour_0_5_External[2] =
			{
				Point(4,7), Point(4,8)
			};

		public:
			
			TEST_METHOD(FindAllContours_Test_0)
			{
				unsigned char DataSetExpanded[324];
				Level::ExpandLevelData(9, 9, B, DataSet_0, DataSetExpanded);
				Level* level = new Level(9, 9, B, DataSetExpanded);
				level->FindAllContours();
				Assert::AreEqual(3, (int)level->m_Contours.size());
			}
		

	};

	/*
	Набор тестов функции проверки принадлежности точки контуру ContainsPoint(int x, int y)
	*/
	TEST_CLASS(ContourTest_PointInsideContour)
	{
	private:
		wchar_t Message[100];
		Point Contour_1[8] =
		{
			Point(1,1), Point(2,1), Point(3,1), Point(3,2),
			Point(3,3), Point(2,3), Point(1,3),	Point(1,2)
		};

		Point Contour_2[36] =
		{
			Point(0,0), Point(1,0), Point(2,0), Point(3,0), Point(4,0),	Point(5,0),
			Point(6,0), Point(7,0), Point(8,0), Point(8,1), Point(8,2), Point(8,3),
			Point(8,4), Point(8,5), Point(8,6), Point(8,7), Point(8,8), Point(7,8),
			Point(6,8), Point(6,7), Point(6,6), Point(5,5), Point(4,5), Point(3,5),
			Point(2,6),	Point(2,7), Point(2,8), Point(1,8), Point(0,8), Point(0,7),
			Point(0,6),	Point(0,5), Point(0,4),	Point(0,3), Point(0,2),	Point(0,1)
		};

	public:
		// Check function ContainsPoint return true if point resides inside contour
		// Expected result true
		TEST_METHOD(ContourContainsPoint_Test_0)
		{
			Contour* contour = new Contour(Contour_1, 8);
			Assert::IsTrue(contour->ContainsPoint(2, 2));
		}

		// Check function ContainsPoint return false if point resides on contour
		TEST_METHOD(ContourContainsPoint_Test_1)
		{
			Contour* contour = new Contour(Contour_1, 8);
			Assert::IsFalse(contour->ContainsPoint(1, 2));
		}

		// Check function ContainsPoint return false if point resides on contour
		// Expected result true
		TEST_METHOD(ContourContainsPoint_Test_2)
		{
			Contour* contour = new Contour(Contour_1, 8);
			Assert::IsFalse(contour->ContainsPoint(0, 1));
		}
		// Check function ContainsPoint return false if point resides on contour
		// Expected result true
		TEST_METHOD(ContourContainsPoint_Test_3)
		{
			Contour* contour = new Contour(Contour_1, 8);
			Assert::IsFalse(contour->ContainsPoint(0, 2));
		}

		// Test if defined point lays inside contour	
		TEST_METHOD(ContourContainsPoint_Test_4)
		{
			Contour* contour = new Contour(Contour_2, 36);
			Assert::IsTrue(contour->ContainsPoint(1, 5));
		}

		TEST_METHOD(ContourContainsPoint_Test_5)
		{
			Contour* contour = new Contour(Contour_2, 36);
			Assert::IsFalse(contour->ContainsPoint(4, 7));
		}

		Point Contour_06[263] =
		{
			Point(380, 173), Point(381, 173), Point(381, 174), Point(381, 175), Point(381, 176),
			Point(381, 177), Point(381, 178), Point(381, 179), Point(381, 180), Point(381, 181),
			Point(381, 182), Point(381, 183), Point(381, 184), Point(381, 185), Point(380, 186),
			Point(380, 187), Point(380, 188), Point(380, 189), Point(380, 190), Point(380, 191),
			Point(380, 192), Point(380, 193), Point(380, 194), Point(379, 195), Point(379, 196),
			Point(379, 197), Point(379, 198), Point(378, 199), Point(378, 200), Point(378, 201),
			Point(378, 202), Point(378, 203), Point(377, 204), Point(377, 205), Point(376, 205),
			Point(375, 205), Point(374, 205), Point(373, 204), Point(373, 203), Point(372, 202),
			Point(373, 201), Point(374, 200), Point(375, 199), Point(375, 198), Point(376, 197),
			Point(376, 196), Point(377, 195), Point(377, 194), Point(377, 193), Point(377, 192),
			Point(377, 191), Point(377, 190), Point(377, 189), Point(376, 188), Point(375, 189),
			Point(374, 190), Point(374, 191), Point(374, 192), Point(374, 193), Point(373, 194),
			Point(372, 195), Point(372, 196), Point(371, 197), Point(371, 198), Point(371, 199),
			Point(371, 200), Point(370, 200), Point(369, 200), Point(368, 200), Point(367, 200),
			Point(366, 201), Point(366, 202), Point(367, 203), Point(367, 204), Point(368, 205),
			Point(368, 206), Point(369, 207), Point(369, 208), Point(370, 209), Point(371, 209),
			Point(372, 208), Point(373, 208), Point(374, 209), Point(375, 210), Point(375, 211),
			Point(375, 212), Point(375, 213), Point(375, 214), Point(374, 215), Point(373, 216),
			Point(373, 217), Point(373, 218), Point(373, 219), Point(372, 220), Point(372, 221),
			Point(371, 222), Point(370, 223), Point(370, 224), Point(369, 225), Point(368, 225),
			Point(367, 224), Point(366, 223), Point(365, 222), Point(364, 222), Point(363, 222),
			Point(362, 222), Point(361, 221), Point(360, 221), Point(359, 222), Point(359, 223),
			Point(359, 224), Point(360, 225), Point(361, 225), Point(361, 226), Point(361, 227),
			Point(361, 228), Point(360, 229), Point(359, 229), Point(358, 230), Point(357, 230),
			Point(356, 229), Point(355, 228), Point(354, 228), Point(353, 227), Point(352, 226),
			Point(351, 225), Point(351, 224), Point(350, 223), Point(350, 222), Point(349, 221),
			Point(350, 220), Point(350, 219), Point(350, 218), Point(350, 217), Point(350, 216),
			Point(350, 215), Point(350, 214), Point(350, 213), Point(350, 212), Point(351, 211),
			Point(351, 210), Point(352, 209), Point(352, 208), Point(353, 207), Point(354, 206),
			Point(355, 206), Point(356, 206), Point(357, 205), Point(358, 204), Point(359, 203),
			Point(360, 202), Point(361, 201), Point(362, 200), Point(363, 200), Point(364, 200),
			Point(365, 199), Point(366, 199), Point(367, 198), Point(367, 197), Point(367, 196),
			Point(367, 195), Point(368, 194), Point(369, 193), Point(369, 192), Point(368, 191),
			Point(367, 190), Point(366, 191), Point(365, 192), Point(365, 193), Point(364, 194),
			Point(363, 195), Point(362, 196), Point(361, 197), Point(360, 198), Point(359, 198),
			Point(358, 198), Point(357, 198), Point(356, 198), Point(355, 199), Point(354, 199),
			Point(354, 198), Point(354, 197), Point(354, 196), Point(354, 195), Point(354, 194),
			Point(354, 193), Point(355, 194), Point(356, 195), Point(357, 194), Point(358, 193),
			Point(358, 192), Point(359, 191), Point(360, 190), Point(360, 189), Point(359, 188),
			Point(358, 189), Point(357, 190), Point(356, 190), Point(356, 189), Point(355, 188),
			Point(355, 187), Point(355, 186), Point(356, 185), Point(356, 184), Point(357, 184),
			Point(358, 184), Point(359, 185), Point(360, 184), Point(360, 183), Point(360, 182),
			Point(359, 181), Point(359, 180), Point(359, 179), Point(359, 178), Point(360, 177),
			Point(361, 177), Point(362, 177), Point(363, 177), Point(363, 178), Point(364, 179),
			Point(365, 180), Point(366, 180), Point(367, 179), Point(367, 178), Point(368, 179),
			Point(369, 179), Point(370, 179), Point(371, 179), Point(372, 178), Point(373, 177),
			Point(373, 176), Point(373, 175), Point(374, 175), Point(375, 176), Point(376, 177),
			Point(377, 178), Point(377, 179), Point(377, 180), Point(377, 181), Point(376, 182),
			Point(376, 183), Point(375, 184), Point(375, 185), Point(375, 186), Point(375, 187),
			Point(376, 188), Point(377, 189), Point(378, 189), Point(379, 188), Point(379, 187),
			Point(379, 186), Point(380, 185), Point(379, 184), Point(379, 183), Point(379, 182),
			Point(379, 181), Point(379, 180), Point(379, 179), Point(379, 178), Point(379, 177),
			Point(379, 176), Point(380, 175), Point(380, 174)
		};

		TEST_METHOD(Test_06_ContourContainsPoint)
		{
			Contour* contour = new Contour(Contour_06, 263);
			Assert::IsFalse(contour->ContainsPoint(377, 189));
		}

		unsigned char DataSet_07[231] =
		{
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, B, B, B, E, B, B, B, B, B, B, B, B, E, E, E, B, B, B, E,
			E, E, B, B, B, B, B, B, B, B, B, B, B, B, E, E, E, E, B, B, E,
			E, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, E, B, B, E, E,
			E, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, E, B, B, E, E,
			E, B, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, B, B, E, E,
			E, E, B, B, B, E, B, B, B, B, B, B, B, B, E, B, B, B, B, E, E,
			E, E, B, B, E, E, B, B, B, B, B, B, B, E, E, B, B, B, B, E, E,
			E, E, E, E, E, B, B, B, B, E, B, B, B, E, E, B, B, B, B, E, E,
			E, E, E, E, B, B, B, B, E, E, E, B, B, E, E, B, B, B, B, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E
		};

		TEST_METHOD(Test_07_ContourContainsPoint)
		{
			unsigned char DataSetExpanded[924];
			Level::ExpandLevelData(21, 11, B, DataSet_07, DataSetExpanded);
			Level* level = new Level(21, 11, B, DataSetExpanded);
			Contour* contour = level->FindExternalContour();
			Assert::IsFalse(contour->ContainsPoint(5, 6));
		}


	};

	TEST_CLASS(ContourTest_FindRightNearestContourPoint)
	{
	private:
		wchar_t Message[100];
		Point Contour_1[8] =
		{
			Point(1,1), Point(2,1), Point(3,1), Point(3,2),
			Point(3,3), Point(2,3), Point(1,3),	Point(1,2)
		};

	public:
		// Point resides inside contour
		//TEST_METHOD(FindRightNearestPoint_Test_0)
		//{
		//	Contour* contour = new Contour(Contour_1, 8);
		//	Point* startPoint = new Point(2, 2);
		//	Point* foundPoint = contour->GetRightNearestContourPoint(startPoint);
		//	Point ExpectedPoint = Point(3, 2);
		//	swprintf(Message, 100, L"Found point X=%i,Y=%i. Expected point X=%i,Y=%i",
		//		foundPoint->X, foundPoint->Y,
		//		ExpectedPoint.X, ExpectedPoint.Y);
		//	Assert::IsTrue(*foundPoint == ExpectedPoint, Message);
		//}

		// Point resides on left outside contour
	/*	TEST_METHOD(FindRightNearestPoint_Test_1)
		{
			Contour* contour = new Contour(Contour_1, 8);
			Point* startPoint = new Point(0, 2);
			Point  ExpectedPoint = Point(1, 2);
			Point* foundPoint = contour->GetRightNearestContourPoint(startPoint);
			swprintf(Message, 100, L"Found point X=%i,Y=%i. Expected point X=%i,Y=%i",
				foundPoint->X, foundPoint->Y,
				ExpectedPoint.X, ExpectedPoint.Y);
			Assert::IsTrue(*foundPoint == ExpectedPoint, Message);
		}*/

		// Point resides on right outside contour
	/*	TEST_METHOD(FindRightNearestPoint_Test_2)
		{
			Contour* contour = new Contour(Contour_1, 8);
			Point* startPoint = new Point(4, 2);
			Point* foundPoint = contour->GetRightNearestContourPoint(startPoint);
			Assert::IsNull(foundPoint, L"Point not expected");
		}*/


	};

	TEST_CLASS(LevelTest_EraseContour)
	{
	private:
		wchar_t Message[100];
		array<wchar_t, 100> Msg;

		unsigned char DataSet_0_Initial[25] =
		{
			B, B, B, B, B,
			B, B, B, B, B,
			B, B, B, B, B,
			B, B, B, B, B,
			B, B, B, B, B,
		};

		unsigned char DataSet_0_Final[25] =
		{
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
		};

		Point SquareBorderContourPoints_5x5[16] =
		{
			Point(0, 0), Point(1, 0), Point(2, 0), Point(3, 0),
			Point(4, 0), Point(4, 1), Point(4, 2), Point(4, 3),
			Point(4, 4), Point(3, 4), Point(2, 4), Point(1, 4),
			Point(0, 4), Point(0, 3), Point(0, 2), Point(0, 1)
		};



	public:
		TEST_METHOD(EraseShape_Test_00)
		{
			unsigned char InitialDataSetExpanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_0_Initial, InitialDataSetExpanded);
			////Create level filled with test data
			Level* level = new Level(5, 5, B, InitialDataSetExpanded);
			Contour* externalContour = new Contour(SquareBorderContourPoints_5x5, 16);
			level->EraseShape(externalContour, nullptr);
			bool r = level->CompareLevelDataWithReferenceData(DataSet_0_Final, Message, 100);
			Assert::IsTrue(r, Message);
		}

		unsigned char DataSet_1_Initial[25] =
		{
			B, B, B, B, B,
			B, E, E, E, B,
			B, E, B, E, B,
			B, E, E, E, B,
			B, B, B, B, B,
		};

		unsigned char DataSet_1_Final[25] =
		{
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, B, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
		};

		TEST_METHOD(Test_01_EraseShape)
		{
			unsigned char InitialDataSetExpanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_1_Initial, InitialDataSetExpanded);
			Level* level = new Level(5, 5, B, InitialDataSetExpanded);
			Contour* externalContour = level->FindExternalContour(); 
			Contour* internalContour = level->FindInternalContour(externalContour); 
			level->EraseShape(externalContour, internalContour);
			bool r = level->CompareLevelDataWithReferenceData(DataSet_1_Final, Message, 100);
			Assert::IsTrue(r, Message);
		}


		unsigned char DataSet_2_Initial[25] =
		{
			B, B, B, B, B,
			B, E, E, E, B,
			B, E, B, B, B,
			B, E, B, E, E,
			B, B, B, E, B,
		};

		unsigned char DataSet_2_Final[25] =
		{
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, B,
		};


		TEST_METHOD(Test_02_EraseShape)
		{
			unsigned char InitialDataSetExpanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_2_Initial, InitialDataSetExpanded);
			Level* level = new Level(5, 5, B, InitialDataSetExpanded);
			Contour* externalContour = level->FindExternalContour();// new Contour(ExternalContour_2, 16);
			Contour* internalContour = level->FindInternalContour(externalContour);// new Contour(InternalContour_2, 8);
			level->EraseShape(externalContour, internalContour);
			bool r = level->CompareLevelDataWithReferenceData(DataSet_2_Final, Message, 100);
			Assert::IsTrue(r, Message);
		}

		unsigned char DataSet_3_Initial[81] =
		{
			B, B, B, B, B, B, B, B, B,
			B, E, E, E, E, E, E, E, B,
			B, B, E, B, E, B, B, E, B,
			B, B, E, E, E, E, B, E, B,
			B, E, E, E, E, E, E, E, B,
			B, E, B, B, B, B, B, E, B,
			B, E, B, E, E, E, B, E, B,
			B, E, B, E, B, E, B, E, B,
			B, B, B, E, B, E, B, B, B,
		};

		unsigned char DataSet_3_Final[81] =
		{
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, B, E, B, B, E, E,
			E, E, E, E, E, E, B, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, B, E, E, E, E,
			E, E, E, E, B, E, E, E, E,
		};

		TEST_METHOD(Test_03_EraseShape)
		{
			unsigned char InitialDataSetExpanded[324];
			Level::ExpandLevelData(9, 9, B, DataSet_3_Initial, InitialDataSetExpanded);
			Level* level = new Level(9, 9, B, InitialDataSetExpanded);
			Contour* externalContour = level->FindExternalContour();
			Contour* internalContour = level->FindInternalContour(externalContour);
			level->EraseShape(externalContour, internalContour);
			bool r = level->CompareLevelDataWithReferenceData(DataSet_3_Final, Message, 100);
			Assert::IsTrue(r, Message);
		}

		unsigned char DataSet_4_Initial[25] =
		{
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, B, E,
			E, E, E, E, E,
			E, E, E, E, E,
			
		};

		unsigned char DataSet_4_Final[25] =
		{
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
		};

		Point ExternalContour_4[1] = { Point(3,2) };

		
		TEST_METHOD(EraseShape_Test_04)
		{
			unsigned char InitialDataSetExpanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_4_Initial, InitialDataSetExpanded);
			////Create level filled with test data
			Level* level = new Level(5, 5, B, InitialDataSetExpanded);
			Contour* externalContour = new Contour(ExternalContour_4, 1);
			Contour* internalContour = nullptr;
			level->EraseShape(externalContour, internalContour);
			bool r = level->CompareLevelDataWithReferenceData(DataSet_4_Final, Message, 100);
			Assert::IsTrue(r, Message);
		}

		unsigned char DataSet_5_Initial[25] =
		{
			E, E, E, E, E,
			E, B, B, E, E,
			E, B, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
		};

		unsigned char DataSet_5_Final[25] =
		{
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
		};


		TEST_METHOD(EraseShape_Test_05)
		{
			unsigned char InitialDataSetExpanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_5_Initial, InitialDataSetExpanded);
			////Create level filled with test data
			Level* level = new Level(5, 5, B, InitialDataSetExpanded);
			Contour* externalContour = level->FindExternalContour();
			level->EraseShape(externalContour, nullptr);
			bool r = level->CompareLevelDataWithReferenceData(DataSet_5_Final, Message, 100);
			Assert::IsTrue(r, Message);
		}


		unsigned char DataSet_6_Initial[81] =
		{
			B, B, B, B, B, B, B, B, B,
			B, B, B, B, B, B, B, B, B,
			B, B, B, B, B, E, E, B, B,
			B, B, B, B, E, E, B, B, B,
			B, B, B, E, E, E, B, B, B,
			B, B, B, E, E, B, B, B, B,
			B, B, B, E, E, B, B, B, B,
			B, B, B, B, B, B, B, B, B,
			B, B, B, B, B, B, B, B, B
		};

		unsigned char DataSet_6_Final[81] =
		{
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E
		};

		TEST_METHOD(EraseShape_Test_06)
		{
			unsigned char InitialDataSetExpanded[324];
			Level::ExpandLevelData(9, 9, B, DataSet_6_Initial, InitialDataSetExpanded);
			////Create level filled with test data
			Level* level = new Level(9, 9, B, InitialDataSetExpanded);
			Contour* externalContour = level->FindExternalContour();
			Contour* internalContour = level->FindInternalContour(externalContour);
			level->EraseShape(externalContour, internalContour);
			bool r = level->CompareLevelDataWithReferenceData(DataSet_6_Final, Message, 324);
			Assert::IsTrue(r, Message);
		}

		unsigned char DataSet_7_Initial[272] =
		{
			B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B,
			B, E, E, E, E, E, E, E, E, E, B, B, B, B, B, B, B,
			B, E, E, E, E, E, B, E, E, E, E, B, B, B, B, B, B,
			B, E, E, E, E, B, B, B, E, E, E, E, B, B, B, B, B,
			B, E, E, E, E, B, B, B, B, E, E, E, E, B, B, B, B,
			B, E, E, E, B, B, B, B, B, B, E, E, E, B, B, B, B,
			B, E, E, E, B, B, B, B, B, B, E, E, E, B, B, B, B,
			B, E, E, E, B, B, B, B, B, B, E, E, E, B, B, B, B,
			B, E, E, E, B, B, B, B, B, B, B, E, B, B, B, B, B,
			B, E, E, E, B, B, B, B, B, B, E, E, B, B, B, B, B,
			B, E, E, E, E, B, B, B, B, B, B, B, B, B, B, B, B,
			B, E, E, E, E, B, E, B, B, B, B, B, B, B, B, B, B,
			B, E, E, E, E, E, E, E, E, B, B, B, B, B, B, B, B,
			B, E, E, E, E, E, E, E, E, E, B, B, B, B, B, B, B,
			B, E, E, E, E, E, E, E, E, E, B, B, B, B, B, B, B,
			B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B,
		};

		unsigned char DataSet_7_Final[272] =
		{
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E

		};

		TEST_METHOD(Test_07_EraseShape)
		{
			unsigned char InitialDataSetExpanded[1088];
			Level::ExpandLevelData(17, 16, B, DataSet_7_Initial, InitialDataSetExpanded);
			Level* level = new Level(17, 16, B, InitialDataSetExpanded);
			Contour* externalContour = level->FindExternalContour();
			Contour* internalContour = level->FindInternalContour(externalContour);
			level->EraseShape(externalContour, internalContour);
			bool r = level->CompareLevelDataWithReferenceData(DataSet_7_Final, Message, 324);
			Assert::IsTrue(r, Message);
		}


		unsigned char DataSet_8_Initial[81] =
		{
			B, B, E, E, B, E, E, B, B,
			B, E, B, E, E, E, B, E, B,
			B, E, B, B, B, B, B, E, B,
			B, B, E, E, E, E, B, E, B,
			B, E, E, E, E, E, E, E, B,
			B, E, B, B, B, B, B, E, B,
			B, E, B, E, E, E, B, E, B,
			B, E, B, E, B, E, B, E, B,
			B, B, B, E, B, E, B, B, B,
		};

		unsigned char DataSet_8_Final[81] =
		{
			E, E, E, E, B, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, B, E, E, E, E,
			E, E, E, E, B, E, E, E, E,
		};
		/*
			Test EraseContourContent function
		*/
		TEST_METHOD(EraseShape_Test_08)
		{
			unsigned char InitialDataSetExpanded[324];
			Level::ExpandLevelData(9, 9, B, DataSet_8_Initial, InitialDataSetExpanded);
			Level* level = new Level(9, 9, B, InitialDataSetExpanded);
			Contour* externalContour = level->FindExternalContour();
			level->FillContour(externalContour, 0xFF);
			bool r = level->CompareLevelDataWithReferenceData(DataSet_8_Final, Message, 324);
			Assert::IsTrue(r, Message);
		}

		unsigned char DataSet_9_Initial[81] =
		{
			E, E, E, B, B, B, B, E, E,
			E, E, B, B, E, E, E, B, E,
			E, B, E, E, E, B, E, B, E,
			B, E, E, B, B, B, E, B, E,
			B, E, B, E, E, B, E, E, B,
			B, E, E, E, E, E, B, E, B,
			B, B, B, B, B, E, E, E, B,
			B, B, E, E, B, B, E, E, B,
			B, E, E, E, E, B, B, B, E,
		};

		unsigned char DataSet_9_Final[81] =
		{
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, B, E, E, E,
			E, E, E, B, B, B, E, E, E,
			E, E, B, E, E, B, E, E, E,
			E, E, E, E, E, E, B, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
		};

		/*
			Test RestoreContourContent function
		*/
		TEST_METHOD(Test_09_RestoreContourContent)
		{
			unsigned char InitialDataSetExpanded[324];
			Level::ExpandLevelData(9, 9, B, DataSet_9_Initial, InitialDataSetExpanded);
			Level* level = new Level(9, 9, B, InitialDataSetExpanded);
			Contour* externalContour = level->FindExternalContour();
			Contour* internalContour = level->FindInternalContour(externalContour);
			Assert::IsNotNull(internalContour, L"Internal contour not found.");
			level->FillContour(externalContour, 0xFF);
			level->RestoreContourContent(internalContour);
			bool r = level->CompareLevelDataWithReferenceData(DataSet_9_Final, Message, 324);
			Assert::IsTrue(r, Message);
		}

		unsigned char DataSet_10_Initial[81] =
		{
			B, B, E, E, B, E, E, B, B,
			B, E, B, E, E, E, B, E, B,
			B, E, B, B, B, B, B, E, B,
			B, B, E, E, E, E, B, E, B,
			B, E, E, E, E, E, E, E, B,
			B, E, B, B, B, B, B, E, B,
			B, E, B, E, E, E, B, E, B,
			B, E, B, E, B, E, B, E, B,
			B, B, B, E, B, E, B, B, B,
		};

		unsigned char DataSet_10_Final[81] =
		{
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
		};

		/*
			Test EraseBuffer function
		*/
		TEST_METHOD(EraseShape_Test_10)
		{
			unsigned char InitialDataSetExpanded[324];
			Level::ExpandLevelData(9, 9, B, DataSet_9_Initial, InitialDataSetExpanded);
			Level* level = new Level(9, 9, B, InitialDataSetExpanded);
			level->EraseBuffer();
			
			bool r = level->CompareLevelDataWithReferenceData(DataSet_10_Final, Message, 324);
			Assert::IsTrue(r, Message);
		}

		unsigned char DataSet_11_Initial[81] =
		{
			B, B, B, B, B, B, B, B, B,
			B, E, E, E, E, E, E, E, B,
			B, B, E, B, E, B, B, E, B,
			B, B, E, E, E, E, B, E, B,
			B, E, E, E, E, E, E, E, B,
			B, E, B, B, B, B, B, E, B,
			B, E, B, E, E, E, B, E, B,
			B, E, B, E, B, E, B, E, B,
			B, B, B, E, B, E, B, B, B,
		};

		unsigned char DataSet_11_Final[81] =
		{
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E,
			E, E, E, E, B, E, E, E, E,
			E, E, E, E, B, E, E, E, E,
		};

		TEST_METHOD(Test_11_EraseShape)
		{
			unsigned char InitialDataSetExpanded[324];
			Level::ExpandLevelData(9, 9, B, DataSet_11_Initial, InitialDataSetExpanded);
			Level* level = new Level(9, 9, B, InitialDataSetExpanded);
			Contour* externalContour = level->FindExternalContour();
			level->EraseShape(externalContour, nullptr);
			bool r = level->CompareLevelDataWithReferenceData(DataSet_11_Final, Message, 100);
			Assert::IsTrue(r, Message);
		}

		unsigned char DataSet_12_Initial[231] =
		{ 
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, B, B, B, E, B, B, B, B, B, B, B, B, E, E, E, B, B, B, E,
			E, E, B, B, B, B, B, B, B, B, B, B, B, B, E, E, E, E, B, B, E,
			E, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, E, B, B, E, E,
			E, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, E, B, B, E, E,
			E, B, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, B, B, E, E,
			E, E, B, B, B, E, B, B, B, B, B, B, B, B, E, B, B, B, B, E, E,
			E, E, B, B, E, E, B, B, B, B, B, B, B, E, E, B, B, B, B, E, E,
			E, E, E, E, E, B, B, B, B, E, B, B, B, E, E, B, B, B, B, E, E,
			E, E, E, E, B, B, B, B, E, E, E, B, B, E, E, B, B, B, B, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E
		};

		unsigned char DataSet_12_Final[231] =
		{
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E
		};

		TEST_METHOD(Test_12_EraseShape)
		{
			unsigned char InitialDataSetExpanded[924];
			Level::ExpandLevelData(21, 11, B, DataSet_12_Initial, InitialDataSetExpanded);
			Level* level = new Level(21, 11, B, InitialDataSetExpanded);
			Contour* externalContour = level->FindExternalContour();
			level->EraseShape(externalContour, nullptr);
			bool r = level->CompareLevelDataWithReferenceData(DataSet_12_Final, Message, 100);
			Assert::IsTrue(r, Message);
		}


		unsigned char DataSet_13_Initial[1700] =
		{
/* 01 */ B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B,
/* 02 */ B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B,
/* 03 */ B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B,
/* 04 */ B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, B, B, B,
/* 05 */ B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, B, B, B, B, B, E, E, B, B, B,
/* 06 */ B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, E, B, B, B, E, E, E, B, B, B,
/* 07 */ B, B, B, B, B, B, B, B, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, B, E, E, E, B, B, B,
/* 08 */ B, B, B, B, B, B, B, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, B, E, E, E, B, B, B,
/* 09 */ B, B, B, B, B, B, B, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, E, E, E, B, B, B,
/* 10 */ B, B, B, B, B, B, B, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, E, E, E, B, B, B,

/* 11 */ B, B, B, B, B, B, B, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, E, E, E, B, B, B,
/* 12 */ B, B, B, B, B, B, B, B, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, B, E, E, E, B, B, B,
/* 13 */ B, B, B, B, B, B, B, B, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, B, E, E, E, B, B, B,
/* 14 */ B, B, B, B, B, E, E, E, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, B, B, E, E, E, B, B, B,
/* 15 */ B, B, B, B, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, B, B, B, E, E, B, B, B,
/* 16 */ B, B, B, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, B, B, E, E, B, B, B, B,
/* 17 */ B, B, B, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, B, B, E, E, B, B, B, B,
/* 18 */ B, B, B, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, B, E, E, B, B, B, B,
/* 19 */ B, B, B, B, B, E, E, E, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, E, E, E, E, B, B, B, B,
/* 20 */ B, B, B, B, B, E, E, B, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, B, E, E, E, E, B, B, B, B,

/* 21 */ B, B, B, B, B, B, B, B, E, E, E, E, E, E, E, E, B, E, E, E, E, E, E, E, B, B, E, E, E, E, B, B, B, B,
/* 22 */ B, B, B, B, B, B, B, E, E, E, E, E, E, E, E, B, B, B, E, E, E, E, E, E, B, B, E, E, E, E, B, B, B, B,
/* 23 */ B, B, B, E, B, B, B, E, E, E, E, E, E, E, E, B, B, B, E, E, E, E, E, E, B, B, E, E, E, E, B, B, B, B,
/* 24 */ B, B, B, E, E, B, E, E, E, E, E, E, E, E, B, B, B, E, E, E, E, E, E, B, B, B, E, E, E, E, B, B, B, B,
/* 25 */ B, B, B, E, E, E, E, E, E, E, E, E, E, B, B, B, E, E, E, E, E, E, B, B, B, B, E, E, E, B, B, B, B, B,
/* 26 */ B, B, B, E, E, E, E, E, E, E, E, E, B, B, B, B, E, E, E, E, E, E, B, B, B, E, E, E, E, B, B, B, B, B,
/* 27 */ B, B, B, E, E, E, E, E, E, E, E, B, B, B, B, B, E, E, E, E, E, B, B, B, B, E, E, E, E, B, B, B, B, B,
/* 28 */ B, B, B, E, E, E, E, E, E, E, B, B, B, B, B, B, E, E, E, E, E, B, B, B, E, E, E, E, E, B, B, B, B, B,
/* 29 */ B, B, B, E, E, B, B, B, B, B, B, B, B, B, E, E, E, E, E, E, E, B, B, B, E, E, E, E, B, B, B, B, B, B,
/* 30 */ B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, E, E, E, E, E, E, B, B, E, E, E, E, E, B, B, B, B, B, B,

/* 31 */ B, B, B, B, B, B, B, B, B, B, B, B, E, E, E, E, B, B, B, B, B, B, E, E, E, E, E, E, B, B, B, B, B, B,
/* 32 */ B, B, B, B, B, B, B, B, B, B, B, E, E, E, E, E, B, B, B, B, B, E, E, E, E, E, E, E, B, B, B, B, B, B,
/* 33 */ B, B, B, B, B, B, B, B, B, B, E, E, E, E, E, E, E, B, B, B, B, B, E, E, E, E, E, E, B, B, B, B, B, B,
/* 34 */ B, B, B, B, B, B, B, B, B, E, E, E, E, E, E, E, E, B, B, B, B, B, E, E, E, E, E, B, B, B, B, B, B, B,
/* 35 */ B, B, B, B, B, B, B, B, E, E, E, E, E, E, E, E, E, E, B, B, B, B, B, E, E, E, E, B, B, B, B, B, B, B,
/* 36 */ B, B, B, B, B, B, B, E, E, E, E, E, E, E, E, E, E, E, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B,
/* 37 */ B, B, B, B, B, B, E, E, E, E, E, E, E, E, E, E, E, E, E, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B,
/* 38 */ B, B, B, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, B, E, E, B, B, B, B, B, B, B, B, B, B, B,
/* 39 */ B, B, B, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, B, B, B, B, B, B, B, B, B,
/* 40 */ B, B, B, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, B, B, B, B, B, B, B, B,

/* 41 */ B, B, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, E, E, B, B, B, B, B, B, B, B, B,
/* 42 */ B, B, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, B, B, B, B, B, B, B, B, B, B, B,
/* 43 */ B, B, B, E, B, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, B, B, B, B, B, B, B, B, B, B, B, B,
/* 44 */ B, B, B, B, B, E, E, E, E, B, B, E, E, E, E, E, E, E, E, E, E, E, B, B, B, B, B, B, B, B, B, B, B, B,
/* 45 */ B, B, B, B, B, E, E, E, E, B, B, E, E, E, E, E, E, E, E, E, B, B, B, B, B, B, B, B, B, B, B, B, B, B,
/* 46 */ B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, E, E, B, B, B, B, B, B, B, B, B, B, B, B, B, B,
/* 47 */ B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, E, E, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B,
/* 48 */ B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B,
/* 49 */ B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B,
/* 50 */ B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B, B
		};

		unsigned char DataSet_13_Final[1700] =
		{
			/* 00 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 01 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 02 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 03 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 04 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 05 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 06 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 07 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 08 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 09 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 10 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 11 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 12 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 13 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 14 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 15 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 16 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 17 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 18 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 19 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 20 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 21 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 22 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 23 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 24 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 25 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 26 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 27 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 28 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 29 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 30 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 31 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 32 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 33 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 34 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 35 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 36 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 37 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 38 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 01 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 01 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 01 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 01 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 01 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 01 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 01 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 01 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 01 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 01 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
			/* 01 */ E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
		};


		TEST_METHOD(Test_13_EraseShape)
		{
			unsigned char InitialDataSetExpanded[6800];
			Level::ExpandLevelData(34, 50, B, DataSet_13_Initial, InitialDataSetExpanded);
			Level* level = new Level(34, 50, B, InitialDataSetExpanded);
			Contour* externalContour = level->FindExternalContour();
			Contour* internalContour = level->FindInternalContour(externalContour);
			level->EraseShape(externalContour, internalContour);
			bool r = level->CompareLevelDataWithReferenceData(DataSet_13_Final, Message, 100);
			Assert::IsTrue(r, Message);
		}


	};

}  // namespace ContourHelpersTest