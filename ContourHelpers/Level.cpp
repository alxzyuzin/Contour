#include "pch.h"
#include "Level.h"

using namespace ContourHelpers;

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
	for (int i=0; i< m_BufferLength; i++)
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
	m_Color =0;
}


void Level::Outline()
{
//	FindInternalContours(nullptr, m_Color);
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
							SetPixel(i, j, 0xFF);
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
				pPixelBuffer[PixelBufferOffset + 3] = 0xFF;
			}
		}
	}
}





void Level::SetPixel(int position, unsigned char color)
{
	m_pBuffer[position] = color;
}

unsigned char Level::GetPixel(int position)
{
	return m_pBuffer[position];
}

void Level::SetPixel(Point* point, unsigned char color)
{
	m_pBuffer[point->Y * m_Width + point->X] = color;
}

unsigned char Level::GetPixel(Point* point)
{
	return m_pBuffer[point->Y * m_Width + point->X];
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
IN parameters 
	parentContour - function looking for first point of new contour inside parentContour.
					If parentContour = nullptr it means that we start search most top level contours.
	shapeColor	  - color of shape being contoured
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
Входные параметры
	parentContour -  контур внутри которого выполняется поиск закрашенной области
					для оконтуривания
	shapeColor -  цвет которым закрашена оконтуриваемая область

Возвращаемое значение

	Если контур найден - указатель на объект класса Contour, содержащий список
	точек контура иначе nullptr

Доработки
	Убрать параметр shapeColor - он всегда равен значению переменной m_Color

	Попробовать прекращать поиск точек контура как только новая точка
	совпадёт с одной из найденных точек контура
	В этом случае контура без внутренних точек будут короче и возможно поиск
	точек контура будет заканчиваться быстрее.
*/
Contour* Level::FindExternalContour(Contour* parentContour, unsigned char shapeColor)
{
	Point firstPoint(0,0);
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
				if (GetPixel(nextPoint.X, nextPoint.Y) == shapeColor)
				{
					if ((nextPoint.X == firstPoint.X) && (nextPoint.Y == firstPoint.Y))
						return contour;
					else
					{
						contour->AddPoint(nextPoint);
						break;
					}
				}
			}
			searchDirection = NextDirection(searchDirection);
		}
		if (l == 8)  // сделали полный круг и не нашли продолжения контура
			break; // контур состоит из одного пикселя
	}
	return contour;
}

