#pragma once
#pragma once
#include <vector>
#include <set>



namespace ContourExtractorWindowsRuntimeComponent
{
	struct Point
	{
		int X;
		int Y;

		Point(int x, int y)
		{
			X = x;
			Y = y;
		}

		bool operator ==(const Point& point);

	};


	class Contour sealed
	{
	public:
		enum ContourType { External, Internal };
		int Length = 0;
		ContourType Type = ContourType::External;

	private:
		std::vector<Point> m_Points;
		// Структура данных в m_PointsMap
		// первое значение с типом int - координата Y точки контура 
		// второе значение с типом int - координата X точки контура
		// третье значение с типом int - номер точки с координатами X,Y в векторе m_Points.
		// Контур может содержать несколько точек с координатами X,Y.
		std::map<int, std::map<int, std::vector<int>*>*> m_PointsMap;

	public:

		Contour();
		Contour(ContourType type);
		Contour(Point* contourPoints, int pointsNumber);
		~Contour();

		void AddPoint(Point point);
		int Size();
		Point* GetPoint(int i);

		Point* FindLeftNearestPoint(int pointnumber);
		Point* FindRightNearestPoint(int pointnumber);

		enum SearchNearestPointDirection { Left, Right };
		Point* GetNearestContourPoint(int pointIndex, SearchNearestPointDirection direction);

		int GetNextContourPointIndex(int i);
		int GetPrevContourPointIndex(int i);
		bool PointLaysOnContour(int x, int y);

		Point* operator[](unsigned int i);
		bool ContainsPoint(int x, int y);

	};

}