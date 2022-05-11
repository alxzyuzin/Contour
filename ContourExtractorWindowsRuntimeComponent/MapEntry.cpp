#include "pch.h"
#include "MapEntry.h"

namespace ContourExtractorWindowsRuntimeComponent
{
	MapEntry::MapEntry()
	{}

	int MapEntry::operator[](int index)
	{
		return m_PointNumbers[index];
	}
	bool MapEntry::IsDuplicated()
	{
		return (m_numberOfPoints > 1);
	}
	int MapEntry::Size()
	{
		return m_numberOfPoints;
	}
	

	void MapEntry::AddPointNumber(int pointnumber)
	{
		m_PointNumbers[m_numberOfPoints] = pointnumber;
		m_numberOfPoints++;
	}
	
}