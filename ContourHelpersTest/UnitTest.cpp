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

	// 5x5 Square with black border and black point inside
	unsigned char DataSet_1[25] =
	{ 
		B, B, B, B, B,
		B, E, E, E, B,
		B, E, B, E, B,
		B, E, E, E, B,
		B, B, B, B, B,
	};

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
		Find first external contour point if parent contour is null
		Analize 5x5 square area filled by white color (0xFE) 
		Look for first shape pixel with color 0xFE
		OK - if pixel found (function FindFirstContourPoint return true)
		*/
        TEST_METHOD(FindFirstExternalContourPoint_Test_1_1)
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
		TEST_METHOD(FindFirstExternalContourPoint_Test_1_2)
		{
			unsigned char WhiteSquareExpanded[100];
			Level::ExpandLevelData(5, 5, W, WhiteSquare, WhiteSquareExpanded);

			Level* level = new Level(5, 5, W, WhiteSquareExpanded);
			Point firstContourPoint = Point(5, 5);
			Point ExpectedPoint = Point(0, 0);
			level->FindFirstExternalContourPoint(firstContourPoint);
			Assert::IsTrue(firstContourPoint == ExpectedPoint);
		}

		///*
		//Find first external contour point if parent contour is NOT null
		//Analize 5x5 square (sample DataSet_2) area filled by white color (0xFE)
		//Look for first point of external contour of shape with color 0x00 (B)
		//inside contour  formed by points defined in SquareBorderContourPoints_5x5.
		//OK - if pixel found (function FindFirstContourPoint return true)
		//*/
		//TEST_METHOD(FindFirstExternaLContourPoint_Test_1_3)
		//{
		//	unsigned char DataSet[100];
		//	// Create level
		//	Level::ExpandLevelData(5, 5, B, DataSet_2, DataSet );
		//	Level* level = new Level(5, 5, B, DataSet);

		//	// Create internal contour
		//	Contour* contour = new Contour();
		//	for (int i = 0; i < 16; i++)
		//		contour->AddPoint(SquareBorderContourPoints_5x5[i]);
		//	level->m_Contours.push_back(contour);
		//	// Execute test			
		//	Point firstPoint = Point(MAXINT, MAXINT);
		//	Point ExpectedPoint = Point(1, 1);
		//	swprintf(Message, 100, L"Found point coords X=%i, Y=%i, expected X=%i, Y=%i",
		//		firstPoint.X, firstPoint.Y, ExpectedPoint.X, ExpectedPoint.Y);
		//	Assert::IsTrue(level->FindFirstExternalContourPoint(contour, firstPoint), Message);
		//}

		///*
		//Find first external contour point if parent contour is NOT null
		//Same as previouse test, and check found point coords.
		//*/
		//TEST_METHOD(FindFirstExternaLContourPoint_Test_1_4)
		//{
		//	unsigned char DataSet[100];
		//	// Create level
		//	Level::ExpandLevelData(5, 5, B, DataSet_2, DataSet);
		//	Level* level = new Level(5, 5, B, DataSet);

		//	// Create internal contour
		//	Contour* contour = new Contour();
		//	for (int i = 0; i < 16; i++)
		//		contour->AddPoint(SquareBorderContourPoints_5x5[i]);
		//	level->m_Contours.push_back(contour);
		//	// Execute test			
		//	Point firstPoint = Point(MAXINT, MAXINT);
		//	Point ExpectedPoint = Point(1, 1);
		//	swprintf(Message, 100, L"Found point coords X=%i, Y=%i, expected X=%i, Y=%i",
		//		firstPoint.X, firstPoint.Y, ExpectedPoint.X, ExpectedPoint.Y);
		//	level->FindFirstExternalContourPoint(contour, firstPoint);
		//	Assert::IsTrue(firstPoint == ExpectedPoint, Message);
		//}

		/*
		Find next pixel if current pixel lays in upper right corner of OneColor area 0xFE
		Parent contour is null
		OK - if expected coords == X = 4 Y = 1
		*/
		TEST_METHOD(FindNextExternalContourPoint_Test_1)
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
		TEST_METHOD(FindExternalContour_Test_0)
		{
			unsigned char EmptySquareExpanded[100];
			Level::ExpandLevelData(5, 5, B, EmptySquare, EmptySquareExpanded);
			
			Level* level = new Level(5, 5, B, EmptySquareExpanded);
			Contour* contour = level->FindExternalContour(nullptr, B);
			Assert::IsNull(contour, L"contour is not null");
		}


		/*
		Find contour of square area filled with white color (0xFE)
		Check contour length
		*/
		TEST_METHOD(FindExternalContour_Test_1)
		{
			unsigned char WhiteSquareExpanded[100];
			Level::ExpandLevelData(5, 5, W, WhiteSquare, WhiteSquareExpanded);
			
			// use this expression to find array size
			//int size = *(&arr + 1) - arr;
			int expectedContourLength = 16;
						
			Level* level = new Level(5, 5, W, WhiteSquareExpanded);
			Contour* contour = level->FindExternalContour(nullptr, W);
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

		/*
		Find contour of square area having black border and black point in center
		Check contour length
		*/
		TEST_METHOD(FindExternalContour_Test_2)
		{
			wchar_t Message[100];
			unsigned char DataSet_1_Expanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_1, DataSet_1_Expanded);

			// use this expression to find array size
			//int size = *(&arr + 1) - arr;
			int expectedContourLength = 16;

			Level* level = new Level(5, 5, B, DataSet_1_Expanded);
			Contour* contour = level->FindExternalContour(nullptr, B);
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

	
	}; // class LevelTest_ExternalContour

	TEST_CLASS(LevelTest_InternalContour)
	{
	private:
		wchar_t Message[100];
	public:
		// Try to find first internal contour point
		TEST_METHOD(FindFirstInternalContourPoint_Test_1)
		{
			unsigned char DataSet_1_Expanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_1, DataSet_1_Expanded);

			Point point = Point(MAXINT, MAXINT);
			//Create level filled with test data
			Level* level = new Level(5, 5, B, DataSet_1_Expanded);
			// Find external contour
			Contour* contour = level->FindExternalContour(nullptr, B);
			Assert::IsNotNull(contour, L"Contour not found.");
			// Find first internal contour point
			bool result  = level->FindFirstInternalContourPoint(contour, point);
			Assert::IsTrue(result);
		}

		// Check if first internal contour point has expected coords
		TEST_METHOD(FindFirstInternalContourPoint_Test_2)
		{
			unsigned char DataSet_1_Expanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_1, DataSet_1_Expanded);

			Point point = Point(MAXINT, MAXINT);
			Point expectedPoint = Point(1, 1);
			//Create level filled with test data
			Level* level = new Level(5, 5, B, DataSet_1_Expanded);
			// Find external contour
			Contour* contour = level->FindExternalContour(nullptr, B);

			bool result = level->FindFirstInternalContourPoint(contour, point);
			Assert::IsTrue(result, L"First point not found.");
			
			Assert::IsTrue(point == expectedPoint,L"Points does not match.");
		}

		// Check if next internal contour point i defined directions 
		// can not be part of contour
		TEST_METHOD(CheckNextInternalContourPoint_Test_1)
		{
			unsigned char DataSetExpanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_2, DataSetExpanded);
			//Create level filled with test data
			Level* level = new Level(5, 5, B, DataSetExpanded);
			// Find external contour
			Contour* contour = level->FindExternalContour(nullptr, B);
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
			Contour* contour = level->FindExternalContour(nullptr, B);

			for (int i = 0; i < 8; i++)
			{
				Point currentPoint = Point(2, 2);
				bool result = level->CheckNextInternalContourPoint(contour, currentPoint, AllDirections[i]);
				swprintf(Message, 100, L"Neibor point in direction %i cannot be internal contour point", i);
				Assert::IsTrue(result, Message);
			}
		}

		Point ContourPoints[8] =
		{
			Point(1,1), Point(2,1), Point(3,1), Point(3,2),
			Point(3,3), Point(2,3), Point(1,3),	Point(1,2)
		};

		// Check function parameter value control
		TEST_METHOD(FindInternalContour_Test_0)
		{
			Assert::ExpectException< std::invalid_argument, Contour*>([]()
				{
					unsigned char DataSetExpanded[100];
					Level::ExpandLevelData(5, 5, B, DataSet_1, DataSetExpanded);
					Level* level = new Level(5, 5, B, DataSetExpanded);
					return level->FindInternalContour(nullptr);
				}, L"Parametr parentContour is null and exception not thrown.");
		}

		// Find internal contour
		TEST_METHOD(FindInternalContour_Test_1)
		{
			unsigned char DataSetExpanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_1, DataSetExpanded);
			//Create level filled with test data
			Level* level = new Level(5, 5, B, DataSetExpanded);
			// Find external contour
			Contour* externalCountour = level->FindExternalContour(nullptr, B);

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
			Contour* contour = level->FindExternalContour(nullptr, B);

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
			Contour* contour = level->FindExternalContour(nullptr, B);

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
			
			// Try to find first internal contour
			TEST_METHOD(FindAllContours_Test_0)
			{
				unsigned char DataSetExpanded[324];
				Level::ExpandLevelData(9, 9, B, DataSet_0, DataSetExpanded);
			//Create level filled with test data
				Level* level = new Level(9, 9, B, DataSetExpanded);

				level->FindAllContours();
				Assert::AreEqual((int)level->m_Contours.size(), 4);
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
		TEST_METHOD(FindRightNearestPoint_Test_0)
		{
			Contour* contour = new Contour(Contour_1, 8);
			Point* startPoint = new Point(2, 2);
			Point* foundPoint = contour->GetRightNearestContourPoint(startPoint);
			Point ExpectedPoint = Point(3, 2);
			swprintf(Message, 100, L"Found point X=%i,Y=%i. Expected point X=%i,Y=%i",
				foundPoint->X, foundPoint->Y,
				ExpectedPoint.X, ExpectedPoint.Y);
			Assert::IsTrue(*foundPoint == ExpectedPoint, Message);
		}

		// Point resides on left outside contour
		TEST_METHOD(FindRightNearestPoint_Test_1)
		{
			Contour* contour = new Contour(Contour_1, 8);
			Point* startPoint = new Point(0, 2);
			Point  ExpectedPoint = Point(1, 2);
			Point* foundPoint = contour->GetRightNearestContourPoint(startPoint);
			swprintf(Message, 100, L"Found point X=%i,Y=%i. Expected point X=%i,Y=%i",
				foundPoint->X, foundPoint->Y,
				ExpectedPoint.X, ExpectedPoint.Y);
			Assert::IsTrue(*foundPoint == ExpectedPoint, Message);
		}

		// Point resides on right outside contour
		TEST_METHOD(FindRightNearestPoint_Test_2)
		{
			Contour* contour = new Contour(Contour_1, 8);
			Point* startPoint = new Point(4, 2);
			Point* foundPoint = contour->GetRightNearestContourPoint(startPoint);
			Assert::IsNull(foundPoint, L"Point not expected");
		}


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
		TEST_METHOD(EraseShape_Test_0)
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

		Point ExternalContour_1[16] =
		{
			Point(0, 0), Point(1, 0), Point(2, 0), Point(3, 0),
			Point(4, 0), Point(4, 1), Point(4, 2), Point(4, 3),
			Point(4, 4), Point(3, 4), Point(2, 4), Point(1, 4),
			Point(0, 4), Point(0, 3), Point(0, 2), Point(0, 1)
		};

		Point InternalContour_1[8] =
		{
			Point(1, 1), Point(2, 1), Point(3, 1), Point(3, 2),
			Point(3, 3), Point(2, 3), Point(1, 3), Point(1, 2)
		};

		TEST_METHOD(EraseShape_Test_1)
		{
			unsigned char InitialDataSetExpanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_1_Initial, InitialDataSetExpanded);
			////Create level filled with test data
			Level* level = new Level(5, 5, B, InitialDataSetExpanded);
			Contour* externalContour = new Contour(ExternalContour_1, 16);
			Contour* internalContour = new Contour(InternalContour_1, 8);
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
			B, B, B, E, E,
		};

		unsigned char DataSet_2_Final[25] =
		{
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
			E, E, E, E, E,
		};

		Point ExternalContour_2[16] =
		{
			Point(0, 0), Point(1, 0), Point(2, 0), Point(3, 0),
			Point(4, 0), Point(4, 1), Point(4, 2), Point(3, 2),
			Point(2, 2), Point(2, 3), Point(2, 4), Point(1, 4),
			Point(0, 4), Point(0, 3), Point(0, 2), Point(0, 1)
		};

		Point InternalContour_2[8] =
		{
			Point(1, 1), Point(2, 1), Point(3, 1), Point(2, 1),
			Point(1, 1), Point(1, 2), Point(1, 3), Point(1, 2)
		};

		TEST_METHOD(EraseShape_Test_2)
		{
			unsigned char InitialDataSetExpanded[324];
			Level::ExpandLevelData(5, 5, B, DataSet_2_Initial, InitialDataSetExpanded);
			////Create level filled with test data
			Level* level = new Level(5, 5, B, InitialDataSetExpanded);
			Contour* externalContour = new Contour(ExternalContour_2, 16);
			Contour* internalContour = new Contour(InternalContour_2, 8);
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

		Point ExternalContour_3[36] =
		{
			Point(0,0), Point(1,0), Point(2,0), Point(3,0), Point(4,0),	Point(5,0),
			Point(6,0), Point(7,0), Point(8,0), Point(8,1), Point(8,2), Point(8,3),
			Point(8,4), Point(8,5), Point(8,6), Point(8,7), Point(8,8), Point(7,8),
			Point(6,8), Point(6,7), Point(6,6), Point(5,5), Point(4,5), Point(3,5),
			Point(2,6),	Point(2,7), Point(2,8), Point(1,8), Point(0,8), Point(0,7),
			Point(0,6),	Point(0,5), Point(0,4),	Point(0,3), Point(0,2),	Point(0,1)
		};

		Point InternalContour_3[28] =
		{
			Point(1,1), Point(2,1), Point(3,1), Point(4,1), Point(5,1),	Point(6,1),
			Point(7,1), Point(7,2), Point(7,3), Point(7,4), Point(7,5),	Point(7,6),
			Point(7,7), Point(7,6), Point(7,5),	Point(6,4), Point(5,4),	Point(4,4),
			Point(3,4), Point(2,4), Point(1,5),	Point(1,6), Point(1,7),	Point(1,6),
			Point(1,5), Point(1,4), Point(2,3),	Point(2,2)
		};

		TEST_METHOD(EraseShape_Test_3)
		{
			unsigned char InitialDataSetExpanded[324];
			Level::ExpandLevelData(9, 9, B, DataSet_3_Initial, InitialDataSetExpanded);
			////Create level filled with test data
			Level* level = new Level(9, 9, B, InitialDataSetExpanded);
			Contour* externalContour = new Contour(ExternalContour_3, 36);
			Contour* internalContour = new Contour(InternalContour_3, 28);
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

		
		TEST_METHOD(EraseShape_Test_4)
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

		Point ExternalContour_5[3] =
		{
			Point(1,1), Point(2,1), Point(1,2)
		};

		

		TEST_METHOD(EraseShape_Test_5)
		{
			unsigned char InitialDataSetExpanded[100];
			Level::ExpandLevelData(5, 5, B, DataSet_5_Initial, InitialDataSetExpanded);
			////Create level filled with test data
			Level* level = new Level(5, 5, B, InitialDataSetExpanded);
			Contour* externalContour = new Contour(ExternalContour_5, 3);
			level->EraseShape(externalContour, nullptr);
			bool r = level->CompareLevelDataWithReferenceData(DataSet_5_Final, Message, 100);
			Assert::IsTrue(r, Message);
		}


	};

}  // namespace ContourHelpersTest