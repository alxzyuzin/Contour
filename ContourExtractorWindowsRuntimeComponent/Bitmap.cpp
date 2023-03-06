/*---------------------------------------------------------------------------------
 * Copyright(c) 2023 Alexandr Ziuzin.
 * e-mail alx.zyuzin@gmail.com
 * This file is part of Contour project.
 *
 * Main class for image manipulation
 *
 ---------------------------------------------------------------------------------*/

#include "pch.h"
#include "Bitmap.h"
#include <ppltasks.h>
#include <ColorGroup.h>
#include "Color.h"

using namespace concurrency;
using namespace std;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::Storage::Streams;
using namespace Platform;
using namespace Windows::Foundation;

using namespace ContourExtractorWindowsRuntimeComponent;

//=============================================================================
// Properties
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

// Array of image colors
Array<unsigned int>^ ContourBitmap::Colors::get()
{
	Array<unsigned int>^ ColorMap = ref new Array<unsigned int>((int)m_Levels.size());
	int i = 0;
	for (auto levelItem : m_Levels)
	{
		ColorMap[i++] = levelItem.second->m_OriginalColor;
	}
	return ColorMap;
}

// Number of levels in converted image  
int ContourBitmap::LevelsCount::get()
{
	return (int) m_Levels.size();
}

//-----------------------------------------------------------------------------
// Public members
//-----------------------------------------------------------------------------

ContourBitmap::ContourBitmap()
{
	m_Width = 0;
	m_Height = 0;
	m_PixelBufferLength = 0;
	m_uintImageDataLength = 0;
}


/// <summary>
/// ContourBitmap constructor
/// </summary>
/// <param name="width">
/// Image width
/// </param>
/// <param name="height">
/// Image height
/// </param>
ContourBitmap::ContourBitmap(int width, int height)
{
	m_Width = width;   
	m_Height = height;
	m_PixelBufferLength = 4 * width * height;
	m_uintImageDataLength = width * height;
	m_Bitmap = ref new WriteableBitmap(width, height);
	m_pOriginalImageData = new unsigned char[m_PixelBufferLength];  // Create buffer for original image data
	m_pConvertedImageData = new unsigned char[m_PixelBufferLength]; // Create buffer for converted image data
}


/// <summary>
/// Extract pointer to  WritableBitmap internal buffer
/// All image conversions handle right in WritableBitmap internal buffer
/// </summary>
/// <param name="stream"></param>
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
	pBufferByteAccess->Buffer(&m_pPixelBuffer); // Now m_pPixelBuffer contains address of image data in the internal WritableBitmap buffer
	// Make copy image data to m_pOriginalImageData before any manipulations
	memcpy(m_pOriginalImageData, m_pPixelBuffer, m_PixelBufferLength);
	// Make possible work with image data in two ways (as byte array and as unsigned int array
	m_ImageData.charBuffer = m_pPixelBuffer;
}

/// <summary>
/// Convert original color image into grayscale with defined number of gray tints
/// </summary>
/// <param name="levels">
/// Number of desirable gray tints
/// </param>
void ContourBitmap::ConvertToGrayscale(unsigned char levels)
{
	// Убедимся что входной параметр нажодится в допустимом диапазоне
	if (levels < 2 || levels > 255) throw ref new InvalidArgumentException();

	//Restore original image data
	std::memcpy(m_pPixelBuffer, m_pOriginalImageData, m_PixelBufferLength);

	// Split aall range of possible gray colors into ranges
	int range = 255 / levels;
	std::vector<GrayColorRange> colorRanges;
	GrayColorRange grayColorRange;
	unsigned char bottomLevel = 0;
	// Define for each range top value, bottom value and average color 
	while (grayColorRange.top < 255)
	{
		grayColorRange.bottom = bottomLevel;
		grayColorRange.top = bottomLevel + range;
			
		if ((bottomLevel + range) > 255)
			grayColorRange.top = 255;

		grayColorRange.value = (unsigned char)((grayColorRange.bottom + grayColorRange.top) / 2);
		colorRanges.push_back(grayColorRange);
		bottomLevel += (range + 1);
	}
	// Convert source image to grayscaled image
	for (auto colorRange : colorRanges)
		for (int j = 0; j < m_PixelBufferLength; j+=4)
		{
			if ( colorRange.ContainColor(m_pPixelBuffer[j], m_pPixelBuffer[j + 1], m_pPixelBuffer[j + 2]) )
			{
				m_pPixelBuffer[j] = colorRange.value;;
				m_pPixelBuffer[j + 1] = colorRange.value;;
				m_pPixelBuffer[j + 2] = colorRange.value;;
				m_pPixelBuffer[j + 3] = 0xFF;
			}
	}
	// Save converted image to the buffer for later use
	std::memcpy(m_pConvertedImageData, m_pPixelBuffer, m_PixelBufferLength);
}

