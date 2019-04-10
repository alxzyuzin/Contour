#include "pch.h"
#include "Level.h"

using namespace ContourHelpers;

Level::Level(int width, int height, unsigned char levelColor, unsigned char* pPixelBuffer)
{
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
	//for (int i = 0; i < m_BufferLength; i++)
	//	m_pBuffer[i] = 0xFF;

	for (int y = 0; y < m_Height; y++)
	{
		for (int x = 0, k = 0; x < (m_Width * 4); x += 4, k++)
		{
			unsigned char bitmapPixelColor = pPixelBuffer[y * (m_Width * 4) + x];
			int levelOffset = y * m_Width + k;
			m_pBuffer[y * m_Width + k] = (bitmapPixelColor == levelColor) ? levelColor : 0xFF;
		}
	}

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
	return m_pBuffer[y * m_Width + x];
}

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

void Level::Outline()
{
	FindInternalContours(nullptr, m_Color);
}

void Level::FindInternalContours(Contour* parentContour, unsigned char shapeColor)
{
	while (true)
	{
		Contour* contour = FindContour(parentContour, shapeColor);
		if (contour == nullptr)
			break;
		// Найдём внутренние контуры нового контура
		unsigned char newShapeColor = (shapeColor == m_Color) ? 0xFF : shapeColor;
		FindInternalContours(contour, newShapeColor);
		RemoveShape(contour);
		m_Contours.push_back(contour);
	} 
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
bool Level::FindFirstContourPoint(Contour* parentContour, Point& point, unsigned char shapeColor)
{
	if (parentContour == nullptr)
	{
		// parentCountour - рамка изображения
		for (int y = 0; y < m_Height; y++)
			for (int x = 0; x < m_Width; x++)
			{
				unsigned char c = m_pBuffer[y * m_Width + x]; //
				if (m_pBuffer[y * m_Width + x] == shapeColor)
				{
					point.X = x;
					point.Y = y;
					return true;
				}
			}
		return false;
	}
	else
		// Ищем контур внутри контура parentCountour
	{
		for (int i = (parentContour->Size() - 1); i >= 0; i--) // обходим контур против часовой стрелки
		{
			Point* StartPoint = parentContour->GetPoint(i);
			Point* EndPoint =	parentContour->FindRightNearestPoint(i);
			if (EndPoint)
			{
				for (int x = StartPoint->X + 1; x < EndPoint->X; x++)
				{
					unsigned char c = m_pBuffer[StartPoint->Y * m_Width + x];
					if (m_pBuffer[StartPoint->Y * m_Width + x] == shapeColor)
					{
						point.X = x;
						point.Y = StartPoint->Y;
						return true;
					}
				}
			}

		}
	}
	return false;
}

/*
Находит следующую точку контура
Если такая точка найдена то создаёт новый объект Point и возвращает указатель на него
Если точка не найдена то возвращает nullptr
*/
bool Level::FindNextContourPoint(Contour* parentContour, Point& currentPoint, Direction direction, unsigned char shapeColor)
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

	
	if (parentContour == nullptr)
	{
		// Проверяем не выходит ли точка за границы изображения
		if (y >= 0 && y < m_Height && x >= 0 && x < m_Width && (GetPixel(x, y) == shapeColor))
		{
			currentPoint.X = x;
			currentPoint.Y = y;
			return true;
		}
		else
			return false;
	}
	else
	{
		if (parentContour->ContainPoint(x,y))
			return false;
//		if (!parentContour->EnclosePoint(x,y))
//			return nullptr;
		if (GetPixel(x, y) != shapeColor)
			return false;

		currentPoint.X = x;
		currentPoint.Y = y;
		return true;

	}
	return false;
}

