/*---------------------------------------------------------------------------------
 * Copyright(c) 2023 Alexandr Ziuzin.
 * e-mail alx.zyuzin@gmail.com
 *
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
using namespace Microsoft::WRL;

using namespace ContourExtractorWindowsRuntimeComponent;

//=============================================================================
// Properties
//=============================================================================
WriteableBitmap^	ContourBitmap::ImageData::get() { return m_Bitmap; }

unsigned int ContourBitmap::Width::get()  { return (unsigned int) m_Width; }
unsigned int ContourBitmap::Height::get() { return (unsigned int) m_Height; }

IRandomAccessStream^ ContourBitmap::ImageDataStream::get()
{
	//return m_Bitmap->PixelBuffer.;
	return nullptr;
}

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
	for (auto& levelItem : m_Levels)
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


//int ContourBitmap::AvailableColorsAmount::get()
//{
//	// Максимальный требуемый объём памяти для обработки изображения
//	// (Ширина * высоту) - размер буфера в Level
//	// умножить на 2 (два буфера)
//	// умножить на количество цветов
//	// + плюс память для хранения контуров ( подсчитаем позднее)
//
//	// Пытаемся выделить требуемый объём памяти
//	bool memoryAllocated = false;
//	int colorsNumber = 128;
//	unsigned char* buffer = nullptr;
//
//	while (!memoryAllocated && colorsNumber > 1)
//	{
//		try
//		{
//			long requiredMemoryAmount = m_Width * m_Height * colorsNumber * 256;
//			unsigned char* buffer = new unsigned char[requiredMemoryAmount];
//			memoryAllocated = true;
//		}
//		catch (bad_alloc ex)
//		{
//			colorsNumber--;
//		}
//	}
//
//	delete[] buffer;
//
//	return colorsNumber;
//}

//-----------------------------------------------------------------------------
// Public members
//-----------------------------------------------------------------------------

ContourBitmap::ContourBitmap()
{
	m_Width = 0;
	m_Height = 0;

	m_uintPixelBufferLength = 0;
	m_bytePixelBufferLength = 0;

	m_Bitmap = nullptr;
	m_pOriginalImageData = nullptr;
	m_pConvertedImageData = nullptr;
}

/// <summary>
/// ContourBitmap constructor (we will use this constructor for testing only)
/// </summary>
/// <param name="width">
/// Image width
/// </param>
/// <param name="height">
/// Image height
/// </param>
/// <param name="imageData">
/// Pointer to image data
/// </param>
//ContourBitmap::ContourBitmap(int width, int height, unsigned int* imageData)
//{
//	m_Width = width;
//	m_Height = height;
//
//	m_uintPixelBufferLength = width * height;
//	m_bytePixelBufferLength = m_uintPixelBufferLength * 4;
//
//	m_Bitmap = ref new WriteableBitmap(width, height);
//	m_pOriginalImageData = new unsigned char[m_bytePixelBufferLength];  // Create buffer for original image data
//	m_pConvertedImageData = new unsigned char[m_bytePixelBufferLength];  // Create buffer for converted image data
//	m_PixelBuffer = imageData;
//	// Make copy image data to m_pOriginalImageData before any manipulations
//	SaveOriginalImageData();
//}

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

	m_uintPixelBufferLength = width * height;
	m_bytePixelBufferLength = m_uintPixelBufferLength *4;

	m_Bitmap = ref new WriteableBitmap(width, height);
	m_pOriginalImageData  = new unsigned char[m_bytePixelBufferLength];  // Create buffer for original image data
	m_pConvertedImageData = new unsigned char[m_bytePixelBufferLength];  // Create buffer for converted image data
}

void ContourBitmap::Invalidate()
{
	m_Bitmap->Invalidate();
}

/// <summary>
/// Extract pointer to  WritableBitmap internal buffer
/// All image conversions handle right in WritableBitmap internal buffer
/// </summary>
/// <param name="stream"></param>
void ContourBitmap::SetSource(IRandomAccessStream^ stream)
{
	m_Bitmap->SetSource(stream);  // m_Bitmap - WritableBitmap^
	m_PixelBuffer = GetPointerToWriteableBitmapPixelData(m_Bitmap);
	// Make copy image data to m_pOriginalImageData before any manipulations
	SaveOriginalImageData();
}

void ContourExtractorWindowsRuntimeComponent::ContourBitmap::CancelOperation()
{
	if (m_CancellationTokenSource != nullptr)
		m_CancellationTokenSource->cancel();
}



/// <summary>
/// Convert original color image into grayscale with defined number of gray tints
/// </summary>
/// <param name="levels">
/// Number of desirable gray tints
/// </param>

IAsyncActionWithProgress<double>^ ContourBitmap::ConvertToGrayscaleAsync(unsigned int numberOfColors)
{

	// Убедимся что входной параметр нажодится в допустимом диапазоне
	if (numberOfColors < 2 || numberOfColors > 255) throw ref new InvalidArgumentException();
	return create_async([this, numberOfColors](progress_reporter<double> reporter)
		{
			int progress = 1;
	//Restore original image data
	RestoreOriginalImageData();
	// Split full range of possible gray colors into ranges
	int range = 255 / numberOfColors;
	vector<GrayColorRange> colorRanges;
	GrayColorRange grayColorRange;
	unsigned char bottomLevel = 0;
	// Define for each range top and bottom value
	while (grayColorRange.top < 255)
	{
		grayColorRange.bottom = bottomLevel;
		grayColorRange.top = bottomLevel + range;

		if ((bottomLevel + range) > 255)
			grayColorRange.top = 255;

		colorRanges.push_back(grayColorRange);
		bottomLevel += (range + 1);
	}
	reporter.report((double)progress / numberOfColors + 1);
	// Convert source image to grayscaled image
	for (auto& colorRange : colorRanges)
	{
		for (int i = 0; i < m_uintPixelBufferLength; i++)
			if (colorRange.ContainsColor(m_PixelBuffer[i]))
			{
				(m_PixelBuffer)[i] = colorRange.Value();
			}
		reporter.report((double)progress / numberOfColors + 1);
		progress++;
	}
	// Save converted image to the buffer for later use
	SaveConvertedImageData();
		});
}


/// <summary>
/// Convert original color image into image with reduced number of colors
/// </summary>
/// <param name="numberOfColors">
/// Number of desirable colors in result image
/// </param>

IAsyncActionWithProgress<double>^ ContourBitmap::ConvertToReducedColorsAsync(unsigned int numberOfColors)
{
	
	m_CancellationTokenSource = new cancellation_token_source();
	cancellation_token cancellationToken = m_CancellationTokenSource->get_token();
	// Check if input parameter is correct
	if (numberOfColors < 2 || numberOfColors > 128) throw ref new InvalidArgumentException();
	return create_async([this, numberOfColors](progress_reporter<double> reporter)
		{
			double progress = 1;

			std::vector<ColorGroup*> colorGroups;
	
			//Restore original image data
			RestoreOriginalImageData();

			// Create first color group containes all image colors based on data from pixel buffer 
			ColorGroup* colorGroup = new ColorGroup(m_PixelBuffer, m_uintPixelBufferLength);
			colorGroups.push_back(colorGroup);

			reporter.report(progress++ / numberOfColors);
			// Split first color group according number of desired colors in image
		for (unsigned int k = 1; k < numberOfColors; k++)
		{
			// Find color group with max base color range
			sort(colorGroups.begin(), colorGroups.end(), [](ColorGroup* a, ColorGroup* b) { return a->MaxColorRange() > b->MaxColorRange(); });
			ColorGroup* cg = colorGroups[0];

			// Split found color group with max base color range to two color groups
			ColorGroup* a = new ColorGroup();
			ColorGroup* b = new ColorGroup();
			cg->Split(a, b);

			// Remove pointer to splitted color group from color groups list and delete it from the list
			delete cg;
			colorGroups.erase(colorGroups.begin());
			int i =  colorGroups.size();
			

			// Add two new color groups to color groups list
			colorGroups.push_back(a);
			colorGroups.push_back(b);

			reporter.report(progress++ / numberOfColors);
		}
		// Convert source image to image with reduced number of colors
		for (ColorGroup* group : colorGroups)
		{
			for (int i = 0; i < m_uintPixelBufferLength; i++)
			{
				// If pixel color belongs to group change pixel color to group average color
				if (group->Contains(m_PixelBuffer[i]))
				{
					m_PixelBuffer[i] = group->AverageGroupColor();
				}
			}
		}

		colorGroups.clear();
		for (ColorGroup* group : colorGroups)
		{
			delete group;
		}
		// Save converted image to the buffer for later use
		SaveConvertedImageData();
		});
}

IAsyncActionWithProgress<double>^ ContourBitmap::CleanUpImageAsync(int size)
{
	return create_async([this, size](progress_reporter<double> reporter)
	{
		for (int y = 0; y <= m_Height - size; y++)
		{
			for (int x = 0; x <= m_Width - size; x++)
				ClearRectangleArea(x, y, size);
			reporter.report((double)y / (m_Height - size));
		}
		});
}

void ContourBitmap::RotateLeft()
{
	m_BufferBitmap = ref new WriteableBitmap(m_Bitmap->PixelHeight, m_Bitmap->PixelWidth);
	unsigned int* newPixelBuffer = GetPointerToWriteableBitmapPixelData(m_BufferBitmap);

	int h = m_Bitmap->PixelHeight;
	int w = m_Bitmap->PixelWidth;
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
			newPixelBuffer[(w - 1 - x) * h + y] = m_PixelBuffer[y * w + x];
	m_Bitmap = m_BufferBitmap;
	
	int m_Height = m_BufferBitmap->PixelHeight;
	int m_Width  = m_BufferBitmap->PixelWidth;
	m_PixelBuffer = newPixelBuffer;
	SaveOriginalImageData();
}

void ContourBitmap::RotateRight()
{
	m_BufferBitmap = ref new WriteableBitmap(m_Bitmap->PixelHeight, m_Bitmap->PixelWidth);
	unsigned int* newPixelBuffer = GetPointerToWriteableBitmapPixelData(m_BufferBitmap);

	int h = m_Bitmap->PixelHeight;
	int w = m_Bitmap->PixelWidth;
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
			newPixelBuffer[h * x + h - y - 1] = m_PixelBuffer[y * w + x];
	m_Bitmap = m_BufferBitmap;

	int m_Height = m_BufferBitmap->PixelHeight;
	int m_Width = m_BufferBitmap->PixelWidth;
	m_PixelBuffer = newPixelBuffer;
	SaveOriginalImageData();
}

void ContourBitmap::ClearRectangleArea(int left_top_x, int left_top_y, int size)
{
	if (size > m_Width || size > m_Height)
		return;
	int leftTopOffsetX = left_top_y * m_Width + left_top_x;
	int leftBottomOffsetX = leftTopOffsetX + (size - 1) * m_Width;
	int leftTopOffsetY = leftTopOffsetX;
	int rightTopOffsetY = leftTopOffsetY + size - 1;

	int ltox = leftTopOffsetX;
	unsigned int color = m_PixelBuffer[ltox];
	for (int i = 0; i < size; i++)
	{
		if (
			!(m_PixelBuffer[leftTopOffsetX] == color && m_PixelBuffer[leftBottomOffsetX] == color &&
				m_PixelBuffer[leftTopOffsetY] == color && m_PixelBuffer[rightTopOffsetY] == color)
			)
			return;
		++leftTopOffsetX;
		++leftBottomOffsetX;
		leftTopOffsetY += m_Width;
		rightTopOffsetY += m_Width;
	}
	// All border points color is empty color
	// Fill area inside borders with empty color
	for (int y = 1, y_offset = m_Width; y < size - 1; y++, y_offset += m_Width)
		for (int x = 1; x < size - 1; x++)
			m_PixelBuffer[ltox + y_offset + x] = color;
	return;
}

/// <summary>
/// Split color image to levels. Every level contains pixels of one color.
/// For each color in original image function create separate level.
/// The level size fits to size of original image.
/// Function moves to level only pixels with color for wihich this level been created,
/// other pixels set to color 0xFF (mean empty color)
/// </summary>
/// <param name="conversionType"></param>
IAsyncActionWithProgress<double>^ ContourBitmap::ExtractLevelsAsync(int numcolors)
{
	
	// Delete results of previouse extracting levels
	DeleteAllLevels();
	
	m_CancellationTokenSource = new cancellation_token_source();
	cancellation_token cancellationToken = m_CancellationTokenSource->get_token();
	
	return create_async([this, numcolors, cancellationToken](progress_reporter<double> reporter)
	{
			ContourBitmap^ _this = this;
			return create_task([_this, numcolors, cancellationToken, reporter]()
				{

					/*bool memoryAllocated = false;
					int colorsNumber = 128;
					unsigned char* buffer = nullptr;

					while (!memoryAllocated && colorsNumber > 1)
					{
						try
						{
							long requiredMemoryAmount = _this->m_Width * _this->m_Height * colorsNumber * 2;
							unsigned char* buffer = new unsigned char[requiredMemoryAmount];
							memoryAllocated = true;
						}
						catch (bad_alloc ex)
						{
							colorsNumber--;
						}
					}

					delete[] buffer;*/


					/*return;*/

					double progress = 0;
					set<unsigned int> colorset;
					reporter.report(++progress / numcolors);
					// Build list of color in image
					for (int i = 0; i < _this->m_uintPixelBufferLength; i++)
					{
						if (cancellationToken.is_canceled())
						{
							_this->DeleteAllLevels();
							cancel_current_task();

						}
						unsigned int color = _this->m_PixelBuffer[i];
						// If pixel color not in the list of colos add it to list
						if (colorset.count(color) == 0)
						{
							try
							{
								Level* level = new Level(_this->m_Width, _this->m_Height, color, _this->m_PixelBuffer);
								colorset.insert(color);
								_this->m_Levels.insert(pair<unsigned int, Level*>(color, level));
								reporter.report(++progress / numcolors);
							}
							catch (bad_alloc)
							{
								int k = 0;
							}
						}
					}
				});

		// Sort array of levels by level color
		//std::sort(m_Levels.begin(), m_Levels.end(), [](pair<unsigned int, Level*>& a, pair<unsigned int, Level*>& b)
		//	{
		//		return true;// a.second->m_OriginalColor < b.second->m_OriginalColor;
		//	});
	});
	
}

