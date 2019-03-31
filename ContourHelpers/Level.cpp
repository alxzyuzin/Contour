﻿#include "pch.h"
#include "Level.h"
//#include "Outline.h"

using namespace ContourHelpers;


Level::Level(int width, int height, byte levelColor, byte* pPixelBuffer)
{
	m_Width = width;
	m_Height = height;
	m_Color = levelColor;
	m_BufferLength = width * height;
	m_pBuffer = new byte[m_BufferLength];
	m_pShapesBuffer = new byte[m_BufferLength];
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
			//int pixelBufferOffset = y * (m_Width * 4) + x;
			byte bitmapPixelColor = pPixelBuffer[y * (m_Width * 4) + x];
			int levelOffset = y * m_Width + k;

			if (bitmapPixelColor == levelColor)
				SetPixel(levelOffset, levelColor);
		}
	}

	for (int i=0; i< m_BufferLength; i++)
		m_pShapesBuffer[i] = m_pBuffer[i];
	
}

Level::~Level()
{
	for (auto contour : m_Contours)
	{
		for (Point * point : *contour)
			delete(point);
		delete contour;
	}

	delete[] m_pShapesBuffer;
	delete[] m_pBuffer;
}

void Level::Clear()
{
	
	for (auto contour : m_Contours)
	{
		for (Point * point : *contour)
			delete(point);
		delete contour;
	}
	
	delete[] m_pBuffer;
	m_BufferLength = 0;
	m_Width = 0;
	m_Height = 0;
	m_Color =0;
}

void Level::SetPixel(int position, byte color)
{
	m_pBuffer[position] = color;
}

byte Level::GetPixel(int position)
{
	return m_pBuffer[position];
}

void Level::SetPixel(Point* point, byte color)
{
	m_pBuffer[point->Y * m_Width + point->X] = color;
}

byte Level::GetPixel(Point* point)
{
	return m_pBuffer[point->Y * m_Width + point->X];
}

void Level::SetPixel(int x, int y, byte color)
{
	m_pBuffer[y * m_Width + x] = color;
}

byte Level::GetPixel(int x, int y)
{
	return m_pBuffer[y * m_Width + x];
}


void Level::GetLevelShapes(byte* pPixelBuffer)
{
	for (int j = 0; j < m_Height; j++)
	{
		for (int i = 0, k = 0; i < (m_Width * 4); i += 4, k++)
		{
			int PixelBufferOffset = j * (m_Width * 4) + i;
			byte pixelColor = m_pShapesBuffer[j * m_Width + k];
			if (pixelColor == m_Color)
			{
				pPixelBuffer[PixelBufferOffset] = pixelColor;
				pPixelBuffer[PixelBufferOffset + 1] = pixelColor;
				pPixelBuffer[PixelBufferOffset + 2] = pixelColor;
				pPixelBuffer[PixelBufferOffset + 3] = 0xFF;
			}
		}
	}


}

void Level::FindAllContours()
{
	vector<Point*>* contour = new vector<Point*>();
	do
	{
		contour = FindContour();
		if (contour->size() > 0)
		{
			RemoveShape(contour);
			m_Contours.push_back(contour);
		}

	} while (contour->size() > 0);

}


//----------------------------------------------------------------------------
// Находит граничную точку первой областе для оконтуривания
//----------------------------------------------------------------------------

Point* Level::FindFirstContourPoint()
{
	for (int y = 0; y < m_Height; y++)
		for (int x = 0; x < m_Width; x++)
		{
			if (m_pBuffer[y * m_Width + x] == m_Color)
				return new Point(x,y);
		}
	return nullptr;
}

