#pragma once
namespace ContourExtractorWindowsRuntimeComponent
{
	class MapEntry sealed
	{
	private:
		
		int m_PointNumbers[4]{ -1,-1,-1,-1};
		byte m_numberOfPoints = 0;

	public:
		MapEntry();
		

		using const_iterator = const int*;
		using iterator = int*;

		const_iterator cbegin() const { return m_PointNumbers; }
		const_iterator cend() const { return m_PointNumbers + m_numberOfPoints; }
		const_iterator begin() const { return cbegin(); }
		const_iterator end() const { return cend(); }

		iterator begin() { return m_PointNumbers; }
		iterator end() { return m_PointNumbers + m_numberOfPoints; }
		

		int operator[](int index);

		bool IsDuplicated();
		int  Size();
		void AddPointNumber(int pointnumber);
		int* GetPointNumbers() { return m_PointNumbers; }
			
	
		

	};
}