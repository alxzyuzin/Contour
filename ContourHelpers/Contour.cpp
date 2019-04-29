#include "pch.h"
#include "Contour.h"

using namespace std;


namespace ContourHelpers
{

	/*bool Point::operator==(const Point& p1, const Point& p2)
	{
		return p1.X == p2.X && p1.Y == p2.Y;
	}*/

	//	bool Point::operator ==(const Point& point) {
	//		if (this->X == point.X && this->Y == point.Y)
	//			return true;
	//		return false;
	//	}

	Contour::Contour()
	{

	}

	Contour::Contour(unsigned char color)
	{
		m_Color = color;
	}

	Contour::Contour(Point* contourPoints, int pointsNumber)
	{
		for (int i = 0; i < pointsNumber; i++)
			AddPoint(contourPoints[i]);
	}

	Contour::~Contour()
	{

	}

	Point* Contour::operator[](unsigned int i)
	{
		if (i > m_Points.size()) {
			return &m_Points[0];
		}
		return &m_Points[i];
	}

	void Contour::AddPoint(Point point)
	{
		m_Points.push_back(point);
		++Length;
	}

	Point* Contour::GetPoint(int i)
	{
		return &m_Points[i];
	}

	unsigned char Contour::GetColor()
	{
		return m_Color;
	}


	int Contour::Size()
	{
		return Length;
	}

	/* Находит ближайшую слева точку контура лежащую в той же строке что и заданная
----------------------------------------------------------------------------*/
	Point* Contour::FindLeftNearestPoint(int pointnumber)
	{
		// Найдём ближайжую слева точку контура лежащую в той же строке что и точка контура с номером pointnumber
		int lastDistance = MININT;
		Point *p = nullptr;
		for (unsigned int l = 0; l < m_Points.size(); l++)
		{
			if (l == pointnumber)
				continue;
			if (m_Points[l].Y == m_Points[pointnumber].Y)
			{
				int newDistance = m_Points[l].X - m_Points[pointnumber].X;
				if (newDistance <= 0 && newDistance > lastDistance)
				{
					lastDistance = newDistance;
					p = &m_Points[l];
				}
			}
		}
		return p;
	}

	Point* Contour::FindRightNearestPoint(int pointnumber)
	{
		// Найдём точку контура ближайшую к точке с номером pointnumber лежащую справа в той же строке что и p2.  
		int lastDistance = MAXINT;
		Point *p = nullptr;
		for (unsigned int l = 0; l < m_Points.size(); l++)
		{
			if (l == pointnumber)
				continue;
			if (m_Points[l].Y == m_Points[pointnumber].Y)
			{
				int newDistance = m_Points[l].X - m_Points[pointnumber].X;
				if (newDistance >= 0 && newDistance < lastDistance)
				{
					lastDistance = newDistance;
					p = &m_Points[l];
				}
			}
		}
		return p;
	}


	/*
	Находит точку контура ближайшую к точке с координатами point.X, point.Y
	лежащую справа в той же строке что и point.  
	*/
	Point* Contour::GetRightNearestContourPoint(Point* point)
	{
		int lastDistance = MAXINT;
		Point *p = nullptr;
		for (unsigned int i = 0; i < this->Length; i++)
		{
			if (m_Points[i].Y == point->Y)
			{
				if (m_Points[i].X == point->X)
					continue;

				int newDistance = m_Points[i].X - point->X;
				if (newDistance >= 0 && newDistance < lastDistance)
				{
					lastDistance = newDistance;
					p = &m_Points[i];
				}
			}
		}
		return p;
	}

	/*
		Находит точку контура ближайшую к точке с координатами point.X, point.Y
		лежащую слева в той же строке что и point.
	*/
	Point* Contour::GetLeftNearestContourPoint(Point* point)
	{

		int lastDistance = MAXINT;
		Point *p = nullptr;
		for (unsigned int i = 0; i < this->Length; i++)
		{
			if (m_Points[i].Y == point->Y)
			{
				if (m_Points[i].X == point->X)
					continue;

				int newDistance = m_Points[i].X - point->X;
				if (newDistance <= 0 && newDistance > lastDistance)
				{
					lastDistance = newDistance;
					p = &m_Points[i];
				}
			}
		}
		return p;
	}