void ContourBitmap::DeleteAllLevels()
{
	for (auto& l : this->m_Levels)
		delete l.second;
	this->m_Levels.clear();
	
}
IAsyncActionWithProgress<int>^ ContourBitmap::FindLevelContoursAsync(unsigned int levelColor)
{
	return m_Levels[levelColor]->FindAllContours();
}


void ContourBitmap::SetOriginalImageDataToDisplayBuffer()
{
	RestoreOriginalImageData();
}

void ContourBitmap::SetConvertedImageDataToDisplayBuffer()
{
	RestoreConvertedImageData();
}

void ContourBitmap::SetLevelDataToDisplayBuffer(unsigned int color)
{
	m_Levels[color]->SetLevelShapesToDisplayBuffer(m_PixelBuffer);
}
/// <summary>
/// Set contours found in image to display buffer
/// </summary>
void ContourBitmap::DisplayContours(ContourColors contourcolor, int minContourLength, unsigned char contourDensity)
{
	for (auto& level : m_Levels)
		level.second->SetContoursToDisplayBuffer(m_PixelBuffer, contourcolor, minContourLength, contourDensity, ContourType::External);
}


/// <summary>
/// Очистка объекта класса ContourBitmap и освобождение памяти занятой данными объекта
/// </summary>
void ContourBitmap::Clear()
{
	for (auto& level : m_Levels)
	{
		level.second->Clear();
		delete level.second;
	}
	
	delete[] m_pOriginalImageData;
	delete[] m_pConvertedImageData;
	delete m_Bitmap;
	m_Width = 0;				// Ширина изображения в рикселях
	m_Height = 0;				// Высота изображения в рикселях
}

