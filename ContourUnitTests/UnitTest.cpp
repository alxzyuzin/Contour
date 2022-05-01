#include "pch.h"
#include "CppUnitTest.h"
#include <..\\ContourExtractorWindowsRuntimeComponent\Contour.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ContourExtractorWindowsRuntimeComponent;
using namespace std;

namespace ContourUnitTests
{
    TEST_CLASS(ContourUnitTests)
    {
    public:
        /// <summary>
        /// Check if function return TRUE for contour with duplicated points
        /// </summary>
        TEST_METHOD(Contour_HasDuplicatedPoints_1)
        {
            Contour* contour = new Contour(ContourExtractorWindowsRuntimeComponent::Contour::ContourType::External);
           Point point_1( 20,30 );
            Point point_2( 20,30 );
            contour->AddPoint(point_1);
            contour->AddPoint(point_2);
            bool d = contour->HasDuplicatedPoints();
            Assert::IsTrue(d, L"");
        }

        /// <summary>
       /// Check if function return FALSE for contour without duplicated points
       /// </summary>
        TEST_METHOD(Contour_HasDuplicatedPoints_2)
        {
            Contour* contour = new Contour(Contour::ContourType::External);
            Point point_1(20,30);
            Point point_2(30,40);
            contour->AddPoint(point_1);
            contour->AddPoint(point_2);

            Assert::IsFalse(contour->HasDuplicatedPoints(), L"");
        }


        Point ContourData_1[8]{ Point(1, 1) ,Point(2, 1) ,Point(3, 1) ,
                               Point(3, 2) ,Point(3, 3) ,Point(2, 3) ,
                               Point(1, 3) ,Point(1, 2) };

        /// <summary>
        /// Поиск ближайшей точки контура справа (точка существует)
        /// </summary>
        TEST_METHOD(Contour_GetNearestContourPointNumber_1)
        {
        
            Contour contour;
            PopulateContour(&contour, ContourData_1, 8);

            Point* point_1 = contour.GetPoint(1);
            Point* point_2 = contour.GetNearestContourPoint(0, Contour::SearchNearestPointDirection::Right);
            
            Assert::IsTrue(*point_1 == *point_2, L"");
        }

        /// <summary>
        /// Поиск ближайшей точки контура справа (точка не существует)
        /// </summary>
        TEST_METHOD(Contour_GetNearestContourPoint_2)
        {

            Contour contour;
            PopulateContour(&contour, ContourData_1, 8);
           
            Point* point_1 = contour.GetPoint(3);
            Point* point_2 = contour.GetNearestContourPoint(3, Contour::SearchNearestPointDirection::Right);

            Assert::IsTrue(nullptr == point_2, L"");
        }



        Point ContourData_2[8]{ Point(1, 1) ,Point(2, 2) ,Point(3, 1) ,
                              Point(2, 2) ,Point(3, 3) ,Point(2, 2) ,
                              Point(1, 3) ,Point(2, 2) };



        TEST_METHOD(Contour_GetNearestContourPoint_3)
        {
            Contour contour;
            PopulateContour(&contour, ContourData_2, 8);

            Point* point_1 = contour.GetPoint(3);
            Point* point_2 = contour.GetNearestContourPoint(3, Contour::SearchNearestPointDirection::Right);

            Assert::IsTrue(*point_1 == *point_2, L"");
        }
       
        TEST_METHOD(Test_std_map)
        {
            std::map<int, int> intmap;
            intmap.insert(make_pair(2, 3));
            int f =    intmap.count(5);
            int g = intmap.count(2);
        }
        
        void PopulateContour(Contour* contour, Point points[], int numberOfPoints)
        {
            for (int i=0; i< numberOfPoints; i++)
                contour->AddPoint(points[i]);
        }
    };
}