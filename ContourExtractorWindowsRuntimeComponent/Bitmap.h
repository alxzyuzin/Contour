#pragma once
#include <robuffer.h>
#include <ppltasks.h>
//#include "Level.h"

//using namespace Windows::UI::Xaml::Media::Imaging;
//using namespace Windows::Storage;
//using namespace Windows::Storage::Streams;
//using namespace Windows::Foundation;
//using namespace Windows::UI::Xaml::Controls;
using namespace Platform;

namespace ContourExtractorWindowsRuntimeComponent
{
	public ref class DisplayParams sealed
	{
	public:
		property byte Color
		{
			byte get();
			void set(byte color);
		}

		property bool DisplayShapes
		{
			bool get();
			void set(bool displayShapes);
		}

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

	public ref class ContourBitmap sealed
	{

	public:

		property  Windows::UI::Xaml::Media::Imaging::WriteableBitmap^ ImageData
		{
			Windows::UI::Xaml::Media::Imaging::WriteableBitmap^ get();
			void set(Windows::UI::Xaml::Media::Imaging::WriteableBitmap^ imageDataValue);
		}

		property Array<byte>^ GrayScaleColorMap
		{
			Array<byte>^ get();
		}

	public:
		ContourBitmap();
		ContourBitmap(int width, int height);
		ContourBitmap(Windows::UI::Xaml::Controls::Page^ page, int width, int height);

		void SetSource(Windows::Storage::Streams::IRandomAccessStream^ stream);
		/// IAsyncAction^ SetSourceAsync(IRandomAccessStream^ stream);
		void ConvertToGrayscale(byte levels);
		void ExtractLevels();
		void OutlineImage();
		void RectifyLevel(unsigned char color, int size);
		void DisplayOutlinedImage(const Array<DisplayParams^>^ parameters);
		void RestoreOriginalImage();
		void Clear();

	private:	//Methods
		void	DisplayLevelShapes(unsigned char color);
		void	DisplayLevelContours(unsigned char color);
//		Level*	SelectLevel(unsigned char color);
		void	SortColorMap(std::vector<unsigned char>* colormap);
		void	ClearPixelBuffer();
		void	SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	private:	//Members
		int m_Width;				// Ширина изображения в рикселях
		int m_Height;				// Высота изображения в рикселях
		int m_PixelBufferLength;	// Длина буфера изображения в байтах
		unsigned char* m_pPixelBuffer;		// Указатель на буфер WriteableBitmap. Содержимое этого буфера является источником данных для объекта Image
		unsigned char* m_pOriginalImageData; // Буфер хранит оригинальное изображение загруженное из файла
		Windows::UI::Xaml::Media::Imaging::WriteableBitmap^ m_ImageData = nullptr;
		Windows::UI::Xaml::Controls::Page^ m_pMainPage;
		bool m_Initialized = false;
//		std::vector<Level*> m_Levels;
	};

}