	Point* Contour::GetNearestContourPoint(Point* point, Contour::SearchNearestPointDirection direction)
	{
		int lastDistance = MAXINT;
		Point *p = nullptr;
		for (unsigned int i = 0; i < this->Length; i++)
		{
			if (m_Points[i].Y == point->Y)
			{
				if (m_Points[i].X == point->X)
					continue;

				int newDistance = m_Points[i].X - point->X;
				switch (direction)
				{
				case Right:
					if (newDistance >= 0 && newDistance < lastDistance)
					{
						lastDistance = newDistance;
						p = &m_Points[i];
					}
				case Left:
					if (newDistance <= 0 && newDistance > lastDistance)
					{
						lastDistance = newDistance;
						p = &m_Points[i];
					}
				}
			}
		}
		return p;
	}

	Point* Contour::GetNearestContourPoint(int pointIndex, SearchNearestPointDirection direction)
	{
		int lastDistance = direction == Right ? MAXINT : MININT;
		Point *p = nullptr;
		for (unsigned int i = 0; i < this->Length; i++)
		{
			if (m_Points[i].Y == m_Points[pointIndex].Y)
			{
				if (m_Points[i].X == m_Points[pointIndex].X)
					continue;

				int newDistance = m_Points[i].X - m_Points[pointIndex].X;
				switch (direction)
				{
				case Right:
					if (newDistance >= 0 && newDistance < lastDistance)
					{
						lastDistance = newDistance;
						p = &m_Points[i];
					}
				case Left:
					if (newDistance <= 0 && newDistance > lastDistance)
					{
						lastDistance = newDistance;
						p = &m_Points[i];
					}
				}
			}
		}
		return p;
	}

	Point* Contour::GetNearestInternalContourPoint(Point* externalContourPoint, SearchNearestPointDirection direction)
	{
		int lastDistance = direction == Right ? MAXINT : MININT;
		Point *p = nullptr;
		for (int i = 0; i < Length; i++)
		{
			if (m_Points[i].Y == externalContourPoint->Y)
			{
				int newDistance = m_Points[i].X - externalContourPoint->X;
				switch (direction)
				{
				case Right:
					if (newDistance >= 0 && newDistance < lastDistance)
					{
						lastDistance = newDistance;
						p = &m_Points[i];
					}
				case Left:
					if (newDistance <= 0 && newDistance > lastDistance)
					{
						lastDistance = newDistance;
						p = &m_Points[i];
					}
				}
			}
		}
		return p;
	}

	/*
	  Находит ближайшую по горизонтали точку контура лежащую справа от точки,
	  заданой координатами x, y
	  Входные параметры
			x, y - координаты точки для которой нужно найти
					ближайшую справа точку контура
	  Возвращаемое значение
			номер точки если она найдена
			-1 если точка не найдена
	*/
	int Contour::GetRightNearestPointIndex(int x, int y)
	{
		int index = -1;
		int lastDistance = MAXINT;

		for (int i = 0; i < Length; i++)
		{
			if (m_Points[i].Y == y)
			{
				// Исключим из рассмотрения точку с координатами x,y
				if (m_Points[i].X == x)
					continue;

				int newDistance = m_Points[i].X - x;
				if (newDistance >= 0 && newDistance < lastDistance)
				{
					lastDistance = newDistance;
					index = i;
				}
			}
		}
		return index;
	}

	/*
	Возвращает индекс следующей точки контура
	Отсчёт идёт по часовой стрелке
	*/
	int Contour::GetNextContourPointIndex(int i)
	{
		return ++i % Length;
	}

	/*
	 Возвращает индекс предыдущей точки контура
	 Отсчёт идёт по часовой стрелке
	*/
	int Contour::GetPrevContourPointIndex(int i)
	{
		return (--i + Length) % Length;
	}


	bool Contour::Contains(Point* point)
	{
		for (Point p : m_Points)
		{
			if (p.X == point->X && p.Y == point->Y)
				return true;
		}
		return false;
	}

	//Point* Contour::GetPrevContourPoint(Point* point)
	//{
	//	//m_Points.
	//}

	//Point* Contour::GetNextContourPoint(Point* point)
	//{