/// <summary>
/// Convert original color image into image with reduced number of colors
/// </summary>
/// <param name="numberOfColors">
/// Number of desirable colors in result image
/// </param>
void ContourBitmap::ConvertToReducedColors(unsigned char numberOfColors)
{
	std::vector<ColorGroup*> colorGroups;

	// Check if input parameter is correct
	if (numberOfColors < 2 || numberOfColors > 128) throw ref new InvalidArgumentException();
	//Restore original image data
	memcpy(m_pPixelBuffer, m_pOriginalImageData, m_PixelBufferLength);

	// Create first color group containes all image colors based on data from pixel buffer 
	ColorGroup* colorGroup = new ColorGroup(m_pPixelBuffer, m_PixelBufferLength);
	colorGroups.push_back(colorGroup);
	
	// Split first color group according number of desired colors in image
	for (int k = 1; k < numberOfColors; k++)
	{
		// Find color group with max base color range
		sort(colorGroups.begin(), colorGroups.end(), [](ColorGroup* a, ColorGroup* b) { return a->MaxColorRange() > b->MaxColorRange(); });
		ColorGroup* cg = colorGroups[0];
		
		// Split found color group with max base color range to two color groups
		ColorGroup* a = new ColorGroup();
		ColorGroup* b = new ColorGroup();
		cg->Split(a, b);
		// Remove pointer to splitted color group from color groupss list and delete it from the list
		colorGroups.erase(colorGroups.begin());
		delete cg;
		
		// Add two new color groups to color groups list
		colorGroups.push_back(a);
		colorGroups.push_back(b);
	}
	// Convert source image to image with reduced number of colors
	for (ColorGroup* group : colorGroups)
	{
		for (int pos = 0; pos < m_PixelBufferLength; pos += 4)
		{
			// If pixel color belongs to group change pixel color to group average color
			if (group->Contain(m_pPixelBuffer[pos], m_pPixelBuffer[pos + 1], m_pPixelBuffer[pos + 2]))
			{
				m_pPixelBuffer[pos] = group->AverageRed();
				m_pPixelBuffer[pos + 1] = group->AverageGreen();
				m_pPixelBuffer[pos + 2] = group->AverageBlue();
				//m_pPixelBuffer[pos + 3] = 0xFF;
			}
		}
	}
	// Save converted image to the buffer for later use
	memcpy(m_pConvertedImageData, m_pPixelBuffer, m_PixelBufferLength);
}


/// <summary>
/// Split color image to levels. Every level contains pixels of one color.
/// For each color in original image function create separate level.
/// The level size fits to size of original image.
/// Function moves to level only pixels with color for wihich this level been created,
/// other pixels set to color 0xFF (mean empty color)
/// </summary>
/// <param name="conversionType"></param>
int ContourBitmap::ExtractLevels()
{
	map<unsigned int, unsigned char> colormap;
		
	// Build list of color in image
	unsigned char levelValue = 1;
	for (unsigned int i = 0; i < m_uintImageDataLength; i++)
	{
		unsigned int c = m_ImageData.intBuffer[i];
		// If pixel color not in the list of colos add it to list
		if (colormap.count(c) == 0)
		{
			colormap.insert(pair<unsigned int, unsigned char>(c, levelValue));
			levelValue += 2;
		}
	}
	
	// Delete results of previouse extracting levels
	for (auto l : m_Levels)
		delete l.second;
	m_Levels.clear();
	
	// Move each color into separate buffer
	// Variable m_Levels contains list of levels (objects of class Level). Each Level presents slice of image by one color
	// Levels data stored in byte array. Each pixel of source image represents with one byte in level data.
	// Level constructor extract pixels with color defaned by parameter levelColor from m_pPixelBuffer
	// and create slice of source image by color levelColor
	// Create new levels
	for (pair<unsigned int, unsigned char> colorPair : colormap)
	{
		m_Levels.insert(pair<unsigned int, Level*>(colorPair.first, new Level(m_Width, m_Height, colorPair, m_ImageData)));
	}
	// Sort array of levels by level color
	//std::sort(m_Levels.begin(), m_Levels.end(), [](pair<unsigned int, Level*>& a, pair<unsigned int, Level*>& b)
	//	{
	//		return true;// a.second->m_OriginalColor < b.second->m_OriginalColor;
	//	});
	
	return (int)m_Levels.size();
}

/// <summary>
///  Look for contours in difined level
/// </summary>
/// <param name="levelnumber">
///  Level number in the list of levels in bitmap 
/// </param>
/// <returns>
/// Number of contours found
/// </returns>
//int ContourBitmap::FindLevelContours(int levelnumber)
//{
//	return m_Levels[levelnumber]->FindAllContours();
//}

IAsyncOperation<int>^ ContourBitmap::FindLevelContoursAsync(unsigned int levelColor)
{
	return create_async([this, levelColor]()->int
		{
			return m_Levels[levelColor]->FindAllContours();
		}
	);
}



void ContourBitmap::SetOriginalImageDataToDisplayBuffer()
{
	std::memcpy(m_pPixelBuffer, m_pOriginalImageData, m_PixelBufferLength);
}

