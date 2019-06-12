#include "pch.h"
#include "Level.h"

using namespace ContourExtractorWindowsRuntimeComponent;

Level::Level() {};
Level::Level(int width, int height, unsigned char levelColor, unsigned char pPixelBuffer[])
{
	if (width <= 0)
		throw std::invalid_argument("parameter width <= 0");
	if (height <= 0)
		throw std::invalid_argument("parameter width <= 0");
	if (levelColor == 0xFF)
		throw std::invalid_argument("Received levelColor value 0xFF");
	if (!pPixelBuffer)
		throw std::invalid_argument("Pointer to pPixelBuffer is null");

	m_Width = width;
	m_Height = height;
	m_Color = levelColor;
	m_BufferLength = width * height;
	m_pBuffer = new unsigned char[m_BufferLength];
	m_pShapesBuffer = new unsigned char[m_BufferLength];
	// Закрасим слой белым цветом
	// При преобразовании исходного изображения к оттенкам серого
	// в полученном изображении не будет точек белого цвета
	// (определяется алгоритмом преобразования) 
	for (int i = 0; i < m_BufferLength; i++)
		m_pBuffer[i] = 0xFF;

	// Сожмём данные исходного изображения
	// 4 байта исходного изображения в оттенках серого сохраняем в буфере слоя в одном байте
	// поскольку байты RGB исходного изображения содержат одинаковые значения 
	for (int y = 0; y < m_Height; y++)
	{
		for (int x = 0, k = 0; x < (m_Width * 4); x += 4, k++)
		{
			unsigned char bitmapPixelColor = pPixelBuffer[y * (m_Width * 4) + x];
			int levelOffset = y * m_Width + k;
			m_pBuffer[y * m_Width + k] = (bitmapPixelColor == levelColor) ? levelColor : 0xFF;
		}
	}
	// Сделаем копию сформированных данных в m_pShapesBuffer.
	// Данные из этого буфера будем использовать для рисования слоя на экране
	for (int i = 0; i < m_BufferLength; i++)
		m_pShapesBuffer[i] = m_pBuffer[i];

}

Level::~Level()
{
	for (auto contour : m_Contours)
		delete contour;

	delete[] m_pShapesBuffer;
	delete[] m_pBuffer;
}

void Level::Clear()
{
	for (auto contour : m_Contours)
		delete contour;

	delete[] m_pBuffer;
	m_BufferLength = 0;
	m_Width = 0;
	m_Height = 0;
	m_Color = 0;
}

void Level::Rectify(int size)
{
	for (int y = size; y < m_Height - 2 * size; y++)
	{
		for (int x = size; x < m_Width - 2 * size; x++)
		{
			if (BorderHasOnlyOneColor(x, y, size)) // В окружении на расстоянии +-size нет цветных точек
			{
				unsigned char centralColor = GetPixel(x, y);
				if (centralColor != 0xFF)
				{
					// Закрасим внутренность белым цветом
					for (int j = y - size; j <= y + size; j++)
						for (int i = x - size; i <= x + size; i++)
							SetPixel(i, j, EMPTY_COLOR);
				}
			}
		}
	}
}

/*
Переносит все закрашенные области слоя в буфер изображения отображаемого на экране
*/
void Level::GetLevelShapes(unsigned char* pPixelBuffer)
{
	for (int y = 0; y < m_Height; y++)
	{
		for (int x = 0; x < m_Width; x++)
		{
			int PixelBufferOffset = (y * m_Width + x) * 4;
			unsigned char pixelColor = m_pShapesBuffer[y * m_Width + x];
			if (pixelColor == m_Color)
			{
				pPixelBuffer[PixelBufferOffset] = m_Color;
				pPixelBuffer[PixelBufferOffset + 1] = m_Color;
				pPixelBuffer[PixelBufferOffset + 2] = m_Color;
				pPixelBuffer[PixelBufferOffset + 3] = EMPTY_COLOR;
			}
		}
	}
}

void Level::SetPixel(Point* point, unsigned char color)
{
	m_pBuffer[point->Y * m_Width + point->X] = color;
}

