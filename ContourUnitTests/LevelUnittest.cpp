#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\ContourHelpers\Contour.h"
#include "..\ContourHelpers\Level.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace  ContourHelpers;

//void FindInternalContours(Contour* parentContour, byte shapeColor);
//Contour* FindContour(Contour* parentContour, byte shapeColor);
//bool FindFirstContourPoint(Contour* parentContour, Point& point, byte shapeColor);
//bool FindNextContourPoint(Contour* parentContour, Point& point, Direction direction, byte shapeColor);


namespace ContourUnitTests
{		
	TEST_CLASS(LevelTest)
	{
	private:
		/*
		Квадратная область 5х5 закрашенная цветом 0x254
		*/
		
		unsigned char OneColor[25] = {	0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
										0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
										0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
										0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
										0xFE, 0xFE, 0xFE, 0xFE, 0xFE };
								
	public:
		/*
		Поиск первой точки контура в области закрашенной цветом 0x254
		Цвет искомой точки совпадает с цветом области
		Ожидаемый результат
			Точка с координатами x=0,y=0
		*/
		TEST_METHOD(Test_FindFirstContourPoint_Level_1)
		{
			Level* level = new Level(5, 5, 0xFE, OneColor);
			Point firstContourPoint = Point(5, 5);
			Point ExpectedPoint = Point(0, 0);
			Assert::IsFalse(level->FindFirstContourPoint(nullptr, firstContourPoint, 0xFE));
//			Assert::AreEqual(firstContourPoint, ExpectedPoint);
			
		}

	};
}