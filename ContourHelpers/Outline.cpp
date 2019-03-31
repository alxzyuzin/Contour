#include "pch.h"
#include "Outline.h"

using namespace ContourHelpers;
//=============================================================================
// Class Point
//=============================================================================

int  Point::X::get() { return m_X; }
void Point::X::set(int x) { m_X = x; }

int  Point::Y::get() { return m_Y; }
void Point::Y::set(int y) { m_Y = y; }


//=============================================================================
// Class Outline
//=============================================================================

int Outline::Length::get() { return m_Points.size(); }
Array<Point^>^ Outline::Points::get()
{
	Array<Point^>^ points = ref new Array<Point^>(m_Points.size());
	for (unsigned int i = 0; i < m_Points.size(); i++)
			points[i] = m_Points[i];
	
	return points; 
}


int Outline::MinX::get() { return m_MinX; }
int Outline::MaxX::get() { return m_MaxX; }
int Outline::MinY::get() { return m_MinY; }
int Outline::MaxY::get() { return m_MaxY; }

Outline::Outline()
{
}

void Outline::AddPoint(Point^ point)
{
	m_Points.push_back(point);
	if (m_Points.size() == 1)
	{
		m_MaxX = point->X;
		m_MinX = point->X;
		m_MaxY = point->Y;
		// MinY - это всегда Y первой точки контура исходя их алгоритма поиска первой точки 
		// так что здесь можно сэкономить одну операцию присваивания
		m_MinY = point->Y;   
	}
	else
	{
		if (point->X > m_MaxX) m_MaxX = point->X;
		if (point->X < m_MinX) m_MinX = point->X;
		if (point->Y > m_MaxY) m_MaxY = point->Y;
		if (point->Y < m_MinY) m_MinY = point->Y;
	}
}

bool Outline::Contain(Point^ point)
{
	for (Point^ p : m_Points)
	{
		if ((p->X == point->X) && (p->Y == point->Y))
			return true;
	}
	return false;
}