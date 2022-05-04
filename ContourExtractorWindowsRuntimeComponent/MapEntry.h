#pragma once
namespace ContourExtractorWindowsRuntimeComponent
{
	class MapEntry_ sealed
	{
	private:
		
		int m_PointNumbers[4]{ -1,-1,-1 };
		byte m_numberOfPoints = 0;

	public:

		using const_iterator = const int*;

		const_iterator cbegin() const { return m_PointNumbers; }
		const_iterator cend() const { return m_PointNumbers + m_numberOfPoints; }
		const_iterator begin() const { return cbegin(); }
		const_iterator end() const { return cend(); }

		int operator[](int index);

		bool IsDuplicated();
		int  Size();
		void AddPoint(int pointnumber);
	
		

	};
}