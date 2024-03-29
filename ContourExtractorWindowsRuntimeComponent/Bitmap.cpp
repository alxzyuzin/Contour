﻿#include "pch.h"
#include "Bitmap.h"
#include <ppltasks.h>

using namespace std;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::Storage::Streams;
using namespace Platform;

using namespace ContourExtractorWindowsRuntimeComponent;

//=============================================================================
// Свойства
//=============================================================================
WriteableBitmap^	ContourBitmap::ImageData::get() { return m_Bitmap; }
void				ContourBitmap::ImageData::set(WriteableBitmap^ imageDataValue) { m_Bitmap = imageDataValue; }

// Массив оттенков серого содержащихся в изображении
Array<unsigned char>^ ContourBitmap::GrayScaleColorMap::get()
{
	Array<unsigned char>^ grayScaleColorMap = ref new Array<unsigned char>((int)m_Levels.size());
	for (int i = 0; i < (int)m_Levels.size(); i++)
		grayScaleColorMap->set(i, m_Levels[i]->m_Color);
	return grayScaleColorMap;
}


//-----------------------------------------------------------------------------
// public members
//-----------------------------------------------------------------------------

ContourBitmap::ContourBitmap()
{
	m_Width = 0;
	m_Height = 0;
	m_PixelBufferLength = 0;
}

/*
  Input parameters
	width  - image width
	height - image height
*/
ContourBitmap::ContourBitmap(int width, int height)
{
	m_Width = width;   
	m_Height = height;
	m_PixelBufferLength = 4 * width * height;
	m_Bitmap = ref new WriteableBitmap(width, height);
	m_pOriginalImageData = new unsigned char[m_PixelBufferLength];  // Copy of original image data
}

//ContourBitmap::ContourBitmap(int width, int height,unsigned char* pPixelBuffer)
//{
//	m_Width = width;
//	m_Height = height;
//	m_PixelBufferLength = 4 * width * height;
//	m_pPixelBuffer = pPixelBuffer;
//	m_pOriginalImageData = new unsigned char[m_PixelBufferLength];  // Copy of original image data
//	memcpy(m_pOriginalImageData, m_pPixelBuffer, m_PixelBufferLength);
//}

// Функция получает указатель нв внутренний буфер WritableBitmap
// В дальнейшем все манипуляции с изображением выполняются непосредственно
// во внутреннем буфере WritableBitmap
void ContourBitmap::SetSource(IRandomAccessStream^ stream)
{
	m_Bitmap->SetSource(stream);  // m_Bitmap - WritableBitmap^

	using namespace Microsoft::WRL;

	// Obtain access to image data
	ComPtr<IBufferByteAccess> pBufferByteAccess;
	IBuffer^ pIBuffer = m_Bitmap->PixelBuffer;
	ComPtr<IUnknown> pBuffer((IUnknown*)pIBuffer);
	pBuffer.As(&pBufferByteAccess);
	// Get pointer to pixel bytes
	pBufferByteAccess->Buffer(&m_pPixelBuffer); // Теперь m_pPixelBuffer содержит адрес даных изображения во внутреннем буфере WritableBitmap
	// До начала всех менипуляций сделаем копию данных изображения в m_pOriginalImageData
	memcpy(m_pOriginalImageData, m_pPixelBuffer, m_PixelBufferLength);
}

/*
	Convert original color image into grayscale with defined number of gray tints
	Input parameters
		levels - number of desirable gray tints
*/
void ContourBitmap::ConvertToGrayscale(unsigned char levels)
{
	// Убедимся что входной параметр нажодится в допустимом диапазоне
	if (levels < 0 || levels > 255) throw ref new InvalidArgumentException();

	int range = 255 / levels;

	for (int j = 0; j < m_Height; j++)
	{
		for (int i = 0; i < (m_Width * 4); i += 4)
		{
			int pos = j * (m_Width * 4) + (i);
			unsigned char pixelColor = (((m_pPixelBuffer[pos] + m_pPixelBuffer[pos + 1] + m_pPixelBuffer[pos + 2]) / 3) / range)*range;
			// Зарезервируем цвет 0xFF для пикселей фона
			// All pixels with color 0xFF in source image convert into color 0xFE
			// Далее при построении контуров в слоях цвет 0xFF будет означать отсутствие в этом месте пикселя
			pixelColor = pixelColor == 0xFF ? 0xFE : pixelColor;
			m_pPixelBuffer[pos] = pixelColor;
			m_pPixelBuffer[pos + 1] = pixelColor;
			m_pPixelBuffer[pos + 2] = pixelColor;
			m_pPixelBuffer[pos + 3] = 0xFF;
		}
	}
}

