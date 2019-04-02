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

	void Contour::AddPoint(Point* point)
	{
		m_Points.push_back(*point);
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
		// Найдём точку контура ближайжую к точке с номером pointnumber лежащую справа в той же строке что и p2.  
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

}