/// <summary>
/// Выполняет поиск всех контуров в изображении
/// </summary>
double ContourBitmap::OutlineImage()
{
	clock_t start = clock();

	for (auto& level : m_Levels)
		level.second->FindAllContours();
	// time содержит время выполнения функции в милисекундах 
	double time = (clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
	// Изображение разделено на 8 слоёв. Исходное время построения контуров 58818 ms (760 контуров)
	// Изображение разделено на 2 слоя. Исходное время построения контуров  5774 (4587) ms (40 контуров)
	// Изображение разделено на 4 слоя. Исходное время построения контуров 409 818 ms (760 контуров)
	return time;
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
	m_Levels[color]->Rectify(size);
}

// Закрашивает буфер изображения белым цветом
void inline ContourBitmap::ClearPixelBuffer()
{
	for (int i = 0; i < m_uintPixelBufferLength; i++)
		m_PixelBuffer[i] = 0xFFFFFFFF;
}


inline void ContourBitmap::SaveOriginalImageData()
{
	memcpy(m_pOriginalImageData, m_PixelBuffer, m_bytePixelBufferLength);
}
inline void ContourBitmap::SaveConvertedImageData()
{
	memcpy(m_pConvertedImageData, m_PixelBuffer, m_bytePixelBufferLength);
}
inline void ContourBitmap::RestoreOriginalImageData()
{
	memcpy(m_PixelBuffer, m_pOriginalImageData, m_bytePixelBufferLength);
}
inline void ContourBitmap::RestoreConvertedImageData()
{
	memcpy(m_PixelBuffer, m_pConvertedImageData, m_bytePixelBufferLength);
}

int ContourBitmap::GetPossibleNumberOfColors(int numberOfColors)
{
	vector<Level*> levels;
	int i = 0;
	try
	{
		for (; i < numberOfColors; i++)
		{
			Level* level = new Level(this->m_Width, this->m_Height, 0x000000000, this->m_PixelBuffer);
			levels.push_back(level);
		}
	}
	catch (bad_alloc)
	{
		for (Level* l : levels)
		{
			delete l;
		}
		levels.clear();

	}
	return i;
}
//======================================================================
// Private functions
//======================================================================

unsigned int* ContourBitmap::GetPointerToWriteableBitmapPixelData(WriteableBitmap^ bitmap)
{
	unsigned char* bytePixelBuffer;
	// Obtain access to image data
	ComPtr<IBufferByteAccess> pBufferByteAccess;
	IBuffer^ pIBuffer = bitmap->PixelBuffer;
	ComPtr<IUnknown> pBuffer((IUnknown*)pIBuffer);
	pBuffer.As(&pBufferByteAccess);
	// Get pointer to pixel bytes
	pBufferByteAccess->Buffer(&bytePixelBuffer); // Now bytePixelBuffer contains address of image data in the internal WritableBitmap buffer
	return (unsigned int*)bytePixelBuffer;
}
