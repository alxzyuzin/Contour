#include "pch.h"
#include "CppUnitTest.h"
#include <..\\ContourExtractorWindowsRuntimeComponent\Contour.h>
#include <..\\ContourExtractorWindowsRuntimeComponent\MapEntry.h>
#include "../../../../../../Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/winrt/wrl/client.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ContourExtractorWindowsRuntimeComponent;
using namespace std;

namespace ContourUnitTests
{
    TEST_CLASS(ContourUnitTests)
    {
    private:
        Point Contour_1[8] =
        {
            Point(1,1), Point(2,1), Point(3,1),
                                    Point(3,2),
            Point(3,3), Point(2,3), Point(1,3),
            Point(1,2)
        };

    public:
        TEST_METHOD(Contour_AddPoint_1)
        {
            Contour contour;
            PopulateContour_1(&contour);
            Assert::AreEqual(contour.Size(), 8);
        }

        TEST_METHOD(Contour_AddPoint_2)
        {
            Contour contour;
            PopulateContour_1(&contour);
            int* i = contour.GetPointNumbersListByCoords(3, 2);
            Assert::AreEqual(*i, 3);
        }

        TEST_METHOD(Contour_PointLaysOnContour_1)
        {
            Contour contour;
            PopulateContour_1(&contour);
            bool p = contour.PointLaysOnContour(3, 1);
            Assert::IsTrue(p);
        }

        TEST_METHOD(Contour_PointLaysOnContour_2)
        {
            Contour contour;
            PopulateContour_1(&contour);
            bool p = contour.PointLaysOnContour(2, 2);
            Assert::IsFalse(p);
        }

        TEST_METHOD(Contour_FindLeftNearestPoint_1)
        {
            Contour contour;
            PopulateContour_1(&contour);
            Point* p = contour.FindLeftNearestPoint(3);
            Assert::IsTrue(*p == Point(1,2));
        }
        
        TEST_METHOD(Contour_FindLeftNearestPoint_2)
        {
            Contour contour;
            PopulateContour_1(&contour);
            Point* p = contour.FindLeftNearestPoint(7);
            Assert::IsNull(p);
        }

        TEST_METHOD(Contour_FindRightNearestPoint_1)
        {
            Contour contour;
            PopulateContour_1(&contour);
            Point* p = contour.FindRightNearestPoint(3);
            Assert::IsNull(p);
        }



        TEST_METHOD(Contour_FindRightNearestPoint_2)
        {
            Contour contour;
            PopulateContour_1(&contour);
            Point* p = contour.FindRightNearestPoint(7);
            Assert::IsTrue(*p == Point(3, 2));
        }

        // Check function ContainsPoint return false if point resides left from contour
         // Expected result true
        TEST_METHOD(Contour_ContainsPoint_3)
        {
            Contour contour;
            PopulateContour_1(&contour); 
            Assert::IsFalse(contour.ContainsPoint(0, 1));
        }

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


        /// <summary>
        /// Поиск ближайшей точки контура справа (точка существует)
        /// </summary>
        TEST_METHOD(Contour_GetNearestContourPoint_1)
        {
        
            Contour contour;
            PopulateContour_1(&contour);
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
            PopulateContour_1(&contour);
            Point* point_1 = contour.GetPoint(3);
            Point* point_2 = contour.GetNearestContourPoint(3, Contour::SearchNearestPointDirection::Right);
            Assert::IsTrue(nullptr == point_2, L"");
        }


        TEST_METHOD(Contour_GetNearestContourPoint_3)
        {
            Contour contour;
            PopulateContour_2(&contour);
            Point* point_1 = contour.GetPoint(3);
            Point* point_2 = contour.GetNearestContourPoint(3, Contour::SearchNearestPointDirection::Right);
            Assert::IsTrue(*point_1 == *point_2, L"");
        }
       
               
        TEST_METHOD(Contour_ContainsPoint_1)
        {
            Contour contour;
            PopulateContour_3(&contour);
            Assert::IsFalse(contour.HasPoint(0, 0), L"");
        }

        TEST_METHOD(Contour_ContainsPoint_2)
        {
            Contour contour;
            PopulateContour_3(&contour);
            Assert::IsTrue(contour.ContainsPoint(2, 2), L"");
        }
        //=======================================================================

        //=======================================================================
        void PopulateContour_1(Contour* contour)
        {
            contour->AddPoint(Point(1, 1));
            contour->AddPoint(Point(2, 1));
            contour->AddPoint(Point(3, 1));
            contour->AddPoint(Point(3, 2));
            contour->AddPoint(Point(3, 3));
            contour->AddPoint(Point(2, 3));
            contour->AddPoint(Point(1, 3));
            contour->AddPoint(Point(1, 2));
        }

        void PopulateContour_2(Contour* contour)
        {
            contour->AddPoint(Point(1, 1));
            contour->AddPoint(Point(2, 2));
            contour->AddPoint(Point(3, 1));
            contour->AddPoint(Point(2, 2));
            contour->AddPoint(Point(3, 3));
            contour->AddPoint(Point(2, 2));
            contour->AddPoint(Point(1, 3));
            contour->AddPoint(Point(2, 2));
        }


       
        void PopulateContour_3(Contour* contour)
        {
            contour->AddPoint(Point(1, 1));
            contour->AddPoint(Point(2, 1));
            contour->AddPoint(Point(3, 1));
            contour->AddPoint(Point(4, 2));
            contour->AddPoint(Point(5, 2));
            contour->AddPoint(Point(6, 1));
            contour->AddPoint(Point(7, 1));
            contour->AddPoint(Point(8, 1));
            contour->AddPoint(Point(8, 2));
            contour->AddPoint(Point(8, 3));
            contour->AddPoint(Point(7, 3));
            contour->AddPoint(Point(7, 4));
            contour->AddPoint(Point(8, 5));
            contour->AddPoint(Point(8, 6));
            contour->AddPoint(Point(8, 7));
            contour->AddPoint(Point(7, 7));
            contour->AddPoint(Point(6, 6));
            contour->AddPoint(Point(5, 6));
            contour->AddPoint(Point(4, 6));
            contour->AddPoint(Point(3, 7));
            contour->AddPoint(Point(2, 7));
            contour->AddPoint(Point(1, 7));
            contour->AddPoint(Point(1, 6));
            contour->AddPoint(Point(2, 5));
            contour->AddPoint(Point(2, 4));
            contour->AddPoint(Point(1, 3));
            contour->AddPoint(Point(1, 2));
        }
    };

   
}