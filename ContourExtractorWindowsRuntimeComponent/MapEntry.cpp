#include "pch.h"
#include "MapEntry.h"

namespace ContourExtractorWindowsRuntimeComponent
{
	int MapEntry_::operator[](int index)
	{
		return m_PointNumbers[index];
	}
	bool MapEntry_::IsDuplicated()
	{
		return (m_numberOfPoints > 1);
	}
	int MapEntry_::Size()
	{
		return m_numberOfPoints;
	}
	

	void MapEntry_::AddPoint(int pointnumber)
	{
		m_PointNumbers[m_numberOfPoints] = pointnumber;
		m_numberOfPoints++;
	}
	
}