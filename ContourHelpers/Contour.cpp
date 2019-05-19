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

	Contour::Contour(ContourType type)
	{
		Type = type;
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

	/*
		Находит ближайшую слева точку контура лежащую в той же строке что и заданная
	*/
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
		for (int i = 0; i < this->Length; i++)
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
		for (int i = 0; i < this->Length; i++)
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
		for (int i = 0; i < this->Length; i++)
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
		
		int prevPoint = GetNextContourPointIndex(pointIndex);
		int nextPoint = GetPrevContourPointIndex(pointIndex);
		
		// Если предыдущая и следующая точки контура имеют одинаковое значение
		// координаты Y (лежат в одной строке), это означает что точка с номером
		// pointIndex лежит на пике контура обращённом вверх или вниз
		// Если пик обращён внутрь контура то выполняем поиск точки контура 
		// лежащей справа или слева от точки с номером pointIndex
		// если пик направлен во вне контура возвращаем null

		if ((m_Points[prevPoint].Y - m_Points[nextPoint].Y) == 0)
		{
			// Имеем пик
			if (m_Points[prevPoint].Y > m_Points[pointIndex].Y)
			{
				// Пик направленн вверх
				// Пик направлен внутрь контура если точка НАД пиком 
				// лежит внутри контура или принадлежит контуру

				if (!ContainsPoint(m_Points[pointIndex].X, m_Points[pointIndex].Y - 1)
					&&
					!PointLaysOnContour(m_Points[pointIndex].X, m_Points[pointIndex].Y - 1)
					)
					return nullptr;

			}
			if (m_Points[prevPoint].Y < m_Points[pointIndex].Y)
			{
				// Пик направленн вниз
				// Пик направлен внутрь контура если точка ПОД пиком 
				// лежит внутри контура или принадлежит контуру

				// точка под пиком
				if (!ContainsPoint(m_Points[pointIndex].X, m_Points[pointIndex].Y + 1)
					&&
					!PointLaysOnContour(m_Points[pointIndex].X, m_Points[pointIndex].Y + 1)
					)
					return nullptr;
			}
			if (m_Points[prevPoint].Y == m_Points[pointIndex].Y)
			{
				// Имеем плато из трёх точек (минимум)
				switch (direction)
				{
				case ContourHelpers::Contour::Left:
					return (m_Points[prevPoint].X < m_Points[nextPoint].X) ? &m_Points[prevPoint] : &m_Points[nextPoint];
				case ContourHelpers::Contour::Right:
					return (m_Points[prevPoint].X < m_Points[nextPoint].X) ? &m_Points[nextPoint] : &m_Points[prevPoint];
				default:
					break;
				} 
			}
		}

		// В цикле просмотрим все точки контура и выберем ближайшую 
		// к точке с номером pointIndex
		for (int i = 0; i < this->Length; i++)
		{
			// Исключим из рассмотрения точку с номером pointIndex
			if (i == pointIndex)
				continue;

			if (m_Points[i].Y == m_Points[pointIndex].Y)
			{

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

	bool Contour::PointLaysOnContour(int x, int y)
	{
		for (Point p : m_Points)
		{
			if (p.X == x && p.Y == y)
				return true;
		}
		return false;
	}

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
		// Проверим принадлежность точки контуру
		Point currentPoint = Point(x, y);
		if (Contains(&currentPoint))
			return false;
		
		int contourCrossingCount = 0;
		// Выберем точку контура с Y = y, ближайшую справа к точке с координатами x, y.
		int currentPointIndex = GetRightNearestPointIndex(x, y);

		while (currentPointIndex >= 0)
		{
			// Получим индексы точек перед и после ближайшей точки справа 
			int prevPointIndex = GetPrevContourPointIndex(currentPointIndex);
			int nextPointIndex = GetNextContourPointIndex(currentPointIndex);

			// Сравним координаты Y точек соседних с найденной ближайшей справа точкой контура
			if (m_Points[prevPointIndex].Y == m_Points[currentPointIndex].Y)
			{
				// Если предыдущая точка контура лежит на той же горизонтали что текущая точка, начинаем двигаться
				// по контуру назад пока не найдётся точка лежащая выше или ниже текущей точки.
				while (m_Points[prevPointIndex].Y == m_Points[currentPointIndex].Y)
					prevPointIndex = GetPrevContourPointIndex(prevPointIndex);
			}

			if (m_Points[nextPointIndex].Y == m_Points[currentPointIndex].Y)
			{
				// Если следующая точка контура лежит на той же горизонтали что текущая точка, начинаем двигаться
				// по контуру вперёд пока не найдётся точка лежащая выше или ниже текущей точки.
				while (m_Points[nextPointIndex].Y == m_Points[currentPointIndex].Y)
					nextPointIndex = GetPrevContourPointIndex(nextPointIndex);
			}

			if (abs(m_Points[prevPointIndex].Y - m_Points[nextPointIndex].Y) == 2)
			{	
				// Если разность координат Y предыдущей и последующей точки равна 2 , это означает, что
				// предыдущая и следующая точка контура лежат по разные стороны горизонтали на которой лежит
				// найденная ближайшая справа точка контура, то есть горизонталь проведённая через ближайшую
				// справа точку пересекает контур в этой точке
				//		увеличиваем счётчик пересечений контура на 1
				++contourCrossingCount;
				// устанавливаем координату X найденной точки в качестве новой начальной координаты для поиска
				// следующей ближайшей точки контура справа
				x = m_Points[currentPointIndex].X;
			}

			x = m_Points[currentPointIndex].X;
			// Выберем следующую точку контура лежащую на горизонтали y
			currentPointIndex = GetRightNearestPointIndex(x, y);
		}

		return contourCrossingCount % 2 == 1 ? true : false;
	}
	

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
		for (int i=0; i< (int)m_Points.size(); i++)
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
