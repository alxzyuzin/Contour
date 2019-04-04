#pragma once
#include <ctime>
#include "Contour.h"

using namespace std;

namespace ContourHelpers
{
	

	private enum Direction { N, NE, E, SE, S, SW, W, NW };

	class Level sealed
	{

	public:
		byte m_Color;
		vector<Contour*> m_Contours;

	public:
		Level(int width, int height, byte color, byte* pixeldata );
		~Level();

	public:
		void Clear();
		void Outline();
		void FindAllContours();
		void Rectify(int size);
		byte GetPixel(int x, int y);
		byte GetPixel(int position);
		void SetPixel(int pos, byte color);
		void GetLevelShapes(byte* pPixelBuffer);

		void FindInternalContours(Contour* parentContour, byte shapeColor);
		Contour* FindContour(Contour* parentContour, byte shapeColor);
		Point* FindFirstContourPoint(Contour* parentContour, byte shapeColor);
		Point* FindNextContourPoint(Contour* parentContour, Point* point, Direction direction, byte shapeColor);


	private:
		void SetPixel(Point* point, byte color);
		[Windows::Foundation::Metadata::DefaultOverloadAttribute]
		void SetPixel(int x, int y, byte color);
		byte GetPixel(Point* point);
		[Windows::Foundation::Metadata::DefaultOverloadAttribute]
		Contour* FindExternalContour(Contour* parentContour);
		Point* FindFirstExternalContourPoint(Contour* parentContour);
		Point* FindNextExternalContourPoint(Point* point, Direction startDirection);
		bool Level::BorderHasOnlyOneColor(int x, int y, int size);
		void RemoveShape(Contour* contour);

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