/*
	Разбивает изображение на слои. Каждый слой содержит пиксели одного из оттенков
	серого присутствующих в изображении.
	Для каждого оттенка серого в исходном изображении создаётся отдельный слой (Level)
	Размер слоя соответствует размеру исходного изображения.
	В слой переносятся пиксели оттенка серого для котороко создан слой,
	остальные пиксели в слое закрашиваются цветом 0xFF (сооьветствует пустому цвету)
*/
void ContourBitmap::ExtractLevels()
{
	vector<unsigned char> colormap;
	//// Сформируем список оттенков серого присутствующих в изображении
	for (int j = 0; j < m_Height; j++)
	{
		for (int i = 0; i < (m_Width * 4); i += 4)
		{
			int pos = j * (m_Width * 4) + i;
			// Сравниваем толко один байт поскольку при преобразовании в оттенки серого
			// все байты одного пикселя принимают одно значение
			byte pixelColor = m_pPixelBuffer[pos]; 
			bool newColor = true;
			for (byte b : colormap)
			{
				if (b == pixelColor)
				{
					newColor = false;
					break;
				}
			}
			if (newColor)
				colormap.push_back(pixelColor);
		}
	}
	//// Можно ускорить
	//// При формировании списка цветов создать буфер для каждого цвета
	//// и поместить в массив длиной 255 элементов
	////	индекс в массиве равен цвету пикселей помещаемых в буфер
	//// Затем разнести пиксели по буферам за один просмотр

	//// Вынесем каждый оттенок в отдельный буфер
	// Переменная m_Levels Содержит список уровней (экземпляров класса Level). Каждый уровень представляет
	// собой срез изображения по одному из оттенков серого
	// Данные хранятся в массиве байтов. Каждому пкселю исходного изображения  в Level соответствует один байт. 
	// Конструктор класса Level извлекает пиксели с цветом заданным параметром levelColor из буфера m_pPixelBuffer
	// и создаёт срез исходного изображения по цвету levelColor
	for (unsigned char levelColor : colormap)
		m_Levels.push_back(new Level(m_Width, m_Height, levelColor, m_pPixelBuffer)); // std::vector<Level*>
	return;
}

void ContourBitmap::RestoreOriginalImage()
{
	memcpy(m_pPixelBuffer, m_pOriginalImageData, m_PixelBufferLength);
}

void ContourBitmap::DisplayContours()
{
	for (Level* level : m_Levels)
		DisplayLevelContours(level->m_Color);
	

}

void ContourBitmap::Clear()
{
	for (auto level : m_Levels)
	{
		level->Clear();
		delete level;
	}
	//delete m_pPixelBuffer;
	delete[] m_pOriginalImageData;
	delete m_Bitmap;
	m_Width = 0;				// Ширина изображения в рикселях
	m_Height = 0;				// Высота изображения в рикселях
	m_PixelBufferLength = 0;	// Длина буфера изображения в байтах
	m_Initialized = false;
}

void ContourBitmap::DisplayOutlinedImage(const Array<DisplayParams^>^ parameters)
{
	ClearPixelBuffer();
	for (unsigned int i = 0; i < parameters->Length; i++)
	{
		if (parameters[i]->DisplayShapes)
			DisplayLevelShapes(parameters[i]->Color);

		if (parameters[i]->DisplayContours)
			DisplayLevelContours(parameters[i]->Color);
	}
}

