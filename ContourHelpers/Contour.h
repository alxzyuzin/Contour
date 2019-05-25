#pragma once
#include <vector>
#include <set>



namespace ContourHelpers
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

		//bool operator ==(const Point& point);

		friend bool operator==(const Point& l, const Point& r)
		{
			return l.X == r.X && l.Y == r.Y;
		}

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
		unsigned char m_Color;
	
	public:

		Contour();
		Contour(ContourType type);
		
		Contour(unsigned char contourColor);
		Contour(Point* contourPoints, int pointsNumber);
		~Contour();

		void AddPoint(Point point);
		int Size();
		Point* GetPoint(int i);
		unsigned char   GetColor();

		int GetMinY();
		int GetMaxY();
		Point* Contour::GetMostLeftContourPoint(int y);


		Point* FindLeftNearestPoint(int pointnumber);
		Point* FindRightNearestPoint(int pointnumber);
		Point* GetRightNearestContourPoint(Point* point);
		Point* GetLeftNearestContourPoint(Point* point);
		
		enum SearchNearestPointDirection {Left, Right};
		Point* GetNearestContourPoint(Point* point, SearchNearestPointDirection direction);
		Point* GetNearestContourPoint(int pointIndex, SearchNearestPointDirection direction);
		Point* GetNearestInternalContourPoint(Point* externalContourPoint, SearchNearestPointDirection diirection);

//		Point* GetPrevContourPoint(Point* point);
//		Point* GetNextContourPoint(Point* point);

		int GetRightNearestPointIndex(int x, int y);
		int GetNextContourPointIndex(int i);
		int GetPrevContourPointIndex(int i);
		bool PointLaysOnContour(int x, int y);

		Point* operator[](unsigned int i);
		bool Contains(Point* point);
		bool ContainsPoint(int x, int y);
//		bool ContainsPoint(Point* point);
//		bool EnclosePoint(Point* point);

		std::vector<int>*  GetRightPoints(int x, int y);

	};

}