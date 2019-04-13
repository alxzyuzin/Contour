// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the ROOTERLIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// ROOTERLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.



#pragma once
#include <ctime>
#include "Contour.h"


using namespace std;

//#ifdef ROOTERLIB_EXPORTS
//#define ROOTERLIB_API  __declspec(dllexport)
//#else
//#define ROOTERLIB_API __declspec(dllimport)
//#endif //ROOTERLIB_EXPORTS

namespace ContourHelpers
{
	enum Direction { N, NE, E, SE, S, SW, W, NW };

	class Level sealed
	{

	public:
		unsigned char	 m_Color;
		vector<Contour*> m_Contours;

	public:
		Level();
		Level(int width, int height, unsigned char color, unsigned char pixeldata[] );
		~Level();

	public:
		
		static void ExpandLevelData(int width, int height, unsigned char color, unsigned char* inBuffer, unsigned char* outBuffer);
		
		void Clear();
		void Outline();
		void FindAllContours();
		void Rectify(int size);
		unsigned char GetPixel(int x, int y);
		unsigned char GetPixel(int position);
		void SetPixel(int pos, unsigned char color);
		void GetLevelShapes(unsigned char* pPixelBuffer);

		void FindInternalContours(Contour* parentContour, unsigned char shapeColor);
		
		Contour* FindContour(Contour* parentContour, unsigned char shapeColor);
		
		bool	 FindFirstInternalContourPoint(Contour* parentContour, Point& point);
		Contour* FindInternalContour(Contour* parentContour);

		bool FindFirstContourPoint(Contour* parentContour, Point& point, unsigned char shapeColor);
		bool FindNextContourPoint(Contour* parentContour, Point& point, Direction direction, unsigned char shapeColor);


	private:
		void SetPixel(Point* point, unsigned char color);
		void SetPixel(int x, int y, unsigned char color);
		unsigned char GetPixel(Point* point);
		Contour* FindExternalContour(Contour* parentContour);
		Point* FindFirstExternalContourPoint(Contour* parentContour);
		Point* FindNextExternalContourPoint(Point* point, Direction startDirection);
		bool Level::BorderHasOnlyOneColor(int x, int y, int size);
		void RemoveShape(Contour* contour);

//		void SortContourPointsByY(std::vector<Point^> points, int firstindex, int lastindex);
//		void SortContourPointsByX(std::vector<Point^> points, int firstindex, int lastindex);
//		int SortPass(std::vector<Point^> points, int firstindex, int lastindex);

		inline void DrawHorizontalLine(int x1, int x2, int y, unsigned char color);

		Direction StartDirection(Direction direction);
		Direction NextDirection(Direction direction);
		Direction PrevDirection(Direction direction);

		static 	int comparePoints(const void * a, const void * b);

	private:	 // Members
		int m_Width;
		int m_Height;
		unsigned char* m_pBuffer;
		unsigned char* m_pShapesBuffer;
		int m_BufferLength;


		const unsigned char EMPTY_COLOR = 0xFF;
	};

	
}