	//}


	
	/*
	Проверяет лежит ли заданная точка внутри контура
	Входные параметры
		x, y - координаты проверяемой точки
	Возвращаемое значение
		true - если точка лежит внутри контура
		false -  если точка является точкой контура или лежит вне контура
	*/
	bool Contour::ContainsPoint(int x, int y)
	{
		int contourCrossingCount = 0;
		Point currentPoint = Point(x, y);
		// Проверим принадлежность точки контуру
		if (Contains(&currentPoint))
			return false;

		int rightNearestPointIndex = GetRightNearestPointIndex(x, y);
		// Если не найдено ни одной точки справа, то текущая точка лежит вне контура
//		if (rightNearestPointIndex < 0)
//			return false;
		while (rightNearestPointIndex >= 0)
		{
			// Получим индексы точек перед и после ближайшей точки справа 
			int prevPointIndex = GetPrevContourPointIndex(rightNearestPointIndex);
			int nextPointIndex = GetNextContourPointIndex(rightNearestPointIndex);
			// Выберем из них точку которая лежит левее 
			int leftNeiborPointIndex = 0;
			int rightNeiborPointIndex = 0;
		
			int yDistance = abs(m_Points[prevPointIndex].Y - m_Points[nextPointIndex].Y);
			switch (yDistance)
			{
			case 0:
				x = m_Points[rightNearestPointIndex].X;
				break;
			case 1:
				if (m_Points[prevPointIndex].Y == m_Points[rightNearestPointIndex].Y)
				{
					rightNeiborPointIndex = prevPointIndex;
					leftNeiborPointIndex = nextPointIndex;
					while (m_Points[rightNeiborPointIndex].Y == y)
						rightNeiborPointIndex = GetPrevContourPointIndex(rightNeiborPointIndex);
				}
				if (m_Points[nextPointIndex].Y == m_Points[rightNearestPointIndex].Y)
				{
					rightNeiborPointIndex = nextPointIndex;
					leftNeiborPointIndex = prevPointIndex;
					while (m_Points[rightNeiborPointIndex].Y == y)
						rightNeiborPointIndex = GetNextContourPointIndex(rightNeiborPointIndex);
				}
				if (m_Points[leftNeiborPointIndex].Y != m_Points[rightNeiborPointIndex].Y)
					++contourCrossingCount;
				x = m_Points[rightNeiborPointIndex].X;
				break;
			case 2:
				++contourCrossingCount;
				x = m_Points[rightNearestPointIndex].X;
				break;
			}
			rightNearestPointIndex = GetRightNearestPointIndex(x, y);
		}

		return contourCrossingCount % 2 == 1 ? true : false;
	}
	
	/////*
	////	Проверяет принадлежит ли заданная точка контуру
	////	Возвращаемое значение
	////		true -  если точка является точкой контура
	////		false - если точка не является точкой контура
	////*/
	////bool Contour::ContainsPoint(Point* point)
	////{
	////	for (Point contourPoint : m_Points)
	////	{
	////		if (contourPoint == *point)
	////			return false;
	////	}
	////	int i = 0;
	////	Point* rightNearestPoint = FindRightNearestPoint(point);
	////	while (rightNearestPoint)
	////	{
	////		++i;
	////		rightNearestPoint = FindRightNearestPoint(rightNearestPoint);
	////	}
	////	// Если количество точек контура справа от заданной точки чётное,
	////	// то точка лежит вне контура. Иначе внутри контура
	////	return i % 2 == 0 ? false : true;
	////}
	///*
	//	Проверяет лежит ли заданная точка внутри контура
	//	Возвращаемое значение 
	//		true -  если точка лежит внутри контура или принадлежит контуру
	//		false - если точка лежит вне контура
	//*/
	//bool Contour::EnclosePoint(Point* point)
	//{
	//	int i = 0;
	//	Point* rightNearestPoint = FindRightNearestPoint(point);
	//	while (rightNearestPoint)
	//	{
	//		++i;
	//		rightNearestPoint = FindRightNearestPoint(rightNearestPoint);
	//	}
	//	// Если количество точек контура справа от заданной точки чётное,
	//	// то точка лежит вне контура. Иначе внутри контура
	//	return i % 2 == 0 ? false : true;
	//}

	int Contour::GetMinY()
	{
		int y = MAXINT;
		for (Point point : m_Points)
			y = (point.Y < y) ? point.Y : y;
		return y;
	}

	int Contour::GetMaxY()
	{
		int y = 0;
		for (Point point : m_Points)
			y = (point.Y > y) ? point.Y : y;
		return y;
	}

	Point* Contour::GetMostLeftContourPoint(int y)
	{
		int x = MAXINT;
		Point* MostLeftPoint = nullptr;
		for (int i=0; i< m_Points.size(); i++)
		{
			if (y == m_Points[i].Y && m_Points[i].X < x)
			{
				x = m_Points[i].X;
				MostLeftPoint = &m_Points[i];
			}
		}
		return MostLeftPoint;
	}
	
}  // namespace ContourHelpers