void ContourBitmap::SetConvertedImageDataToDisplayBuffer()
{
	std::memcpy(m_pPixelBuffer, m_pConvertedImageData, m_PixelBufferLength);
}

void ContourBitmap::SetLevelDataToDisplayBuffer(unsigned int color)
{
	//Level* level = SelectLevel(color);
	//if (!level) return;

	m_Levels[color]->SetLevelShapesToDisplayBuffer(m_ImageData);
}
/// <summary>
/// Set contours found in image to display buffer
/// </summary>
void ContourBitmap::DisplayContours(ContourColors contourcolor)
{
	for (auto level : m_Levels)
		DisplayLevelContours(level.second->m_OriginalColor, contourcolor);
}


/// <summary>
/// Очистка объекта класса ContourBitmap и освобождение памяти занятой данными объекта
/// </summary>
void ContourBitmap::Clear()
{
	for (auto level : m_Levels)
	{
		level.second->Clear();
		delete level.second;
	}
	
	delete[] m_pOriginalImageData;
	delete[] m_pConvertedImageData;
	delete m_Bitmap;
	m_Width = 0;				// Ширина изображения в рикселях
	m_Height = 0;				// Высота изображения в рикселях
	m_PixelBufferLength = 0;	// Длина буфера изображения в байтах
	m_Initialized = false;
}

/// <summary>
/// Выполняет поиск всех контуров в изображении
/// </summary>
void ContourBitmap::OutlineImage()
{
	clock_t start = clock();

	for (auto level : m_Levels)
		level.second->FindAllContours();
	// time содержит время выполнения функции в милисекундах 
	double time = (clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
	// Изображение разделено на 8 слоёв. Исходное время построения контуров 58818 ms (760 контуров)
	// Изображение разделено на 2 слоя. Исходное время построения контуров  5774 (4587) ms (40 контуров)
	// Изображение разделено на 4 слоя. Исходное время построения контуров 409 818 ms (760 контуров)
}

/// <summary>
/// Удаляет из слоя группы пикселей которые вписываются в квадрат со стороной равной size пикселей
/// </summary>
/// <param name="color">
/// Цвет слоя в котором нужно произвести очистку
/// </param>
/// <param name="size">
/// Длина стороны квадрата в который должны вписываться группы пикселей подлежащие удалению
/// </param>
void ContourBitmap::RectifyLevel(unsigned int color, int size)
{
	//Level* selectedLevel = SelectLevel(color);
	//selectedLevel->Rectify(size);
}

//-----------------------------------------------------------------------------
// Private members
//-----------------------------------------------------------------------------



/// <summary>
/// Отрисовывает в буфере дисплея контуры для слоя цвет которого задан во входном параметре
/// </summary>
/// <param name="color">
/// Цвет слоя для которого требуется отрисовать контур
/// </param>
void ContourBitmap::DisplayLevelContours(unsigned int levelcolor, ContourColors contourcolor)
{
	Point* point;

	for (Contour* contour : m_Levels[levelcolor]->m_Contours)
		for (int i = 0; i < contour->Size(); i++)
		{
			point = contour->GetPoint(i);

			switch (contourcolor)
			{
			case ContourColors::Black	: SetPixel(point->X, point->Y, 0x00, 0x00, 0x00, 0xFF); break;
			case ContourColors::Blue	: SetPixel(point->X, point->Y, 0xFF, 0x00, 0x00, 0xFF); break;
			case ContourColors::Green	: SetPixel(point->X, point->Y, 0x00, 0xFF, 0x00, 0xFF); break;
			case ContourColors::Red		: SetPixel(point->X, point->Y, 0x00, 0x00, 0xFF, 0xFF); break;
			case ContourColors::White	: SetPixel(point->X, point->Y, 0xFF, 0xFF, 0xFF, 0xFF); break;
			}
		}
}



//// Возвращает указатель на уровень заданного цвета
//
//Level* ContourBitmap::SelectLevel(unsigned int color)
//{
//	for (auto level : m_Levels)
//	{
//		if (level.second->m_OriginalColor == color)
//		{
//			return level.second;
//			break;
//		}
//	}
//	return nullptr;
//}


// Закрашивает буфер изображения белым цветом
// Необходимо переписать (Один цикл от i=0 до BufferLength)
void inline ContourBitmap::ClearPixelBuffer()
{

	for (int i = 0; i < m_PixelBufferLength; i++)
		m_pPixelBuffer[i] = 0xFF;
}

inline void ContourBitmap::SetPixel(int x, int y, unsigned char b, unsigned char g, unsigned char r, unsigned char a)
{
	int pos = y * (m_Width * 4) + x * 4;

	m_pPixelBuffer[pos] = b;
	m_pPixelBuffer[pos + 1] = g;
	m_pPixelBuffer[pos + 2] = r;
	m_pPixelBuffer[pos + 3] = a;
}

bool ContourBitmap::CompareLevelsByOriginalColor(Level& l1, Level& l2)
{
	return l1.m_OriginalColor < l2.m_OriginalColor;
}
