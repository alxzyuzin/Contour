#pragma once
#include <vector>

using namespace std;

namespace ContourHelpers
{
	//enum ContourType { External, Internal };

	struct Point
	{
		int X;
		int Y;

		Point(int x, int y)
		{
			X = x;
			Y = y;
		}

		//bool operator ==(const Point& point);

		friend bool operator==(const Point& l, const Point& r)
		{
			return l.X == r.X && l.Y == r.Y;
		}

	};

	

	class Contour sealed
	{
	private:
		
		vector<Point> m_Points;
		unsigned char m_Color;

	public:
		Contour();
		Contour(unsigned char contourColor);
		~Contour();

		void AddPoint(Point point);
		int Size();
		Point* GetPoint(int i);
		unsigned char   GetColor();

		Point* FindLeftNearestPoint(int pointnumber);
		Point* FindRightNearestPoint(int pointnumber);
		Point* FindRightNearestPoint(Point* point);

		Point& operator[](unsigned int i);
		bool Contains(Point* point);
		bool ContainPoint(int x, int y);
		bool ContainPoint(Point* point);
		bool EnclosePoint(Point* point);

	};

}