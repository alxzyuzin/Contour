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
		byte m_Color = 0;
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
		void RectifyLevel(byte color, int size);

	private:	//Methods
		void	DisplayLevelShapes(byte color);
		void	DisplayLevelContours(byte color);
		Level*	SelectLevel(byte color);
		void	SortColorMap(std::vector<byte>* colormap);
		void	SetPixel(int x, int y, byte r, byte g, byte b, byte a);
		void	ClearPixelBuffer();

	private:	//Members
		int m_Width;				// Ширина изображения в рикселях
		int m_Height;				// Высота изображения в рикселях
		int m_PixelBufferLength;	// Длина буфера изображения в байтах
		byte* m_pPixelBuffer;		// Указатель на буфер WriteableBitmap. Содержимое этого буфера является источником данных для объекта Image
		byte* m_pOriginalImageData; // Буфер хранит оригинальное изображение загруженное из файла
		WriteableBitmap^ m_ImageData = nullptr;
		Page^ m_pMainPage;
		bool m_Initialized = false;
		vector<Level*> m_Levels;
	};

}
