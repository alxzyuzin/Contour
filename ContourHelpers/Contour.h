#pragma once

using namespace std;

namespace ContourHelpers
{
	enum ContourType { External, Internal };

	struct Point
	{
		int X;
		int Y;

		Point(int x, int y)
		{
			X = x;
			Y = y;
		}
	};

	class Contour sealed
	{
	public:
		ContourType Type;

	public:
		Contour();
		Contour::Contour(ContourType type);
		~Contour();

		void AddPoint(Point* point);
		int Size();
		Point* GetPoint(int i);

		//Point* Contour::FindLeftNearestPoint(Contour* contour, int pointnumber);
		//Point* Contour::FindRightNearestPoint(Contour* contour, int pointnumber);

		Point* Contour::FindLeftNearestPoint(int pointnumber);
		Point* Contour::FindRightNearestPoint(int pointnumber);

		Point& operator[](int i);
		

	private:
		vector<Point> m_Points;

	};

}