void ContourBitmap::OutlineImage()
{
	clock_t start = clock();

	/*for (Level* level : m_Levels)
		while (true)
		{
			Contour* externalContour = level->FindExternalContour();
			if (!externalContour)
				break;
			level->m_Contours.push_back(externalContour);
			Contour* internalContour = level->FindInternalContour(externalContour);
			if (internalContour)
				level->m_Contours.push_back(internalContour);
			level->EraseShape(externalContour, internalContour);
		}*/

	for (Level* level : m_Levels)
		level->FindAllContours();
	// time содержит время выполнения функции в милисекундах 
	double time = (clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
	// Изображение разделено на 8 слоёв. Исходное время построения контуров 58818 ms (760 контуров)
	// Изображение разделено на 2 слоя. Исходное время построения контуров  5774 (4587) ms (40 контуров)
	// Изображение разделено на 4 слоя. Исходное время построения контуров 409 818 ms (760 контуров)
}

void ContourBitmap::RectifyLevel(unsigned char color, int size)
{
	Level* selectedLevel = SelectLevel(color);
	selectedLevel->Rectify(size);
}

//-----------------------------------------------------------------------------
// private members
//-----------------------------------------------------------------------------


/*
	 Переносит данные одного слоя в буфер для отображения на экране
*/
void ContourBitmap::DisplayLevelShapes(unsigned char color)
{
	Level* selectedLevel = SelectLevel(color);
	if (!selectedLevel) return;
	selectedLevel->GetLevelShapes(m_pPixelBuffer);
}

void ContourBitmap::DisplayLevelContours(unsigned char color)
{
	Level* selectedLevel = SelectLevel(color);
	Point* point;

	for (Contour* contour : selectedLevel->m_Contours)
		for (int i = 0; i < contour->Size(); i++)
		{
			point = contour->GetPoint(i);
			if (contour->Type == Contour::ContourType::External)
				SetPixel(point->X, point->Y, 0x00, 0xFF, 0x00, 0xFF);
			else
				SetPixel(point->X, point->Y, 0x00, 0x00, 0xFF, 0xFF);
		}
}


// Возвращает указатель на уровень заданного цвета
Level* ContourBitmap::SelectLevel(unsigned char color)
{
	for (Level* level : m_Levels)
	{
		if (level->m_Color == color)
		{
			return level;
			break;
		}
	}
	return nullptr;
}

void ContourBitmap::SortColorMap(std::vector<unsigned char>* colormap)
{
	/*
		bool sorted = false;
		byte b;
		while (!sorted)
		{
			for (unsigned int i = 0; i < colormap->size()-1; i++)
			{
				if (colormap[i] > colormap[i + 1])
				{
	//				b = colormap->at(i);
	//				colormap[i] = colormap[i + 1];
	//				colormap[i + 1] = b;
				}
			}
		}
	*/
}

// Закрашивает буфер изображения белым цветом
// Необходимо переписать (Один цикл от i=0 до BufferLength)
void ContourBitmap::ClearPixelBuffer()
{
	for (int j = 0; j < m_Height; j++)
	{
		for (int i = 0; i < (m_Width * 4); i += 4)
		{
			int pos = j * (m_Width * 4) + i;
			m_pPixelBuffer[pos] = 0xFF;
			m_pPixelBuffer[pos + 1] = 0xFF;
			m_pPixelBuffer[pos + 2] = 0xFF;
			m_pPixelBuffer[pos + 3] = 0xFF;
		}
	}
}

inline void ContourBitmap::SetPixel(int x, int y, unsigned char b, unsigned char g, unsigned char r, unsigned char a)
{
	int pos = y * (m_Width * 4) + x * 4;

	m_pPixelBuffer[pos] = b;
	m_pPixelBuffer[pos + 1] = g;
	m_pPixelBuffer[pos + 2] = r;
	m_pPixelBuffer[pos + 3] = a;
}

/*
	Display params properties
*/

byte DisplayParams::Color::get() { return m_Color; }
void DisplayParams::Color::set(unsigned char color) { m_Color = color; }

bool DisplayParams::DisplayShapes::get() { return m_DisplayShapes; }
void DisplayParams::DisplayShapes::set(bool displayShapes) { m_DisplayShapes = displayShapes; }

bool DisplayParams::DisplayContours::get() { return m_DisplayContours; }
void DisplayParams::DisplayContours::set(bool displayContours) { m_DisplayContours = displayContours; }