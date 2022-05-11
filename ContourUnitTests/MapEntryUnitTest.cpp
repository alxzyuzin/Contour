#include "pch.h"
#include "MapEntryUnitTest.h"
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

    TEST_CLASS(MapEntryUnitTests)
    {
        TEST_METHOD(MapEntry_AddPoint)
        {
            MapEntry me;
            me.AddPointNumber(100);
            Assert::IsTrue(me.Size() == 1, L"");
        }

        TEST_METHOD(MapEntry_ForLoop_PointsExist)
        {
            MapEntry me;
            me.AddPointNumber(100);
            me.AddPointNumber(200);
            me.AddPointNumber(300);
            int k = 0;
            for (int pointnumber : me)
                k += pointnumber;
            Assert::AreEqual(k, 600, L"");
        }

        TEST_METHOD(MapEntry_ForLoop_NoPoints)
        {
            MapEntry me;
           
            int k = 0;
            for (int pointnumber : me)
                k += pointnumber;
            Assert::AreEqual(k, 0, L"");
        }

        TEST_METHOD(MapEntry_ForLoop_TestIndexer)
        {
            MapEntry me;
            me.AddPointNumber(100);
            me.AddPointNumber(200);
            me.AddPointNumber(300);

            Assert::AreEqual(me[1], 200, L"");
        }
    };

}