void Level::SetPixel(int x, int y, unsigned char color)
{
	m_pBuffer[y * m_Width + x] = color;
}

unsigned char Level::GetPixel(int x, int y)
{
	int pos = y * m_Width + x;
	unsigned char c = m_pBuffer[y * m_Width + x];
	return m_pBuffer[y * m_Width + x];
}

/*
Ищет первую точку контура (первую попавшуюся точку области закрашенной цветом shapeColor.
OUT parameters
	point - found point. Variable point contains found point coords.
RETURN value
	true  - if point found
	false - if point NOT found
*/
bool Level::FindFirstExternalContourPoint(Point& point)
{
	for (int y = 0; y < m_Height; y++)
		for (int x = 0; x < m_Width; x++)
		{
			unsigned char c = m_pBuffer[y * m_Width + x]; //
			if (m_pBuffer[y * m_Width + x] == m_Color)
			{
				point.X = x;
				point.Y = y;
				return true;
			}
		}
	return false;
}

/*
Находит следующую точку контура
Если такая точка найдена то создаёт новый объект Point и возвращает указатель на него
Если точка не найдена то возвращает nullptr
*/
bool Level::FindNextExternalContourPoint(Point& currentPoint, Direction direction)
{
	int x = currentPoint.X;
	int y = currentPoint.Y;

	switch (direction)
	{
	case N:  --y;		break;
	case NE: ++x; --y;	break;
	case E:  ++x;		break;
	case SE: ++x; ++y; 	break;
	case S:  ++y; 		break;
	case SW: --x; ++y;	break;
	case W:  --x; 		break;
	case NW: --x; --y;	break;
	}
	// Проверяем не выходит ли точка за границы изображения
	if (y >= 0 && y < m_Height && x >= 0 && x < m_Width && (GetPixel(x, y) == m_Color))
	{
		currentPoint.X = x;
		currentPoint.Y = y;
		return true;
	}
	return false;
}

/*
Функция выполняет поиск внешнего контура первой попавшейся закрашенной области
закрашенной цветом shapeColor внутри области определяемой контуром parentContour
Возвращаемое значение

	Если контур найден - указатель на объект класса Contour, содержащий список
	точек контура иначе nullptr

Доработки
	Попробовать прекращать поиск точек контура как только новая точка
	совпадёт с одной из найденных точек контура
	В этом случае контура без внутренних точек будут короче и возможно поиск
	точек контура будет заканчиваться быстрее.
*/
Contour* Level::FindExternalContour()
{
	Point firstPoint(0, 0);
	bool firstPointFound = FindFirstExternalContourPoint(firstPoint);

	if (!firstPointFound)
		return nullptr;

	Contour* contour = new Contour(Contour::ContourType::External);
	contour->AddPoint(firstPoint);

	Point nextPoint = firstPoint;
	// Hаправлениe, в котором была найдена последняя точка контура, всегда E.
	// Это следует из алгоритма поиска первой точки контура.
	Direction searchDirection = Direction::E;
	while (true)
	{
		// Выбираем новое начальное направление поска очередной точки
		// контура исходя из направления, в котором была найдена последняя точка контура
		searchDirection = StartDirection(searchDirection);

		int l;
		for (l = 0; l < 8; l++)
		{
			bool nextPointFound = FindNextExternalContourPoint(nextPoint, searchDirection);
			if (nextPointFound)
			{
				if ((nextPoint.X == firstPoint.X) && (nextPoint.Y == firstPoint.Y))
					return contour;
				else
				{
					contour->AddPoint(nextPoint);
					break;
				}
			}
			searchDirection = NextDirection(searchDirection);
		}
		if (l == 8)  // сделали полный круг и не нашли продолжения контура
			break; // контур состоит из одного пикселя
	}
	return contour;
}

