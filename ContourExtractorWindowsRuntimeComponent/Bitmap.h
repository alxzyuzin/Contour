/*---------------------------------------------------------------------------------
 * Copyright(c) 2023 Alexandr Ziuzin.
 * e-mail alx.zyuzin@gmail.com
 * This file is part of Contour project.
 *
 * Main class for image manipulation
 *
 ---------------------------------------------------------------------------------*/


#pragma once
#include <robuffer.h>
#include <ppltasks.h>
#include "Level.h"
#include "Color.h"


using namespace Platform;
using namespace Windows::Foundation;


namespace ContourExtractorWindowsRuntimeComponent
{
		
	[Windows::Foundation::Metadata::WebHostHiddenAttribute]

	// This class contain all information using in image convertion process
	
	public ref class ContourBitmap sealed
	{

	public:

		property  Windows::UI::Xaml::Media::Imaging::WriteableBitmap^ ImageData
		{
			Windows::UI::Xaml::Media::Imaging::WriteableBitmap^ get();
			void set(Windows::UI::Xaml::Media::Imaging::WriteableBitmap^ imageDataValue);
		}

		// List of gray colors present in image after converting  
		property Array<byte>^ GrayScaleColorMap
		{
			Array<byte>^ get();
		}

		// List of colors present in image after converting  
		property Array<unsigned int>^ Colors
		{
			Array<unsigned int>^ get();
		}

		// Number of levels in converted image  
		property int LevelsCount
		{
			int get();
		}

	public:
		ContourBitmap();
		ContourBitmap(int width, int height);
		
		void SetSource(Windows::Storage::Streams::IRandomAccessStream^ stream);
		void ConvertToGrayscale(unsigned char levels);
		void ConvertToReducedColors(unsigned char numberOfColors);
		int  ExtractLevels();
		IAsyncOperation<int>^  FindLevelContoursAsync(unsigned int levelColor);
		void OutlineImage();
		void RectifyLevel(unsigned int color, int size);

		void ClearPixelBuffer();
		void SetOriginalImageDataToDisplayBuffer();
		void SetConvertedImageDataToDisplayBuffer();
		void SetLevelDataToDisplayBuffer(unsigned int levelColor);
		void DisplayContours(ContourColors color);
		void Clear();

	private:	//Methods
		//void	DisplayLevelContours(unsigned int color, ContourColors contourColor);
		void	SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		//bool    CompareLevelsByOriginalColor(Level& l1, Level& l2);

	private:	//Members
		int m_Width;				// Ширина изображения в рикселях
		int m_Height;				// Высота изображения в рикселях
		int m_PixelBufferLength;	// Длина буфера изображения в байтах
		
		unsigned char* m_pPixelBuffer;		    // Pointer to WriteableBitmap buffer. Data from this buffer serves as data source for Image object to display
		
		PixelBuffer m_ImageData;
		unsigned int m_uintImageDataLength;
		
		unsigned char* m_pOriginalImageData;	// Pointer to buffer to store original image data loaded from file
		unsigned char* m_pConvertedImageData;	// Pointer to buffer to store converted image data 
		Windows::UI::Xaml::Media::Imaging::WriteableBitmap^ m_Bitmap = nullptr;
		Windows::UI::Xaml::Controls::Page^ m_pMainPage;
		bool m_Initialized = false;

		std::map<unsigned int, Level*> m_Levels;

	};

}
