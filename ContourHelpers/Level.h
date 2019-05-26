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


	public:
		Level();
		Level(int width, int height, unsigned char color, unsigned char pixeldata[] );
		~Level();
		void Clear();
		void Rectify(int size);
		
		void GetLevelShapes(unsigned char* pPixelBuffer);

		Contour* FindExternalContour();
		Contour* FindInternalContour(Contour* parentContour);
		void EraseShape(Contour* externalContour, Contour*  internalContour);
		void FindAllContours();

		void FillContour(Contour* contour, unsigned char color);
		void RestoreContourContent(Contour* contour);

		bool FindFirstExternalContourPoint(Point& point);
		bool FindNextExternalContourPoint(Point& point, Direction direction);
		
		bool FindFirstInternalContourPoint(Contour* parentContour, Point& point);
		bool CheckNextInternalContourPoint(Contour* parentContour, Point& point, Direction direction);
		bool FindNextInternalContourPoint(Contour* parentContour, Point& point, Direction& direction);

		void EraseBuffer();
	private:
		
		void FillLine(Contour* externalContour, int startPointNumber, Contour::SearchNearestPointDirection direction, unsigned char color);
		void RestoreLine(Contour* externalContour, int startPointNumber, Contour::SearchNearestPointDirection direction);

		unsigned char GetPixel(int x, int y);
		unsigned char GetPixel(int position);
		unsigned char GetPixel(Point* point);

		void SetPixel(int x, int y, unsigned char color);
		void SetPixel(int pos, unsigned char color);
		void SetPixel(Point* point, unsigned char color);

		void RestorePixel(int x, int y);
		void RestorePixel(Point* point);
		
		bool Level::BorderHasOnlyOneColor(int x, int y, int size);
		void RemoveShape(Contour* contour);

		Direction StartDirection(Direction direction);
		Direction NextDirection(Direction direction);
		Direction PrevDirection(Direction direction);

		Point* Level::GetCorrespondingContourPoint(Contour* externalContour, Contour* internalContour, Point* startPoint, Contour::SearchNearestPointDirection searchDirection);

		/*----------------------------------*/
		/* Debug functions section start    */
	public:
		static void ExpandLevelData(int width, int height, unsigned char color, unsigned char* inBuffer, unsigned char* outBuffer);
		bool CompareLevelDataWithReferenceData(unsigned char* pReferenceData, wchar_t* message, int messageLength);
		/* Debug functions section end      */
		/*----------------------------------*/


	};

	
}