bool Level::FindFirstInternalContourPoint(Contour* contour, Point& point)
{
	if (!contour)
		throw std::invalid_argument("Pointer to parentContour is null");

	if (contour->Length < 4)
		return false;

	Point* prevPoint = nullptr;
	Point* currPoint = contour->GetPoint(0);
	Point* nextPoint = contour->GetPoint(contour->Size() - 1);
	for (int i = contour->Size() - 2; i >= 0; i--)
	{
		prevPoint = currPoint;
		currPoint = nextPoint;
		nextPoint = contour->GetPoint(i);

		if ((prevPoint->Y < currPoint->Y) && (currPoint->Y <= nextPoint->Y))
		{
			Point* StartPoint = currPoint;
			Point* EndPoint = contour->FindRightNearestPoint(i + 1);
			if (!EndPoint)
				continue;
			for (int x = StartPoint->X + 1; x < EndPoint->X; x++)
			{
				if (m_pBuffer[StartPoint->Y * m_Width + x] == EMPTY_COLOR)
				{
					point.X = x;
					point.Y = StartPoint->Y;
					return true;
				}
			}
		}
	}
	return false;
}

bool Level::CheckNextInternalContourPoint(Contour* parentContour, Point& point, Direction direction)
{
	if (!parentContour)
		throw std::invalid_argument("Pointer to parentContour is null");

	int x = point.X;
	int y = point.Y;

	switch (direction)
	{
	case N:  --y;		break;
	case NE: ++x; --y;	break;
	case E:  ++x;		break;
	case SE: ++x; ++y; 	break;
	case S:  ++y; 		break;
	case SW: --x; ++y;	break;
	case W:  --x; 		break;
	case NW: --x; --y;	break;
	}

	if (GetPixel(x, y) == 0xFF && parentContour->ContainsPoint(x, y))
	{
		point.X = x;
		point.Y = y;
		return true;
	}
	return false;
}

bool Level::FindNextInternalContourPoint(Contour* parentContour, Point& point, Direction& direction)
{
	for (Direction d : *m_ClockwiseDirectionMap[direction])
	{
		Point p = point;
		bool pointFound = CheckNextInternalContourPoint(parentContour, p, d);
		if (pointFound)
		{
			point = p;
			direction = d;
			return true;
		}
	}
	return false;
}

Contour* Level::FindInternalContour(Contour* parentContour)
{
	if (!parentContour)
		throw std::invalid_argument("Pointer to parentContour is null");

	Contour* contour = nullptr;
	Point point = Point(MAXINT, MAXINT);
	Direction direction = E;

	bool pointFound = FindFirstInternalContourPoint(parentContour, point);
	Point firstContourPoint = point;
	if (pointFound)
		contour = new Contour(Contour::ContourType::Internal);

	while (pointFound)
	{
		contour->AddPoint(point);
		pointFound = FindNextInternalContourPoint(parentContour, point, direction);
		if (point == *contour->GetPoint(0))
			break;
	}
	return contour;
}

void Level::FindAllContours()
{
	Contour* externalContour = nullptr;
	do
	{
		externalContour = FindExternalContour();
		if (externalContour)
		{
			m_Contours.push_back(externalContour);
			Contour* internalContour = nullptr;
			do
			{
				internalContour = FindInternalContour(externalContour);
				if (internalContour)
				{
					m_Contours.push_back(internalContour);
					FillContour(internalContour, m_Color);
				}
			} while (internalContour);
			FillContour(externalContour, EMPTY_COLOR);
		}
	} while (externalContour);
}

/*
Стирает все точки в рабочем буффере m_pBuffer
*/
void Level::EraseBuffer()
{
	for (int i = 0; i < m_BufferLength; i++)
		m_pBuffer[i] = EMPTY_COLOR;
}

