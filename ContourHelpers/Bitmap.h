#pragma once
#include <robuffer.h>
#include <ppltasks.h>
#include "Level.h"

using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Controls;
using namespace Platform;

namespace ContourHelpers
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

	public ref class Bitmap sealed
	{

	public:

		property WriteableBitmap^ ImageData
		{
			WriteableBitmap^ get();
			void set(WriteableBitmap^ imageDataValue);
		}
		property bool Initialized
		{
			bool get();
		}
		property Array<byte>^ GrayScaleColorMap
		{
			Array<byte>^ get();
		}

	public:
		Bitmap();
		Bitmap(Page^ page, int width, int height);

		void SetSource(IRandomAccessStream^ stream);
		IAsyncAction^ SetSourceAsync(IRandomAccessStream^ stream);
		void ConvertToGrayscale(byte levels);
		void RestoreOriginalImage();
		void Clear();
		void DisplayOutlinedImage(const Array<DisplayParams^>^ parameters);

		void ExtractLevels();
		void OutlineImage();
		void RectifyLevel(unsigned char color, int size);

	private:	//Methods
		void	DisplayLevelShapes(unsigned char color);
		void	DisplayLevelContours(unsigned char color);
		Level*	SelectLevel(unsigned char color);
		void	SortColorMap(std::vector<unsigned char>* colormap);
		void	SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		void	ClearPixelBuffer();

	private:	//Members
		int m_Width;				// Ширина изображения в рикселях
		int m_Height;				// Высота изображения в рикселях
		int m_PixelBufferLength;	// Длина буфера изображения в байтах
		unsigned char* m_pPixelBuffer;		// Указатель на буфер WriteableBitmap. Содержимое этого буфера является источником данных для объекта Image
		unsigned char* m_pOriginalImageData; // Буфер хранит оригинальное изображение загруженное из файла
		WriteableBitmap^ m_ImageData = nullptr;
		Page^ m_pMainPage;
		bool m_Initialized = false;
		vector<Level*> m_Levels;
	};

}
