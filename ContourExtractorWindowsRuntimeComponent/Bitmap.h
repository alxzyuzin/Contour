#pragma once
#include <robuffer.h>
#include <ppltasks.h>
#include "Level.h"

using namespace Platform;

namespace ContourExtractorWindowsRuntimeComponent
{
	public ref class DisplayParams sealed
	{
	public:
		// Цвет к которому относится данный набор параметров
	property byte Color
		{
			byte get();
			void set(byte color);
		}

		// If TRUE display shapes from this level on the output image
		property bool DisplayShapes
		{
			bool get();
			void set(bool displayShapes);
		}

		// If TRUE display contours from this level on the output image
		property bool DisplayContours
		{
			bool get();
			void set(bool displayContours);
		}

	private:
		unsigned char m_Color = 0;
		bool m_DisplayShapes = false;
		bool m_DisplayContours = false;
	};

	[Windows::Foundation::Metadata::WebHostHiddenAttribute]

	// This class contain all information using in image convertion process
	//		List of gray colors after converting original image to grayscale
	//		List of grayscaled image Levels after extracting  
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

	public:
		ContourBitmap();
		ContourBitmap(int width, int height);
		//ContourBitmap(int width, int height, unsigned char* pPixelBuffer);

		void SetSource(Windows::Storage::Streams::IRandomAccessStream^ stream);
		void ConvertToGrayscale(unsigned char levels);
		void ExtractLevels();
		void OutlineImage();
		void RectifyLevel(unsigned char color, int size);
		void DisplayOutlinedImage(const Array<DisplayParams^>^ parameters);
		void RestoreOriginalImage();
		void DisplayContours();
		void Clear();

	private:	//Methods
		void	DisplayLevelShapes(unsigned char color);
		void	DisplayLevelContours(unsigned char color);
		Level*	SelectLevel(unsigned char color);
		void	SortColorMap(std::vector<unsigned char>* colormap);
		void	ClearPixelBuffer();
		void	SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	private:	//Members
		int m_Width;				// Ширина изображения в рикселях
		int m_Height;				// Высота изображения в рикселях
		int m_PixelBufferLength;	// Длина буфера изображения в байтах
		unsigned char* m_pPixelBuffer;		// Указатель на буфер WriteableBitmap. Содержимое этого буфера является источником данных для объекта Image
		unsigned char* m_pOriginalImageData; // Буфер хранит оригинальное изображение загруженное из файла
		Windows::UI::Xaml::Media::Imaging::WriteableBitmap^ m_Bitmap = nullptr;
		Windows::UI::Xaml::Controls::Page^ m_pMainPage;
		bool m_Initialized = false;
		std::vector<Level*> m_Levels;
	};

}
