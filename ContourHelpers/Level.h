// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the ROOTERLIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// ROOTERLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.



#pragma once
#include <ctime>
#include <map>
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
		bool	 CheckNextInternalContourPoint(Contour* parentContour, Point& point, Direction direction);
		bool	 FindNextInternalContourPoint(Contour* parentContour, Point& point, Direction& direction);
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
		
		map<Direction, array<Direction, 8>*> m_ClockwiseDirectionMap = 
		{
			{N ,new array<Direction, 8> {SW, W,  NW, N,  NE, E,  SE, S }},
			{NE,new array<Direction, 8> {W,  NW, N,  NE, E,  SE, S,  SW}},
			{E ,new array<Direction, 8> {NW, N,  NE, E,  SE, S,  SW, W }},
			{SE,new array<Direction, 8> {N,  NE, E,  SE, S,  SW, W,  NW}},
			{S ,new array<Direction, 8> {NE, E,  SE, S,  SW, W,  NW, N }},
			{SW,new array<Direction, 8> {E,  SE, S,  SW, W,  NW, N,  NE}},
			{W ,new array<Direction, 8> {SE, S,  SW, W,  NW, N,  NE, E }},
			{NW,new array<Direction, 8> {S,  SW, W,  NW, N,  NE, E,  SE}}
		};
		
	 	map<Direction, array<Direction,8>*> m_CounterClockwiseDirectionMap =
		{
			{N , new array<Direction,8>{NE, N,  NW, W,  SW, S,  SE, E }},
			{NE, new array<Direction,8>{E,  NE, N,  NW, W,  SW, S,  SE}},
			{E , new array<Direction,8>{SE, E,  NE, N,  NW, W,  SW, S }},
			{SE, new array<Direction,8>{S,  SE, E,  NE, N,  NW, W,  SW}},
			{S , new array<Direction,8>{SW, S,  SE, E,  NE, N,  NW, W }},
			{SW, new array<Direction,8>{W,  SW, S,  SE, E,  NE, N,  NW}},
			{W , new array<Direction,8>{NW, W,  SW, S,  SE, E,  NE, N }},
			{NW, new array<Direction,8>{N,  NW, W,  SW, S,  SE, E,  NE}}
		};
		
		map<Direction, array<Direction,8>*> testmap =
		{
			{N, new array<Direction,8> {N,W,S,E,E,S,W,N}},
			{W, new array<Direction,8>{E,S,W,N,N,W,S,E}}
		};
		const unsigned char EMPTY_COLOR = 0xFF;
	};

	
}