/*
Закрашивает внутренность контура заданным цветом
*/
void Level::FillContour(Contour* contour, unsigned char color)
{
	// Минимальная длина контура содержащего внутреннюю область равна 4
	// таким образом для контура с длиной меньше 4 достаточно стереть сам контур
	if (contour->Length < 4)
	{
		for (int i = 0; i < contour->Length; i++)
			SetPixel(contour->GetPoint(i), color);
		return;
	}

	Contour::SearchNearestPointDirection searchDirection = Contour::SearchNearestPointDirection::Left;

	Point nullPoint = Point(-1, -1);
	Point* p0 = &nullPoint;
	Point* p1 = &nullPoint;
	Point* p2 = contour->GetPoint(0);

	//ErasePixel(contour->GetPoint(0));
	SetPixel(contour->GetPoint(0), color);
	for (int i = 1; i < contour->Length; i++)
	{
		p0 = p1;
		p1 = p2;
		p2 = contour->GetPoint(i);

		// Закрашиваем сам контур 
		SetPixel(p2, color);

		if (p1->Y != p2->Y)
		{
			// Если координата Y увеличивается, устанавливаем направление поиска точки на 
			// противоположной стороне контура слева
			// Если координата Y уменьшается, устанавливаем направление поиска точки на 
			// противоположной стороне контура справа

			searchDirection = (p2->Y > p1->Y) ? Contour::SearchNearestPointDirection::Left : Contour::SearchNearestPointDirection::Right;

			if (p0->Y == p1->Y)
				FillLine(contour, i - 1, searchDirection, color);
			FillLine(contour, i, searchDirection, color);
		}

	}
}

/*
	Восстанавливает все точки внутри заданного контура.
	Для восстановления использует данные из m_pShapesBuffer
	Алгоритм восстановления содержимого контура аналогичен алгоритму стирания контура,
	за исключением того что значение цвета пикселя внутри контура  не равно константе
	0xFF а берётся из соответствующего пикселя буфера m_pShapesBuffer
	Точки самого контура не восстанавливаются
*/
void Level::RestoreContourContent(Contour* contour)
{
	// Минимальная длина контура содержащего внутреннюю область равна 4
	// таким образом для контура с длиной меньше 4 нет необходимости воостанавливать
	// 	содержание внутренней области
	if (contour->Length < 4)
		return;

	Contour::SearchNearestPointDirection searchDirection = Contour::SearchNearestPointDirection::Left;

	Point nullPoint = Point(-1, -1);
	Point* p0 = &nullPoint;
	Point* p1 = &nullPoint;
	Point* p2 = contour->GetPoint(0);

	for (int i = 1; i < contour->Length; i++)
	{
		p0 = p1;
		p1 = p2;
		p2 = contour->GetPoint(i);

		if (p1->Y != p2->Y)
		{
			// Если координата Y увеличивается, устанавливаем направление поиска точки на 
			// противоположной стороне контура слева
			// Если координата Y уменьшается, устанавливаем направление поиска точки на 
			// противоположной стороне контура справа

			searchDirection = (p2->Y > p1->Y) ? Contour::SearchNearestPointDirection::Left : Contour::SearchNearestPointDirection::Right;

			if (p0->Y == p1->Y)
				RestoreLine(contour, i - 1, searchDirection);
			RestoreLine(contour, i, searchDirection);
		}
	}

}

//----------------------------------------------------------------------------
// Вычисляет новое начальное направление поиска следующей точки исходя из напрвления 
// в котором находится текущая точка по отношению к предыдущей
// Параметры
//		direction  - направление от предыдущей точки к текущей точке
//----------------------------------------------------------------------------

Direction Level::StartDirection(Direction direction)
{
	switch (direction)
	{
	case S: return NE;
	case SW: return E;
	case W: return SE;
	case NW: return S;
	case N: return SW;
	case NE: return W;
	case E: return NW;
	case SE: return N;
	default:return N;
	}
}

Direction Level::NextDirection(Direction direction)
{
	switch (direction)
	{
	case N: return NE;
	case NE: return E;
	case E: return SE;
	case SE: return S;
	case S: return SW;
	case SW: return W;
	case W: return NW;
	case NW: return N;
	default:return N;
	}
}

/*
	Закрашивает пространство внутри внешннего контура пустым цветом 0xFF.
	Если внутренний контур не пустой то восстанавливаем содержимое внутреннего контура
*/
void Level::EraseShape(Contour* externalContour, Contour*  internalContour)
{
	if (!externalContour)
		throw std::invalid_argument("Pointer to externalContour is null");

	FillContour(externalContour, EMPTY_COLOR);
	if (internalContour)
		RestoreContourContent(internalContour);
}

