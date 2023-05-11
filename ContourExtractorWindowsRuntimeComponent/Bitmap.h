/*---------------------------------------------------------------------------------
 * Copyright(c) 2023 Alexandr Ziuzin.
 * e-mail alx.zyuzin@gmail.com
 * 
 * This file is part of Contour project.
 *
 * Main class for image manipulation
 *
 ---------------------------------------------------------------------------------*/


#pragma once
#include <robuffer.h>
#include <ppltasks.h>
#include <ppl.h>
#include "Level.h"
#include "Color.h"

using namespace concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Media::Imaging;


namespace ContourExtractorWindowsRuntimeComponent
{
		
	[Windows::Foundation::Metadata::WebHostHiddenAttribute]

	// This class contain all information using in image convertion process
	
	public ref class ContourBitmap sealed
	{

	public:

		property  WriteableBitmap^ ImageData { WriteableBitmap^ get(); }

		property  unsigned int Width { unsigned int get(); }

		property  unsigned int Height { unsigned int get(); }

		// List of gray colors present in image after converting  
		property Array<byte>^ GrayScaleColorMap { Array<byte>^ get(); }

		// List of colors present in image after converting  
		property Array<unsigned int>^ Colors { 	Array<unsigned int>^ get(); }

		// Number of levels in converted image  
		property int LevelsCount { int get(); }


	public:
		ContourBitmap();
		ContourBitmap(int width, int height);
		
		void Invalidate();
		void SetSource(Windows::Storage::Streams::IRandomAccessStream^ stream);
		void CancelOperation();
		IAsyncOperationWithProgress<int, double>^ ExtractLevelsAsync(int numcolors);
		IAsyncActionWithProgress<double>^ ConvertToGrayscaleAsync(unsigned int numberOfColors);
		IAsyncActionWithProgress<double>^ ConvertToReducedColorsAsync(unsigned int numberOfColors);
		IAsyncActionWithProgress<double>^ CleanUpImageAsync(int size);
		IAsyncActionWithProgress<double>^ CleanUpLevelsAsync(int size);
		IAsyncActionWithProgress<double>^ OutlineImageAsync();
		
		void RotateLeft();
		void RotateRight();

		
		void RectifyLevel(unsigned int color, int size);

		void ClearPixelBuffer();
		void SetOriginalImageDataToDisplayBuffer();
		void SetConvertedImageDataToDisplayBuffer();
		void SetLevelDataToDisplayBuffer(unsigned int levelColor);
		void DisplayContours(ContourColors color, int minContourLength, unsigned char contourDensity);
		void Clear();

	private:
		unsigned int* GetPointerToWriteableBitmapPixelData(WriteableBitmap^ bitmap);
		void SaveOriginalImageData();
		void SaveConvertedImageData();
		void RestoreOriginalImageData();
		void RestoreConvertedImageData();
		void ClearRectangleArea(int x, int y, int size);
		void DeleteAllLevels();

	private:	//Members
		int m_Width;					// Image width in pixels
		int m_Height;					// Image width in pixels
	
		int m_bytePixelBufferLength;	// Pixel buffer length in bytes
		int m_uintPixelBufferLength;	// Pixel buffer length in unsigned int
		unsigned int* m_PixelBuffer;    // Pointer to WriteableBitmap buffer. Data from this buffer serves as data source for Image object to display
		
		unsigned char* m_pOriginalImageData;	// Pointer to buffer to store original image data loaded from file
		unsigned char* m_pConvertedImageData;	// Pointer to buffer to store converted image data 
		WriteableBitmap^ m_Bitmap = nullptr;
		WriteableBitmap^ m_BufferBitmap = nullptr;

		std::map<unsigned int, Level*> m_Levels;

		cancellation_token_source* m_CancellationTokenSource;
	};

}