Contour* Level::FindContour(Contour* parentContour, unsigned char shapeColor)
{
	Point firstPoint(0,0);
	bool firstPointFound = FindFirstContourPoint(parentContour, firstPoint, shapeColor);
	
	if (!firstPointFound)
		return nullptr;

	Contour* contour = new Contour();
	contour->AddPoint(firstPoint);

	Point nextPoint = firstPoint;

	Direction searchDirection = Direction::E;
	while (true)
	{
		searchDirection = StartDirection(searchDirection);

		int l;
		for (l = 0; l < 8; l++)
		{
			bool nextPointFound = FindNextContourPoint(parentContour, nextPoint, searchDirection, shapeColor);
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
	// Если контур состоит из одного пикселя с цветом 0xFF, то  не считаем его контуром и возвращаем nullptr
	// Область занимаемую этим пикселем стирать не нужно это и так пустое место
	//if (contour->Size() == 1 && GetPixel(firstPoint.X, firstPoint.Y) == 0xFF)
	//	return nullptr;
	return contour;
}

void Level::FindAllContours()
{
	Contour* contour;
	do
	{
		contour = FindExternalContour(nullptr);
		if (contour->Size() > 0)
		{
			RemoveShape(contour);
			m_Contours.push_back(contour);
		}
	} while (contour->Size() > 0);
}

//----------------------------------------------------------------------------
// Находит граничную точку первой областе для оконтуривания
//----------------------------------------------------------------------------

Point* Level::FindFirstExternalContourPoint(Contour* parentContour)
{
	for (int y = 0; y < m_Height; y++)
		for (int x = 0; x < m_Width; x++)
		{
			unsigned char c = m_pBuffer[y * m_Width + x];
			if (m_pBuffer[y * m_Width + x] == m_Color)
				return new Point(x,y);
		}
	return nullptr;
}

//----------------------------------------------------------------------------
// Извлекает первый контур в слое
//----------------------------------------------------------------------------
Contour* Level::FindExternalContour(Contour* parentContour)
{
	Contour* contour =  new Contour();
	Point* firstPoint = FindFirstExternalContourPoint(nullptr);
	Point* currentPoint = firstPoint;

	if (firstPoint)
	{
		contour->AddPoint(*firstPoint);
		
		Point *nextPoint;
		Direction searchDirection = Direction::E;
		while (1)
		{
			searchDirection = StartDirection(searchDirection);
			nextPoint = nullptr;

			int l;
			for (l = 0; l < 8; l++)
			{
				nextPoint = FindNextExternalContourPoint(currentPoint, searchDirection);
				if (nextPoint)
				{
					if (GetPixel(nextPoint) == m_Color)
					{
						if ((nextPoint->X == firstPoint->X) && (nextPoint->Y == firstPoint->Y))
							return contour;
						else
						{
							currentPoint = nextPoint;
							// возможно здесь происходит копирование структуры из указателя в vector
							// тогда нужно освобождать память выделенную исходной структуре
							contour->AddPoint(*currentPoint);
							break;
						}
					}
					else
						delete(nextPoint);
				}
				searchDirection = NextDirection(searchDirection);
			}
			if (l==8)  // сделали полный круг и не нашли продолжения контура
				break; // контур состоит из одного пикселя
		}
	}
	return contour;
}

//----------------------------------------------------------------------------
// Находит следующую точку контура по отношению к заданной, 
// начиная поиск новой точки с заданного направления
// Параметры
//		currentPoint - текущая точка
//		startDirection  - направление от текущей точки в котором начинать поиск
//						следующей точки контура
//----------------------------------------------------------------------------

Point* Level::FindNextExternalContourPoint(Point* currentPoint, Direction startDirection)
{
	Direction direction = startDirection;
	//bool nextPixelFound = false;
	Point* newPoint = nullptr;

	int Width = m_Width - 1;
	int Height = m_Height - 1;

	switch (direction)
	{
		case N:
			if (currentPoint->Y > 0)
				newPoint = new Point(currentPoint->X, currentPoint->Y-1);
			break;
		case NE:
			if (currentPoint->X < Width  && currentPoint->Y > 0)
				newPoint = new Point(currentPoint->X+1, currentPoint->Y - 1);
			break;
		case E:
			if (currentPoint->X < Width)
				newPoint = new Point(currentPoint->X + 1, currentPoint->Y);
			break;
		case SE:
			if (currentPoint->X < Width && currentPoint->Y < Height)
				newPoint = new Point(currentPoint->X + 1, currentPoint->Y + 1);
			break;
		case S:
			if (currentPoint->Y < Height)
				newPoint = new Point(currentPoint->X, currentPoint->Y + 1);
			break;
		case SW:
			if (currentPoint->X > 0 && currentPoint->Y < Height)
				newPoint = new Point(currentPoint->X - 1, currentPoint->Y + 1);
			break;
		case W:
			if (currentPoint->X > 0)
				newPoint = new Point(currentPoint->X - 1, currentPoint->Y);
			break;
		case NW:
			if (currentPoint->X > 0 && currentPoint->Y > 0)
				newPoint = new Point(currentPoint->X - 1, currentPoint->Y - 1);
			break;
	}
	if (newPoint)
	{
		if (GetPixel(newPoint) == m_Color)
				return newPoint;
	}
	return nullptr;
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

//----------------------------------------------------------------------------
// Сравнивает точки по координате Y
//----------------------------------------------------------------------------

int Level::comparePoints(const void * a, const void * b)
{
	
	if ( (*(Point*)a).Y <  (*(Point*)b).Y ) return -1;
	if ( (*(Point*)a).Y == (*(Point*)b).Y ) return  0;
	if ( (*(Point*)a).Y >  (*(Point*)b).Y ) return  1;
	return 0;
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


inline void Level::DrawHorizontalLine(int x1, int x2, int y, unsigned char color)
{
//	if (x1 > x2)
//	{ 	int x = x1; x1 = x2; x2 = x; }
	
	int s = y * m_Width + x1;
	for (int x = x1; x < x2; x++)
			m_pBuffer[s + x] = color;
}


void Level::Rectify(int size)
{
	for (int y = size; y < m_Height - 2*size; y++)
	{
		for (int x = size; x < m_Width - 2*size; x++)
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

// Проверяет пиксели лежащие на границе отстоящей от центрального пикселя
// заданного координатами x,y на расстояние size. 
// Возвращает 
//		true - если все пиксели границы одного цвета
//		false - если пиксели границы отличаются по цвету

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