bool Level::FindFirstInternalContourPoint(Contour* parentContour, Point& point)
{
	if (!parentContour)
		throw std::invalid_argument("Pointer to parentContour is null");

	// Ищем первую точку не закращенной области внутри контура parentCountour
	// обходим контур против часовой стрелки
	for (int i = 0; i < parentContour->Size(); i++)
	{
		Point* StartPoint = parentContour->GetPoint(i);
		Point* EndPoint = parentContour->FindLeftNearestPoint(i);
		if (EndPoint)
		{
			for (int x = EndPoint->X + 1; x < StartPoint->X;  x++)
			{
				unsigned char c = m_pBuffer[StartPoint->Y * m_Width + x];
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
	while (true)
	{
		Contour* externalContour = FindExternalContour(nullptr, m_Color);
		if (!externalContour)
			break;
		m_Contours.push_back(externalContour);
		Contour* internalContour = FindInternalContour(externalContour);
		if (!externalContour)
			m_Contours.push_back(internalContour);

		EraseShape(externalContour, internalContour);
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

Direction Level::PrevDirection(Direction direction)
{
	switch (direction)
	{
		case N: return NW;
		case NE: return N;
		case E: return NE;
		case SE: return E;
		case S: return SE;
		case SW: return S;
		case W: return SW;
		case NW: return W;
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


// Удаляет фигуру внутри заданного контура (закрашивает белым цветом)
// Закрашивание выполняем по строкам
// Для каждой точки контура находим точку лежащую на противоположной стороне контура
// и рисуем линию от текущей точки до точки на противоположной стороне контура
//
//----------------------------------------------------------------------------
void Level::RemoveShape(Contour* contour)
{
	//byte baseColor = 0xFF;
	unsigned char baseColor = contour->GetColor();

	std::clock_t    start;
	start = std::clock();
	/*
	//
	//// Перенесём точки из контура в массив и отсортируем его по возрастанию Y
	//Pt* sortedPoints = new Pt[contour->Length];

	//int sortedPointsLength = 0;
	//for (Point^ pt : contour->Points)
	//{
	//	sortedPoints[sortedPointsLength].X = pt->X;
	//	sortedPoints[sortedPointsLength].Y = pt->Y;
	//	sortedPointsLength++;
	//}

	//qsort(sortedPoints, sortedPointsLength, sizeof(Pt), comparePoints);

	//std::vector<std::vector<int>> pointIndex;
	//
	//int i = 0;
	//int j = i + 1;

	//do
	//{
	//	std::vector<int> * vector_X = new std::vector<int>;
	//	vector_X.push_back(sortedPoints[i].X);
	//	while (sortedPoints[i].Y == sortedPoints[j].Y);
	//	{
	//		vector_X.push_back(sortedPoints[j].X);
	//		j++;
	//	} 
	//	pointIndex.push_back(vector_X);
	//	i = j;
	//	j++;
	//} while (j<sortedPointsLength);
	//
	//Pt p;
	//
	//for (int j = 0; j < i; j++)
	//{
	//	p.X = sortedPoints[j].X;
	//	p.Y = sortedPoints[j].Y;
	//}
	*/
	Point* p1 = nullptr;
	Point* p2 = nullptr;
	Point* p3 = nullptr;
	
	for (int i = 0; i < contour->Size() - 1; i++)
	{
		// выбираем две последовательные точки контура и определяем направление
		// отрисовки контура по оси Y
		p1 = contour->GetPoint(i);
		p2 = contour->GetPoint(i+1);
		// Если координата Y увеличивается ищем точку на противоположнойстороне контура слева
		if (p2->Y >= p1->Y)
		{
			p3 = contour->FindLeftNearestPoint(i + 1);
			if (!p3)
				continue;
			for (int x = p2->X; x >= p3->X; x--)
				SetPixel(x, p2->Y, baseColor);
		}
		// Если координата Y уменьшается ищем точку на противоположнойстороне контура справ
		if (p2->Y <= p1->Y)
		{
			p3 = contour->FindRightNearestPoint(i + 1);
			if (!p3)
				continue;
			for (int x = p2->X; x <= p3->X; x++)
				SetPixel(x, p2->Y, baseColor);
		}
	}
	// Закрашиваем сам контур 
	for (int i = 0; i < contour->Size(); i++)
		SetPixel(contour->GetPoint(i), baseColor);
	std::clock_t end = std::clock();

double time = (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000); // 119 sec//153072ms
}

// Закрашивает промежуток между внешним и внутренним контуром пустым цветом 0xFF.
// Закрашивание выполняется по строкам
// выполняем обход внешнего контура по часовой стрелке (в том же направлении в котором искали точки контура)
// Если при переходе от предыдущей точки к текущей координата Y увеличилась или не изменилась 
// то среди точек внешнего и внутреннего контура ищем ближайшую точку СЛЕВА от текущей	
// Если при переходе от предыдущей точки к текущей координата Y уменьшилась или не изменилась
// 	то среди точек внешнего и внутреннего контура ищем ближайшую точку СПРАВА от текущей	
// ищем точку ближайшую точку слева от текущей	

// Линию от текущей точки до найденной ближайшей точки ( справа ли слева ) закрашиваем пустым цветом
//
//----------------------------------------------------------------------------
//enum HorizontalDirection {Left, Right};

void Level::EraseShape(Contour* externalContour, Contour*  internalContour)
{
	std::clock_t    start;
	start = std::clock();
	// минимальная длина контура содержащего внутреннюю область равна 4
	// таким образом дл яконтура с длиной меньше 4 достаточно стереть сам контур
	if (externalContour->Length < 4)
	{
		for (int i = 0; i < externalContour->Length; i++)
			SetPixel(externalContour->GetPoint(i), 0xFF);
		return;
	}

	Contour::SearchNearestPointDirection searchDirection = Contour::SearchNearestPointDirection::Left;

	Point nullPoint = Point(-1, -1);
	Point* p0 = &nullPoint;
	Point* p1 = &nullPoint;
	Point* p2 = externalContour->GetPoint(0);

	SetPixel(externalContour->GetPoint(0), 0xFF);
	for (int i = 1; i < externalContour->Size(); i++)
	{
		p0 = p1;
		p1 = p2;
		p2 = externalContour->GetPoint(i);

		// Закрашиваем сам контур 
		SetPixel(p2, 0xFF);

		if (p1->Y != p2->Y)
		{
			// Если координата Y увеличивается, устанавливаем направление поиска точки на 
			// противоположной стороне контура слева
			if (p2->Y > p1->Y)
				searchDirection = Contour::SearchNearestPointDirection::Left;
			// Если координата Y уменьшается, устанавливаем направление поиска точки на 
			// противоположной стороне контура справа
			if (p2->Y < p1->Y)
				searchDirection = Contour::SearchNearestPointDirection::Right;

			if (p0->Y == p1->Y)
				EraseLine(externalContour, internalContour, i-1, searchDirection);
			EraseLine(externalContour, internalContour, i, searchDirection);
		}
		
	}
	std::clock_t end = std::clock();
	double time = (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000); // 119 sec//153072ms
}

void Level::EraseLine(Contour* externalContour, Contour* internalContour, int startPointNumber, Contour::SearchNearestPointDirection direction)
{
	int startX = 0;
	int endX = 0;
	Point* startPoint = externalContour->GetPoint(startPointNumber);
	Point* endPoint = nullptr;
	// Если существует внутренний контур ищем точку на границе внутреннего контура
	if (internalContour)
		endPoint = internalContour->GetNearestInternalContourPoint(startPoint, direction);
	// Если точку на границе внутреннего контура не найдена ищем точку на 
	// противоположной стороне внешнего контура
	if (!endPoint)
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
			SetPixel(x, startPoint->Y, 0xFF);
	}

}


Point* Level::GetCorrespondingContourPoint(Contour* externalContour, Contour* internalContour, Point* startPoint, Contour::SearchNearestPointDirection searchDirection)
{
	return nullptr;
}

inline void Level::DrawHorizontalLine(int x1, int x2, int y, unsigned char color)
{
//	if (x1 > x2)
//	{ 	int x = x1; x1 = x2; x2 = x; }
	
	int s = y * m_Width + x1;
	for (int x = x1; x < x2; x++)
			m_pBuffer[s + x] = color;
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

