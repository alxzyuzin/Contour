#pragma once
#include "ContourTests.h"
#include "pch.h"
#include "CppUnitTest.h"
#include "TestUtils.h"

#include <..\ContourExtractorWindowsRuntimeComponent\Contour.h>
#include <..\ContourExtractorWindowsRuntimeComponent\Bitmap.h>
#include <..\ContourExtractorWindowsRuntimeComponent\Level.h>
//#include <..\ContourExtractorWindowsRuntimeComponent\Color.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace ContourExtractorWindowsRuntimeComponent;


namespace ContourTestVS2019
{

	TEST_CLASS(ContourBitmap_FunctionsParams)
	{
		const unsigned int W = 0xFFFFFFFF;
		const unsigned int B = 0x00000000;

		const unsigned char E = 0xFF;
		const unsigned char P = 0x00;



		unsigned int bt_1_input_data[36] =
		{
			W, W, W, W, W, W,
			W, B, W, W, W, W,
			W, W, W, W, W, W,
			W, W, W, B, B, W,
			W, W, W, B, W, W,
			W, W, W, W, W, W,
		};

		unsigned int bt_1_expected_res[36] =
		{
			W, W, W, W, W, W,
			W, W, W, W, W, W,
			W, W, W, W, W, W,
			W, W, W, B, B, W,
			W, W, W, B, W, W,
			W, W, W, W, W, W,
		};
		
		TEST_METHOD(CleanUpRectangleArea_Test_1)
		{
			ContourBitmap^ bitmap = ref new ContourBitmap(6, 6, bt_1_input_data);
			//bitmap->ClearRectangleArea(0, 0, 3);
			Assert::IsTrue(TestUtils::CompareArays(bt_1_input_data, bt_1_expected_res, 36));
		}

	

	};
	
}