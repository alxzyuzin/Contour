#include "ContourTests.h"
#include "pch.h"
#include "CppUnitTest.h"
#include "TestUtils.h"
#include <..\ContourExtractorWindowsRuntimeComponent\Contour.h>
#include <..\ContourExtractorWindowsRuntimeComponent\Level.h>
#include <..\ContourExtractorWindowsRuntimeComponent\Color.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace  ContourExtractorWindowsRuntimeComponent;


namespace ContourTestVS2019
{

	TEST_CLASS(LevelTest_FunctionsParams)
	{
		unsigned char pixelBuffer[100];
		pair<unsigned int, unsigned char> levelColors = pair<unsigned int, unsigned char>(B, 0x00);
		//PixelBuffer buffer;
		
		const unsigned int W = 0xFFFFFFFF;
		const unsigned char E = 0xFF;
		const unsigned char P = 0x00;

	public:
		unsigned int ct_1_input_data[9] =
		{
			W, W, W, 
			W, W, W, 
			W, W, W 
		};

		unsigned char ct_1_expected_res[9] =
		{
			E, E, E,
			E, E, E,
			E, E, E
		};
		// Check if buffer width less then cleanup size
		TEST_METHOD(CleanUp_Test_1)
		{
			// Create level with all empty points
			// W = 0xFFFFFFFF != 0xFF00FF00 so all points in level buffer will be filled with empty values
			Level* level = new Level(3, 3, levelColors, ct_1_input_data);
			level->ClearArea(0, 0, 5, 0xFF);
			Assert::IsTrue(level->CompareLevelBufferWithReferenceData(ct_1_expected_res));
		}

		unsigned int ct_2_input_data[9] =
		{
			B, W, W,
			W, W, W,
			W, W, W
		};

		unsigned char ct_2_expected_res[9] =
		{
			P, E, E,
			E, E, E,
			E, E, E
		};
		// Check if buffer width less then cleanup size
		TEST_METHOD(CleanUp_Test_2)
		{
			// Create level with all empty points
			// W = 0xFFFFFFFF != 0xFF00FF00 so all points in level buffer will be filled with empty values
			Level* level = new Level(3, 3, levelColors, ct_2_input_data);
			level->ClearArea(0, 0, 5, 0xFF);
			Assert::IsTrue(level->CompareLevelBufferWithReferenceData(ct_2_expected_res));
		}

		unsigned int ct_3_input_data[9] =
		{
			W, W, W,
			W, B, W,
			W, W, W
		};

		unsigned char ct_3_expected_res[9] =
		{
			E, E, E,
			E, E, E,
			E, E, E
		};
		// Check if buffer width less then cleanup size
		TEST_METHOD(CleanUp_Test_3)
		{
			// Create level with all empty points
			// W = 0xFFFFFFFF != 0xFF00FF00 so all points in level buffer will be filled with empty values
			Level* level = new Level(3, 3, levelColors, ct_3_input_data);
			level->ClearArea(0, 0, 3, 0xFF);
			Assert::IsTrue(level->CompareLevelBufferWithReferenceData(ct_3_expected_res));
		}


		unsigned int ct_4_input_data[36] =
		{
			W, W, W, W, W, W,
			W, B, W, B, B, W,
			W, W, B, W, W, W,
			W, W, W, W, W, W,
			W, W, B, W, W, W,
			W, W, W, W, W, W
		};

		unsigned char ct_4_expected_res[36] =
		{
			E, E, E, E, E, E,
			E, E, E, E, E, E,
			E, E, E, E, E, E,
			E, E, E, E, E, E,
			E, E, E, E, E, E,
			E, E, E, E, E, E
		};
		// Check if buffer width less then cleanup size
		TEST_METHOD(CleanUp_Test_4)
		{
			// Create level with all empty points
			// W = 0xFFFFFFFF != 0xFF00FF00 so all points in level buffer will be filled with empty values
			Level* level = new Level(6, 6, levelColors, ct_4_input_data);
			level->ClearArea(0, 0, 6, 0xFF);
			Assert::IsTrue(level->CompareLevelBufferWithReferenceData(ct_4_expected_res));
		}

		unsigned int ct_5_input_data[36] =
		{
			W, W, W, W, W, W,
			W, B, W, B, B, W,
			W, W, B, W, W, W,
			W, W, W, W, W, W,
			W, W, B, W, W, W,
			W, W, W, W, W, W
		};

		unsigned char ct_5_expected_res[36] =
		{
			E, E, E, E, E, E,
			E, P, E, P, P, E,
			E, E, P, E, E, E,
			E, E, E, E, E, E,
			E, E, P, E, E, E,
			E, E, E, E, E, E
		};

		TEST_METHOD(CleanUp_Test_5)
		{
			// Create level with all empty points
			// W = 0xFFFFFFFF != 0xFF00FF00 so all points in level buffer will be filled with empty values
			Level* level = new Level(6, 6, levelColors, ct_5_input_data);
			level->ClearArea(0, 0, 3, 0xFF);
			Assert::IsTrue(level->CompareLevelBufferWithReferenceData(ct_5_expected_res));
		}


		unsigned int ct_6_input_data[36] =
		{
			W, W, W, W, W, W,
			W, B, W, B, B, W,
			W, W, B, W, W, W,
			W, W, W, W, W, W,
			W, W, B, W, W, W,
			W, W, W, W, W, W
		};

		unsigned char ct_6_expected_res[36] =
		{
			E, E, E, E, E, E,
			E, P, E, P, P, E,
			E, E, P, E, E, E,
			E, E, E, E, E, E,
			E, E, E, E, E, E,
			E, E, E, E, E, E
		};
		TEST_METHOD(CleanUp_Test_6)
		{
			Level* level = new Level(6, 6, levelColors, ct_6_input_data);
			level->ClearArea(1, 3, 3, 0xFF);
			Assert::IsTrue(level->CompareLevelBufferWithReferenceData(ct_6_expected_res));
		}


		unsigned int ct_7_input_data[36] =
		{
			W, W, W, W, W, W,
			W, B, W, W, B, W,
			W, W, B, W, W, W,
			W, W, W, W, B, W,
			W, W, B, W, W, W,
			W, W, W, W, W, W
		};

		unsigned char ct_7_expected_res[36] =
		{
			E, E, E, E, E, E,
			E, P, E, E, E, E,
			E, E, P, E, E, E,
			E, E, E, E, P, E,
			E, E, P, E, E, E,
			E, E, E, E, E, E
		};
		TEST_METHOD(CleanUp_Test_7)
		{
			Level* level = new Level(6, 6, levelColors, ct_7_input_data);
			level->ClearArea(3, 0, 3, 0xFF);
			Assert::IsTrue(level->CompareLevelBufferWithReferenceData(ct_7_expected_res));
		}


		unsigned int ct_8_input_data[36] =
		{
			W, W, W, W, W, W,
			W, B, B, B, B, W,
			W, B, W, B, W, W,
			W, B, B, B, B, W,
			W, W, B, W, W, W,
			W, W, W, W, W, W
		};

		unsigned char ct_8_expected_res[36] =
		{
			E, E, E, E, E, E,
			E, P, P, P, P, E,
			E, P, P, P, E, E,
			E, P, P, P, P, E,
			E, E, P, E, E, E,
			E, E, E, E, E, E
		};
		TEST_METHOD(CleanUp_Test_8)
		{
			Level* level = new Level(6, 6, levelColors, ct_8_input_data);
			level->ClearArea(1, 1, 3, 0x00);
			Assert::IsTrue(level->CompareLevelBufferWithReferenceData(ct_8_expected_res));
		}

		unsigned int ct_9_input_data[36] =
		{
			W, W, W, W, W, W,
			W, B, W, W, B, W,
			W, W, B, W, W, W,
			W, W, W, W, B, W,
			W, W, B, W, W, W,
			W, W, W, W, W, W
		};

		unsigned char ct_9_expected_res[36] =
		{
			E, E, E, E, E, E,
			E, P, E, E, E, E,
			E, E, P, E, E, E,
			E, E, E, E, E, E,
			E, E, E, E, E, E,
			E, E, E, E, E, E
		};
		TEST_METHOD(CleanUp_Test_9)
		{
			Level* level = new Level(6, 6, levelColors, ct_9_input_data);
			level->Rectify(3);
			Assert::IsTrue(level->CompareLevelBufferWithReferenceData(ct_9_expected_res));
		}

		unsigned int ct_10_input_data[36] =
		{
			W, W, W, W, W, W,
			W, B, B, B, B, W,
			W, B, W, B, W, W,
			W, B, B, B, B, W,
			W, W, B, W, W, W,
			W, W, W, W, W, W
		};

		unsigned char ct_10_expected_res[36] =
		{
			E, E, E, E, E, E,
			E, P, P, P, P, E,
			E, P, P, P, E, E,
			E, P, P, P, P, E,
			E, E, P, E, E, E,
			E, E, E, E, E, E
		};
		TEST_METHOD(CleanUp_Test_10)
		{
			Level* level = new Level(6, 6, levelColors, ct_10_input_data);
			level->Rectify(3);
			Assert::IsTrue(level->CompareLevelBufferWithReferenceData(ct_10_expected_res));
		}

	};
}