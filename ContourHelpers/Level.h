#pragma once
#include <ctime>
#include <map>
#include "Contour.h"

using namespace std;

namespace ContourHelpers
{
	
	enum Direction { N, NE, E, SE, S, SW, W, NW };
	enum DeltaYSign {Positive, Negative};

	class Level sealed
	{
	public:
		unsigned char	 m_Color;
		vector<Contour*> m_Contours;

	public:
		Level();
		Level(int width, int height, unsigned char color, unsigned char pixeldata[] );
		~Level();
		void Clear();
		void Outline();
		void Rectify(int size);
		void FindAllContours();
		void GetLevelShapes(unsigned char* pPixelBuffer);

		void FindInternalContours(Contour* parentContour, unsigned char shapeColor);
		Contour* FindContour(Contour* parentContour, unsigned char shapeColor);
		
		
		
		
		bool	 FindFirstInternalContourPoint(Contour* parentContour, Point& point);
		bool	 CheckNextInternalContourPoint(Contour* parentContour, Point& point, Direction direction);
		bool	 FindNextInternalContourPoint(Contour* parentContour, Point& point, Direction& direction);
		Contour* FindInternalContour(Contour* parentContour);

		bool FindFirstContourPoint(Contour* parentContour, Point& point);
		bool FindNextContourPoint(Contour* parentContour, Point& point, Direction direction, unsigned char shapeColor);

		void EraseShape(Contour* externalContour, Contour*  internalContour);


		

	private:
		unsigned char GetPixel(int x, int y);
		void SetPixel(int x, int y, unsigned char color);
		unsigned char GetPixel(int position);
		void SetPixel(int pos, unsigned char color);
		unsigned char GetPixel(Point* point);
		void SetPixel(Point* point, unsigned char color);

		bool Level::BorderHasOnlyOneColor(int x, int y, int size);
		void RemoveShape(Contour* contour);

		inline void DrawHorizontalLine(int x1, int x2, int y, unsigned char color);

		Direction StartDirection(Direction direction);
		Direction NextDirection(Direction direction);
		Direction PrevDirection(Direction direction);

		Point* Level::GetCorrespondingContourPoint(Contour* externalContour, Contour* internalContour, Point* startPoint, Contour::SearchNearestPointDirection searchDirection);

//		static 	int comparePoints(const void * a, const void * b);

		/*----------------------------------*/
		/* Debug functions section start    */
	public:
		static void ExpandLevelData(int width, int height, unsigned char color, unsigned char* inBuffer, unsigned char* outBuffer);
		bool CompareLevelDataWithReferenceData(unsigned char* pReferenceData, wchar_t* message, int messageLength);
		/* Debug functions section end      */
		/*----------------------------------*/


	private:	 // Members
		int m_Width;
		int m_Height;
		unsigned char* m_pBuffer;
		unsigned char* m_pShapesBuffer;
		int m_BufferLength;
		
		map<Direction, vector<Direction>*> m_ClockwiseDirectionMap = 
		{
			{N , new vector<Direction> {SW, W,  NW, N,  NE, E,  SE, S }},
			{NE, new vector<Direction> {W,  NW, N,  NE, E,  SE, S,  SW}},
			{E , new vector<Direction> {NW, N,  NE, E,  SE, S,  SW, W }},
			{SE, new vector<Direction> {N,  NE, E,  SE, S,  SW, W,  NW}},
			{S , new vector<Direction> {NE, E,  SE, S,  SW, W,  NW, N }},
			{SW, new vector<Direction> {E,  SE, S,  SW, W,  NW, N,  NE}},
			{W , new vector<Direction> {SE, S,  SW, W,  NW, N,  NE, E }},
			{NW, new vector<Direction> {S,  SW, W,  NW, N,  NE, E,  SE}}
		};
		 
		const unsigned char EMPTY_COLOR = 0xFF;
	};

	
}

