#include "pch.h"
#include "Contour.h"

using namespace std;

namespace ContourHelpers
{
	Contour::Contour()
	{
		
	}

	Contour::Contour(ContourType type)
	{
		Type = type;
	}

	Contour::~Contour()
	{
		
	}

	Point& Contour::operator[](int i) {
		if (i >  m_Points.size()) {
			return m_Points[0];
		}

		return m_Points[i];
	}

	void Contour::AddPoint(Point point)
	{
		m_Points.push_back(point);
	}

	Point* Contour::GetPoint(int i)
	{
      return &m_Points[i];
	}

	int Contour::Size()
	{
		return m_Points.size();
	}

	/* Находит ближайшую слева точку контура лежащую в той же строке что и заданная
----------------------------------------------------------------------------*/
	Point* Contour::FindLeftNearestPoint(int pointnumber)
	{
		// Найдём ближайжую слева точку контура лежащую в той же строке что и p2
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

	Point* Contour::FindRightNearestPoint(Point* point)
	{
		// Найдём точку контура ближайшую к точке с координатами point.X, point.Y лежащую справа в той же строке что и point.  
		int lastDistance = MAXINT;
		Point *p = nullptr;
		for (unsigned int i = 0; i < m_Points.size(); i++)
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


	bool Contour::Contains(Point* point)
	{
		for (Point p : m_Points)
		{
			if (p.X == point->X && p.Y == point->Y)
				return true;
		}
		return false;
	}
	bool Contour::ContainPoint(int x, int y)
	{
		for (Point point : m_Points)
		{
			if (point.X == x && point.Y == y)
				return true;
		}
		return false;
	}
	
	/*
		Проверяет принадлежит ли заданная точка контуру
		Возвращаемое значение
			true -  если точка является точкой контура
			false - если точка не является точкой контура
	*/
	bool Contour::ContainPoint(Point* point)
	{
		for (Point contourPoint : m_Points)
		{
			if (contourPoint.X == point->X && contourPoint.Y == point->Y)
				return true;
		}
		return false;
	}
	/*
		Проверяет лежит ли заданная точка внутри контура
		Возвращаемое значение 
			true -  если точка лежит внутри контура или принадлежит контуру
			false - если точка лежит вне контура
	*/
	bool Contour::EnclosePoint(Point* point)
	{
		int i = 0;
		Point* rightNearestPoint = FindRightNearestPoint(point);
		while (rightNearestPoint)
		{
			++i;
			rightNearestPoint = FindRightNearestPoint(rightNearestPoint);
		}
		// Если количество точек контура справа от заданной точки чётное,
		// то точка лежит вне контура. Иначе внутри контура
		return i % 2 == 0 ? false : true;
	}

	
}  // namespace ContourHelpers
