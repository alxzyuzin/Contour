#pragma once
#include <ctime>
//#include "Outline.h"

using namespace std;

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
	};

	private enum Direction { N, NE, E, SE, S, SW, W, NW };

	class Level sealed
	{

	public:
		byte m_Color;
		vector<vector<Point*>*> m_Contours;

	public:
		Level(int width, int height, byte color, byte* pixeldata );
		~Level();

	public:
		void Clear();
		void FindAllContours();
		void Rectify(int size);
		byte GetPixel(int x, int y);
		byte GetPixel(int position);
		void SetPixel(int pos, byte color);
		void GetLevelShapes(byte* pPixelBuffer);

	private:
		void SetPixel(Point* point, byte color);
		[Windows::Foundation::Metadata::DefaultOverloadAttribute]
		void SetPixel(int x, int y, byte color);
		byte GetPixel(Point* point);
		[Windows::Foundation::Metadata::DefaultOverloadAttribute]
		vector<Point*>* FindContour();
		Point* FindFirstContourPoint();
		Point* FindNextContourPoint(Point* point, Direction startDirection);
		Point* FindLeftNearestPoint(std::vector<Point*>* contour, int pointnumber);
		Point* FindRightNearestPoint(std::vector<Point*>* contour, int pointnumber);
		bool Level::BorderHasOnlyOneColor(int x, int y, int size);
		void RemoveShape(std::vector<Point*>* contour);

//		void SortContourPointsByY(std::vector<Point^> points, int firstindex, int lastindex);
//		void SortContourPointsByX(std::vector<Point^> points, int firstindex, int lastindex);
//		int SortPass(std::vector<Point^> points, int firstindex, int lastindex);

		inline void DrawHorizontalLine(int x1, int x2, int y, byte color);

		Direction StartDirection(Direction direction);
		Direction NextDirection(Direction direction);
		Direction PrevDirection(Direction direction);

		static 	int comparePoints(const void * a, const void * b);

	private:	 // Members
		int m_Width;
		int m_Height;
		byte* m_pBuffer;
		byte* m_pShapesBuffer;
		int m_BufferLength;


	};

}

