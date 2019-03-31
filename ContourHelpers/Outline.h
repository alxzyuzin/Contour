#pragma once
using namespace Platform;
namespace ContourHelpers
{
	
	

	/*
	public ref class Point sealed
	{
	public:
		Point(int x, int y)
		{
			m_X = x;
			m_Y = y;
		}


		property int X
		{
			int get();
			void set(int x);
		}

		property int Y
		{
			int get();
			void set(int y);
		};

		
	private:
		int m_X; 
		int m_Y;
	
	};
	*/
	

	public class Outline sealed
	{
	public:
		Outline();

		//property Array<Point^>^ Points {Array<Point^>^ get(); }


		//property int Length { int get(); }
		//property int MinX {int get(); }
		//property int MaxX {int get(); }
		//property int MinY {int get(); }
		//property int MaxY {int get(); }
				
		void AddPoint(Point* point);
		bool Contain(Point* point);

	private:
		std::vector<Point> m_Points;
	
		//int m_MinX = 0;
		//int m_MaxX = 0;
		//int m_MinY = 0;
		//int m_MaxY = 0;

	};
}