//----------------------------------------------------------------------------
// Извлекает первый контур в слое
//----------------------------------------------------------------------------
std::vector<Point*>* Level::FindContour()
{
	vector<Point*>* contour =  new vector<Point*>();
	Point* firstPoint = FindFirstContourPoint();
	Point* currentPoint = firstPoint;

	if (firstPoint)
	{
		contour->push_back(firstPoint);
		
		Point *checkPoint;
		Direction searchDirection = Direction::E;
		while (1)
		{
			searchDirection = StartDirection(searchDirection);
			checkPoint = nullptr;

			int l;
			for (l = 0; l < 8; l++)
			{
				checkPoint	= FindNextContourPoint(currentPoint, searchDirection);
				if (checkPoint)
				{
					if (GetPixel(checkPoint) == m_Color)
					{
						if ((checkPoint->X == firstPoint->X) && (checkPoint->Y == firstPoint->Y))
							return contour;
						else
						{
							currentPoint = checkPoint;
							// возможно здесь происходит копирование структуры из указателя в vector
							// тогда нужно освобождать память выделенную исходной структуре
							contour->push_back(currentPoint);
							break;
						}
					}
					else
						delete(checkPoint);
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

Point* Level::FindNextContourPoint(Point* currentPoint, Direction startDirection)
{
	Direction direction = startDirection;
	bool nextPixelFound = false;
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
//
//----------------------------------------------------------------------------
void Level::RemoveShape(vector<Point*>* contour)
{
	byte baseColor = 0xFF;
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

	for (unsigned int i = 0; i < contour->size() - 1; i++)
	{
		p1 = (*contour)[i];
		p2 = (*contour)[i+1];
		if (p2->Y >= p1->Y)
		{
			p3 = FindLeftNearestPoint(contour, i + 1);
			if (!p3)
				continue;
			for (int x = p2->X; x >= p3->X; x--)
				SetPixel(x, p2->Y, baseColor);
		}
		
		if (p2->Y <= p1->Y)
		{
			p3 = FindRightNearestPoint(contour, i + 1);
			if (!p3)
				continue;
			for (int x = p2->X; x <= p3->X; x++)
				SetPixel(x, p2->Y, baseColor);
		}
	}

	for (unsigned int i = 0; i < contour->size(); i++)
		SetPixel((*contour)[i], baseColor);
	std::clock_t end = std::clock();

double time = (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000); // 119 sec//153072ms
}

/* Находит ближайшую слева точку контура лежащую в той же строке что и заданная 
----------------------------------------------------------------------------*/
Point* Level::FindLeftNearestPoint(vector<Point*>* contour, int pointnumber)
{
	// Найдём ближайжую слева точку контура лежащую в той же строке что и p2
	int lastDistance = -m_Width;
	Point *p = nullptr;
	for (unsigned int l = 0; l < contour->size(); l++)
	{
		if (l == pointnumber)
			continue;
		if ((*contour)[l]->Y == (*contour)[pointnumber]->Y)
		{
			int newDistance = (*contour)[l]->X - (*contour)[pointnumber]->X;
			if (newDistance <= 0 && newDistance > lastDistance)
			{
				lastDistance = newDistance;
				p = (*contour)[l];
			}
		}
	}
	return p;
}

Point* Level::FindRightNearestPoint(vector<Point*>* contour, int pointnumber)
{
	// Найдём точку контура ближайжую к точке с номером pointnumber лежащую справа в той же строке что и p2.  
	int lastDistance = m_Width;
	Point *p = nullptr;
	for (unsigned int l = 0; l < contour->size(); l++)
	{
		if (l == pointnumber)
			continue;
		if ((*contour)[l]->Y == (*contour)[pointnumber]->Y)
		{
			int newDistance = (*contour)[l]->X - (*contour)[pointnumber]->X;
			if (newDistance >= 0 && newDistance < lastDistance)
			{
				lastDistance = newDistance;
				p = (*contour)[l];
			}
		}
	}
	return p;
}

inline void Level::DrawHorizontalLine(int x1, int x2, int y, byte color)
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
				byte centralColor = GetPixel(x, y);
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

	byte baseColor = 0xFF;// m_Color;
	// Проверим верхнюю границу
	for (int x = left_x; x <= right_x; x++)
	{
		byte color = GetPixel(x, top_y);
		if (color != baseColor) return false;
	}
	
	// Проверим нижнюю границу
	for (int x = left_x; x <= right_x; x++)
	{
		byte color = GetPixel(x, bottom_y);
		if (color != baseColor) return false;
	}
	
	// Проверим левую границу
	for (int y = top_y + 1; y < bottom_y; y++)
	{
		byte color = GetPixel(left_x, y);
		if (color != baseColor) return false;
	}
	// Проверим правую границу
	for (int y = top_y + 1; y < bottom_y; y++)
	{
		byte color = GetPixel(right_x, y);
		if (color != baseColor) return false;
	}
	return true;
}