void Level::FillLine(Contour* externalContour, int startPointNumber, Contour::SearchNearestPointDirection direction, unsigned char color)
{
	int startX = 0;
	int endX = 0;
	Point* startPoint = externalContour->GetPoint(startPointNumber);
	Point* endPoint = nullptr;

	// Ищем точку на противоположной стороне внешнего контура
	endPoint = externalContour->GetNearestContourPoint(startPointNumber, direction);
	if (endPoint)
	{
		if (startPoint->X <= endPoint->X)
		{
			startX = startPoint->X;
			endX = endPoint->X;
		}
		else
		{
			startX = endPoint->X;
			endX = startPoint->X;
		}
		for (int x = startX; x <= endX; x++)
			m_pBuffer[startPoint->Y * m_Width + x] = color;
	}
}

void Level::RestoreLine(Contour* externalContour, int startPointNumber, Contour::SearchNearestPointDirection direction)
{
	int startX = 0;
	int endX = 0;
	Point* startPoint = externalContour->GetPoint(startPointNumber);
	Point* endPoint = nullptr;

	// Ищем точку на противоположной стороне внешнего контура
	endPoint = externalContour->GetNearestContourPoint(startPointNumber, direction);
	if (endPoint)
	{
		if (startPoint->X <= endPoint->X)
		{
			startX = startPoint->X;
			endX = endPoint->X;
		}
		else
		{
			startX = endPoint->X;
			endX = startPoint->X;
		}
		for (int x = startX; x <= endX; x++)
			RestorePixel(x, startPoint->Y);
	}

}

void Level::RestorePixel(int x, int y)
{
	int offset = y * m_Width + x;
	m_pBuffer[offset] = m_pShapesBuffer[offset];
}

/* Проверяет пиксели лежащие на границе отстоящей от центрального пикселя
 заданного координатами x,y на расстояние size.
 Возвращает
		true - если все пиксели границы одного цвета
		false - если пиксели границы отличаются по цвету
*/
bool Level::BorderHasOnlyOneColor(int x, int y, int size)
{
	int left_x = x - size;
	int right_x = x + size;
	int top_y = y - size;
	int bottom_y = y + size;

	unsigned char baseColor = 0xFF;// m_Color;
	// Проверим верхнюю границу
	for (int x = left_x; x <= right_x; x++)
	{
		unsigned char color = GetPixel(x, top_y);
		if (color != baseColor) return false;
	}

	// Проверим нижнюю границу
	for (int x = left_x; x <= right_x; x++)
	{
		unsigned char color = GetPixel(x, bottom_y);
		if (color != baseColor) return false;
	}

	// Проверим левую границу
	for (int y = top_y + 1; y < bottom_y; y++)
	{
		unsigned char color = GetPixel(left_x, y);
		if (color != baseColor) return false;
	}
	// Проверим правую границу
	for (int y = top_y + 1; y < bottom_y; y++)
	{
		unsigned char color = GetPixel(right_x, y);
		if (color != baseColor) return false;
	}
	return true;
}

#pragma region Debug functions section 

void Level::ExpandLevelData(int width, int height, unsigned char color, unsigned char* inBuffer, unsigned char* outBuffer)
{
	for (int i = 0; i < width * height * 4; i++)
		outBuffer[i] = 0xFF;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int inBufferOffset = y * width + x;
			unsigned char bufferPixelColor = inBuffer[inBufferOffset];
			if (bufferPixelColor == color)
			{
				int PixelBufferOffset = inBufferOffset * 4;
				outBuffer[PixelBufferOffset] = color;
				outBuffer[PixelBufferOffset + 1] = color;
				outBuffer[PixelBufferOffset + 2] = color;
				outBuffer[PixelBufferOffset + 3] = 0xFF;
			}
		}
	}

	unsigned char bbb;
	for (int i = 0; i < width * height * 4; i++)
		bbb = outBuffer[i];
}
bool Level::CompareLevelDataWithReferenceData(unsigned char* pReferenceData, wchar_t* message, int messageLength)
{
	for (int i = 0; i < m_BufferLength; i++)
		if (m_pBuffer[i] != pReferenceData[i])
		{
			swprintf(message, messageLength, L"Data at offset %i isn't equal to reference. Data value is %i. Should be %i",
				i, m_pBuffer[i], pReferenceData[i]);
			return false;
		}
	return true;

}

#pragma endregion

