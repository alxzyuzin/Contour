#pragma once
#include <ctime>
#include <map>
#include <ppltasks.h>
#include "Contour.h"
#include "Color.h"

using namespace std;
using namespace concurrency;
using namespace Windows::Foundation;


namespace ContourExtractorWindowsRuntimeComponent
{

	enum Direction { N, NE, E, SE, S, SW, W, NW };
	enum DeltaYSign { Positive, Negative };

	class Level sealed
	{
	public:
		unsigned char	 m_Color;
		unsigned int     m_OriginalColor;
		vector<Contour*> m_Contours;

	private:	 // Members
		int m_Width;
		int m_Height;
		unsigned char* m_Buffer;
		unsigned char* m_BufferCopy;
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
		const unsigned char BLACK_COLOR = 0x00;

	public:
		Level();
		Level(int width, int height, pair<unsigned int, unsigned char> colorPair, unsigned int* imageData);
				
		~Level();
		void Clear();
		void Rectify(int size);

		void SetLevelShapesToDisplayBuffer(unsigned int* ImageData);
		void SetContoursToDisplayBuffer(unsigned int*  ImageData, ContourColors color, int minContourLength, unsigned char contourDensity, ContourType type);

		Contour* FindExternalContour();
		Contour* FindInternalContour(Contour* parentContour);
		void EraseShape(Contour* externalContour, Contour*  internalContour);
		IAsyncActionWithProgress<int>^ FindAllContours();

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

		void SetPixel(int x, int y, unsigned char color);
		void SetPixel(Point* point, unsigned char color);

		void RestorePixel(int x, int y);

		Direction StartDirection(Direction direction);
		Direction NextDirection(Direction direction);
	
	public:
		bool Level::ClearArea(int x, int y, int size, unsigned char color);
	/*----------------------------------*/
	/* Debug functions section start    */
	public:
		static void ExpandLevelData(int width, int height, unsigned char color, unsigned char* inBuffer, unsigned char* outBuffer);
		bool CompareLevelDataWithReferenceData(unsigned char* pReferenceData, wchar_t* message, int messageLength);
		bool CompareLevelBufferWithReferenceData(unsigned char* referenceData);
	/* Debug functions section end      */
	/*----------------------------------*/
	};


}

